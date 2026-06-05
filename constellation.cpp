#include "Jukebox.h"
#include <cstring>
#include <cmath>
#include <algorithm>

enum EngineState {
    STATE_IDLE = 0,
    STATE_LISTENING = 1,
    STATE_EVALUATING_SILENCE = 2,
    STATE_PATIENCE = 3,
    STATE_PLAYING = 4
};

struct RecordedNote {
    TJBox_UInt8   pitch;
    TJBox_UInt8   velocity;
    TJBox_Float64 onsetPPQ;
    TJBox_Float64 durationPPQ;
};

struct GeneratedNote {
    TJBox_UInt8   pitch;
    TJBox_UInt8   velocity;
    TJBox_Float64 onsetPPQ;
    TJBox_Float64 durationPPQ;
};

struct RippleData {
    bool  active;
    float x, y, radius, alpha;
    bool  isOutput;
};

// ---------------------------------------------------------------------------
class MusicTheory {
public:
    static const int Major[7], Minor[7], HarmonicMinor[7], MelodicMinor[7];
    static const int Dorian[7], Phrygian[7], Lydian[7], Mixolydian[7];

    static const int* GetScale(int mode) {
        switch(mode) {
            case 0: return Major;       case 1: return Minor;
            case 2: return HarmonicMinor; case 3: return MelodicMinor;
            case 4: return Dorian;      case 5: return Phrygian;
            case 6: return Lydian;      case 7: return Mixolydian;
            default: return Major;
        }
    }

    static int GetScaleDegree(int pitch, int root, int mode) {
        if (mode == 8) return pitch % 12;
        int st = (pitch - root) % 12;
        if (st < 0) st += 12;
        const int* sc = GetScale(mode);
        int best = 0, minD = 12;
        for (int i = 0; i < 7; i++) {
            int d = std::abs(st - sc[i]);
            if (d < minD) { minD = d; best = i; }
        }
        return best;
    }

    static int GetPitchFromDegree(int degree, int octave, int root, int mode) {
        if (mode == 8) {
            int p = (octave * 12) + root + degree;
            return std::max(0, std::min(127, p));
        }
        while (degree < 0) { degree += 7; octave--; }
        while (degree > 6) { degree -= 7; octave++; }
        int p = (octave * 12) + root + GetScale(mode)[degree];
        return std::max(0, std::min(127, p));
    }

    static TJBox_UInt32 SeededRandom(TJBox_UInt32& state) {
        state = state * 1664525 + 1013904223;
        return state;
    }
};

const int MusicTheory::Major[7]         = {0,2,4,5,7,9,11};
const int MusicTheory::Minor[7]         = {0,2,3,5,7,8,10};
const int MusicTheory::HarmonicMinor[7] = {0,2,3,5,7,8,11};
const int MusicTheory::MelodicMinor[7]  = {0,2,3,5,7,9,11};
const int MusicTheory::Dorian[7]        = {0,2,3,5,7,9,10};
const int MusicTheory::Phrygian[7]      = {0,1,3,5,7,8,10};
const int MusicTheory::Lydian[7]        = {0,2,4,6,7,9,11};
const int MusicTheory::Mixolydian[7]    = {0,2,4,5,7,9,10};

// ---------------------------------------------------------------------------
class Narcissus {
public:
    Narcissus(TJBox_Float64 iSampleRate) {
        fSampleRate = iSampleRate;
        fIsOn = true; fIsBypassed = false; fRefsInitialized = false;
        fState = STATE_IDLE; fActiveNotes = 0;
        fRecordedNoteCount = 0; fGeneratedNoteCount = 0;
        fTempo = 120.0;
        fFramesSinceAllNotesOff = 0; fFramesSinceSilence = 0;
        fPhraseLengthPPQ = 0.0; fPatiencePPQRemaining = 0.0;
        fPlaybackPPQ = 0.0; fShadowPlaybackPPQ = 0.0;
        fLoopCount = 0; fActiveRoot = 0;
        fFreezeOn = false; fFreezeJustReleased = false; fClearPending = false;
        fLimitHit = false; fPitchDirectionMult = 1;
        fAlterEgoNoteCount = 0;
        fAlterEgoActiveCount = 0;
        for (int i = 0; i < 128; i++) fAlterEgoActive[i] = false;
        fPrevSeedCV = 0.0f; fPrevClearCV = 0.0f;
        for (int i = 0; i < 12;  i++) fPitchClassHistogram[i] = 0;
        for (int i = 0; i < 512; i++) fMutationOffsets[i] = 0;
        fWavePhase = 0.0f; fRippleTimer = 0; fCurrentSeed = 1000;
        for (int i = 0; i < 128; ++i) {
            fActiveNoteBufferIndex[i] = -1;
            fCurrentlyPlayingVoices[i] = 0;
            fInputRippleX[i] = 0.5f; fOutputRippleX[i] = 0.5f;
            fDuetPrimary[i] = -1; fDuetSecondary[i] = -1;
        }
        fDuetQueueSize = 0; fDuetClockPPQ = 0.0;
        for (int i = 0; i < 64; ++i) {
            fInputRipples[i].active = false;
            fOutputRipples[i].active = false;
        }
    }

    // -----------------------------------------------------------------------
    void KillHangingNotes(TJBox_UInt16 frame) {
        for (int i = 0; i < 128; i++) {
            if (fCurrentlyPlayingVoices[i] > 0) {
                TJBox_NoteEvent ev; ev.fNoteNumber=i; ev.fVelocity=0; ev.fAtFrameIndex=frame;
                JBox_OutputNoteEvent(ev); fCurrentlyPlayingVoices[i] = 0;
            }
        }
        if (fRefsInitialized) {
            for (int i = 0; i < 128; i++) fAlterEgoActive[i] = false;
            fAlterEgoActiveCount = 0;
            JBox_StoreMOMProperty(fAlterEgoGateRef, JBox_MakeNumber(0.0));
        }
    }

    TJBox_Float64 FramesToPPQ(TJBox_UInt32 frames) {
        return (static_cast<TJBox_Float64>(frames) / fSampleRate) * (fTempo / 60.0) * 15360.0;
    }

    double SnapToNearestBeat(double ppq) {
        const double kBeat = 3840.0;
        double snapped = std::round(ppq / kBeat) * kBeat;
        return (snapped <= 0.0) ? kBeat : snapped;
    }

    double SyncStepToPPQ(int step) {
        switch(step) {
            case 0: return 0.0;      // Off
            case 1: return 240.0;    // 1/64
            case 2: return 480.0;    // 1/32
            case 3: return 960.0;    // 1/16
            case 4: return 1920.0;   // 1/8
            case 5: return 3840.0;   // 1/4
            case 6: return 7680.0;   // 1/2
            case 7: return 15360.0;  // 1 Bar
            case 8: return 30720.0;  // 2 Bars
            case 9: return 61440.0;  // 4 Bars
            default: return 3840.0;
        }
    }

    double GetSilenceFrames(bool sync, double freeVal, double syncVal) {
        double ppq;
        if (sync) {
            ppq = SyncStepToPPQ((int)syncVal);
            if (ppq <= 0.0) return 0.0;
            return (ppq / 15360.0) * (60.0 / fTempo) * fSampleRate;
        } else {
            return (freeVal * 4.0 + 0.05) * fSampleRate;
        }
    }

    double GetPatiencePPQ(bool sync, double freeVal, double syncVal) {
        if (sync) return SyncStepToPPQ((int)syncVal);
        return freeVal * 4.0 * 15360.0;
    }

    double RateToPPQ(int mode) {
        switch(mode) {
            case 1:  return 61440.0; case 2:  return 30720.0; case 3:  return 15360.0;
            case 4:  return 7680.0;  case 5:  return 11520.0; case 6:  return 5120.0;
            case 7:  return 3840.0;  case 8:  return 5760.0;  case 9:  return 2560.0;
            case 10: return 1920.0;  case 11: return 2880.0;  case 12: return 1280.0;
            case 13: return 960.0;   case 14: return 1440.0;  case 15: return 640.0;
            case 16: return 480.0;   case 17: return 720.0;   case 18: return 320.0;
            case 19: return 240.0;   default: return 0.0;
        }
    }

    void ResolveActiveRoot(int scaleRootKnob, int scaleMode) {
        if (scaleRootKnob == 13) {
            int bestRoot = 0, maxScore = -1;
            for (int r = 0; r < 12; ++r) {
                int score = 0; const int* sc = MusicTheory::GetScale(scaleMode);
                for (int s = 0; s < 7; ++s) score += fPitchClassHistogram[(r + sc[s]) % 12];
                if (score > maxScore) { maxScore = score; bestRoot = r; }
            }
            fActiveRoot = bestRoot;
        } else if (scaleRootKnob >= 0 && scaleRootKnob <= 11) {
            fActiveRoot = scaleRootKnob;
        }
    }

    // -----------------------------------------------------------------------
    void SortByOnset(int* idx, int n) {
        for (int i = 1; i < n; i++) {
            for (int j = i; j > 0; j--) {
                if (fRecordedPhrase[idx[j]].onsetPPQ < fRecordedPhrase[idx[j-1]].onsetPPQ) {
                    int t = idx[j]; idx[j] = idx[j-1]; idx[j-1] = t;
                } else break;
            }
        }
    }

    void SortByPitch(int* idx, int n, bool ascending) {
        for (int i = 1; i < n; i++) {
            for (int j = i; j > 0; j--) {
                int pA = fRecordedPhrase[idx[j]].pitch;
                int pB = fRecordedPhrase[idx[j-1]].pitch;
                bool swap = ascending ? (pA < pB) : (pA > pB);
                if (swap) { int t = idx[j]; idx[j] = idx[j-1]; idx[j-1] = t; }
                else break;
            }
        }
    }

    // -----------------------------------------------------------------------
    void ComputeGeneratedPhrase(int scaleMode, int responseMode,
                                int octaveShift, int transposeShift,
                                int harmonyShift, float memoryKnob,
                                double lengthMultiplier, double activeWindowPPQ,
                                int lowLimit, int highLimit,
                                GeneratedNote* gp, int& gc, bool& limitHit) {
        limitHit = false;
        int n = fRecordedNoteCount;
        if (n == 0) { gc = 0; return; }

        if (responseMode == 8) {
            bool seen[128] = {};
            int chordPitches[128], chordCount = 0;
            for (int j = 0; j < n; j++) {
                int p = fRecordedPhrase[j].pitch;
                if (!seen[p] && chordCount < 128) { seen[p] = true; chordPitches[chordCount++] = p; }
            }
            gc = 0;
            for (int k = 0; k < chordCount && gc < 512; k++) {
                int rawPitch = chordPitches[k];
                int origOct  = rawPitch / 12;
                int origDeg  = MusicTheory::GetScaleDegree(rawPitch, fActiveRoot, scaleMode);
                int degree   = origDeg + transposeShift;
                int octave   = origOct + octaveShift * fPitchDirectionMult;
                int genDeg   = degree + harmonyShift * fPitchDirectionMult;
                TJBox_UInt32 seed = fCurrentSeed + (k * 1999) + (fLoopCount * 777);
                TJBox_UInt32 memR = MusicTheory::SeededRandom(seed);
                if ((memR % 100) > (TJBox_UInt32)(memoryKnob * 100.0f)) {
                    TJBox_UInt32 jr = MusicTheory::SeededRandom(seed) % 100;
                    if (jr<40) genDeg+=(jr%2==0)?1:-1; else if(jr<70) genDeg+=(jr%2==0)?2:-2;
                    else if(jr<90) genDeg+=(jr%2==0)?3:4; else genDeg=0;
                }
                int pitch = MusicTheory::GetPitchFromDegree(genDeg, octave, fActiveRoot, scaleMode);
                if (pitch < lowLimit || pitch > highLimit) limitHit = true;
                pitch = std::max(lowLimit, std::min(highLimit, pitch));
                gp[gc].pitch       = (TJBox_UInt8)pitch;
                gp[gc].velocity    = 100;
                gp[gc].onsetPPQ    = 0.0;
                gp[gc].durationPPQ = activeWindowPPQ > 0.0 ? activeWindowPPQ : 15360.0;
                gc++;
            }
            return;
        }

        int sortedByOnset[512];
        for (int i = 0; i < n; i++) sortedByOnset[i] = i;
        SortByOnset(sortedByOnset, n);

        double timOnset[512], timDur[512];
        for (int slot = 0; slot < n; slot++) {
            int recIdx = sortedByOnset[slot];
            double onset = fRecordedPhrase[recIdx].onsetPPQ    * lengthMultiplier;
            double dur   = fRecordedPhrase[recIdx].durationPPQ * lengthMultiplier;
            if (dur <= 0.0) dur = 240.0;

            if (responseMode == 3) { // Retrograde
                onset = activeWindowPPQ - (onset + dur);
                if (onset < 0.0) onset = 0.0;
            } else if (responseMode == 2 && activeWindowPPQ > 0.0) { // Warp
                double norm = onset / activeWindowPPQ;
                onset = activeWindowPPQ * (norm * norm);
            } else if (responseMode == 5) { // Pendulum
                if (fLoopCount % 2 == 0) {
                    onset = activeWindowPPQ - (onset + dur);
                    if (onset < 0.0) onset = 0.0;
                }
            }
            timOnset[slot] = onset;
            timDur[slot]   = dur;
        }

        int pitchSrc[512];
        for (int i = 0; i < n; i++) pitchSrc[i] = sortedByOnset[i];

        if (responseMode == 4) { // Shuffle
            TJBox_UInt32 shuffleSeed = fCurrentSeed + (fLoopCount * 5555) + 99;
            for (int i = n - 1; i > 0; i--) {
                MusicTheory::SeededRandom(shuffleSeed);
                int j = (int)(shuffleSeed % (TJBox_UInt32)(i + 1));
                int t = pitchSrc[i]; pitchSrc[i] = pitchSrc[j]; pitchSrc[j] = t;
            }
        } else if (responseMode == 6) { // Cascade Up
            SortByPitch(pitchSrc, n, true);
        } else if (responseMode == 7) { // Cascade Down
            SortByPitch(pitchSrc, n, false);
        }

        gc = n;
        for (int slot = 0; slot < n; slot++) {
            int pSrc = pitchSrc[slot];
            int rawPitch = fRecordedPhrase[pSrc].pitch;
            int origOct  = rawPitch / 12;
            int origDeg  = MusicTheory::GetScaleDegree(rawPitch, fActiveRoot, scaleMode);

            int degree = origDeg + fMutationOffsets[pSrc] + transposeShift;
            int octave = origOct + octaveShift * fPitchDirectionMult;

            int genDeg = degree;
            if (responseMode == 1) genDeg = -genDeg; // Upside Down
            genDeg += harmonyShift * fPitchDirectionMult;

            TJBox_UInt32 baseSeed = fCurrentSeed + (slot * 1999) + (fLoopCount * 777);
            TJBox_UInt32 memR = MusicTheory::SeededRandom(baseSeed);
            if ((memR % 100) > (TJBox_UInt32)(memoryKnob * 100.0f)) {
                TJBox_UInt32 jr = MusicTheory::SeededRandom(baseSeed) % 100;
                if (jr<40) genDeg+=(jr%2==0)?1:-1; else if(jr<70) genDeg+=(jr%2==0)?2:-2;
                else if(jr<90) genDeg+=(jr%2==0)?3:4; else genDeg=0;
            }

            int pitch = MusicTheory::GetPitchFromDegree(genDeg, octave, fActiveRoot, scaleMode);

            if (pitch < lowLimit || pitch > highLimit) limitHit = true;
            pitch = std::max(lowLimit, std::min(highLimit, pitch));

            gp[slot].pitch       = (TJBox_UInt8)pitch;
            gp[slot].velocity    = fRecordedPhrase[pSrc].velocity;
            gp[slot].onsetPPQ    = timOnset[slot];
            gp[slot].durationPPQ = timDur[slot];
        }
    }

    // -----------------------------------------------------------------------
    void ScheduleNote(TJBox_UInt8 pitch, TJBox_UInt8 baseVel,
                      double onset, double duration, int noteIndex,
                      int rateMode, float swingKnob, float driftKnob,
                      float densityKnob, float velocityKnob, float accentKnob,
                      float lengthKnob, double activeWindowPPQ) {
        if (pitch == 0 || pitch > 127) return;
        double batchPPQ = FramesToPPQ(64);
        double gridPPQ  = RateToPPQ(rateMode);

        int numSlices = 1;
        double sliceOnsets[64], sliceDurs[64];

        if (rateMode > 0 && gridPPQ > 0.0) {
            // Quantize the onset to the NEAREST grid line and lay out RAW (absolute)
            // ratchet cells. We deliberately do NOT fold gridStart into the window
            // here: running fmod() on gridStart before slicing turns a note that
            // rounds onto the loop edge (fmod(N, N) == 0) into a spurious duplicate
            // slice on the downbeat. Boundary folding/snapping happens per-slice below.
            double gridStart = std::round(onset / gridPPQ) * gridPPQ;
            double span = (duration > 0.0 ? duration : gridPPQ);
            int cellCount = (int)std::round(span / gridPPQ);
            if (cellCount < 1)  cellCount = 1;
            if (cellCount > 64) cellCount = 64;

            numSlices = 0;
            for (int c = 0; c < cellCount && numSlices < 64; ++c) {
                sliceOnsets[numSlices] = gridStart + (double)c * gridPPQ;
                sliceDurs[numSlices]   = gridPPQ;
                numSlices++;
            }
        } else {
            sliceOnsets[0] = onset; sliceDurs[0] = duration;
        }

        for (int s = 0; s < numSlices; ++s) {
            double sOn  = sliceOnsets[s];
            double sDur = sliceDurs[s];
            TJBox_UInt32 noteSeed = fCurrentSeed + (noteIndex * 1999) + (fLoopCount * 777) + (s * 333);

            if (swingKnob != 0.5f) {
                long ticks = (long)std::round(sOn);
                if      (ticks % 3840 == 1920) sOn += (swingKnob - 0.5f) * 2.0 * 960.0;
                else if (ticks % 1920 == 960)  sOn += (swingKnob - 0.5f) * 2.0 * 480.0;
            }

            long rTicks = (long)std::round(sOn);
            bool isDown   = (rTicks % 15360 == 0);
            bool isStrong = (rTicks % 3840  == 0);

            if (densityKnob < 0.5f) {
                float dp = (0.5f - densityKnob) * 2.0f;
                if (isDown) dp *= 0.1f; else if (isStrong) dp *= 0.3f;
                TJBox_UInt32 dr = MusicTheory::SeededRandom(noteSeed);
                if ((dr % 100) < (TJBox_UInt32)(dp * 100.0f)) continue;
            }

            if (driftKnob > 0.0f) {
                TJBox_UInt32 dr = MusicTheory::SeededRandom(noteSeed);
                float da = ((dr % 1000) / 1000.0f) * 1920.0f * driftKnob;
                sOn += (fLoopCount % 2 == 0) ? da : -da;
            }

            // Fold the raw (absolute) grid slice into [0, activeWindowPPQ) HERE, after
            // the slices were generated, so a cell that lands on the loop edge is never
            // pre-duplicated onto the downbeat.
            if (activeWindowPPQ > 0.0) {
                sOn = std::fmod(sOn, activeWindowPPQ);
                if (sOn < 0.0) sOn += activeWindowPPQ;
                // A slice sitting on (or within ~1/64th of a beat before) the loop
                // boundary is really the downbeat of the NEXT pass: snap it cleanly to
                // 0.0 instead of slicing a micro-blip off the tail of the phrase.
                if (activeWindowPPQ - sOn < 60.0) sOn = 0.0;
            }
            if (sOn < 0.0) continue;

            float vf = (float)baseVel + (velocityKnob - 0.5f) * 64.0f;
            if (isStrong) vf += accentKnob * 50.0f; else vf -= accentKnob * 50.0f;
            int finalVel = std::max(1, std::min(127, (int)vf));

            double finalDur = sDur;
            if (sDur > 0.0) { finalDur = sDur * (lengthKnob * 2.0); if (finalDur < 60.0) finalDur = 60.0; }
            
            // Clamp every note to the loop boundary. We deliberately do NOT wrap the
            // overflow around to position 0.0 any more: because the loop reset calls
            // KillHangingNotes(), that wrapped tail always came back out as a detached
            // little fragment at the top of the next cycle -- the "shortened chord" blip.
            // Clamping keeps each note a single, clean event inside the loop.
            if (activeWindowPPQ > 0.0 && sOn + finalDur > activeWindowPPQ) {
                finalDur = activeWindowPPQ - sOn;
            }

            // PRIMARY NOTE SEGMENT
            if (sOn >= fPlaybackPPQ && sOn < fPlaybackPPQ + batchPPQ) {
                TJBox_UInt16 fr = (TJBox_UInt16)(((sOn - fPlaybackPPQ) / batchPPQ) * 64.0);
                TJBox_NoteEvent ev; ev.fNoteNumber=pitch; ev.fVelocity=(TJBox_UInt8)finalVel; ev.fAtFrameIndex=fr;
                JBox_OutputNoteEvent(ev); fCurrentlyPlayingVoices[pitch] = 1;
                float tp = (activeWindowPPQ > 0.0) ? (float)(sOn/activeWindowPPQ) : 0.5f;
                fOutputRippleX[pitch] = std::max(0.0f,std::min(1.0f,tp)); TriggerRipple(true, pitch, fOutputRippleX[pitch]);
            }
            double offT = sOn + finalDur;
            if (offT >= fPlaybackPPQ && offT < fPlaybackPPQ + batchPPQ) {
                TJBox_UInt16 fr = (TJBox_UInt16)(((offT - fPlaybackPPQ) / batchPPQ) * 64.0);
                TJBox_NoteEvent ev; ev.fNoteNumber=pitch; ev.fVelocity=0; ev.fAtFrameIndex=fr;
                JBox_OutputNoteEvent(ev); fCurrentlyPlayingVoices[pitch] = 0;
            }

            // GHOST NOTES (Density)
            if (densityKnob > 0.5f) {
                float ap = (densityKnob - 0.5f) * 2.0f;
                TJBox_UInt32 ar = MusicTheory::SeededRandom(noteSeed);
                if ((ar % 100) < (TJBox_UInt32)(ap * 100.0f)) {
                    double originalDur = sDur * (lengthKnob * 2.0); if (originalDur < 60.0) originalDur = 60.0;
                    double gOn  = sOn + originalDur * 0.5;
                    double gDur = originalDur * 0.25;
                    
                    if (activeWindowPPQ > 0.0) {
                        gOn = std::fmod(gOn, activeWindowPPQ);
                        if (gOn < 0.0) gOn += activeWindowPPQ;
                        if (gOn + gDur > activeWindowPPQ) gDur = activeWindowPPQ - gOn; // Truncate ghost logic
                    }
                    double gOff = gOn + gDur;
                    int gVel = std::max(1, finalVel / 2);
                    
                    if (gOn >= fPlaybackPPQ && gOn < fPlaybackPPQ + batchPPQ) {
                        TJBox_UInt16 fr = (TJBox_UInt16)(((gOn-fPlaybackPPQ)/batchPPQ)*64.0);
                        TJBox_NoteEvent ev; ev.fNoteNumber=pitch; ev.fVelocity=(TJBox_UInt8)gVel; ev.fAtFrameIndex=fr;
                        JBox_OutputNoteEvent(ev); fCurrentlyPlayingVoices[pitch] = 1;
                    }
                    if (gOff >= fPlaybackPPQ && gOff < fPlaybackPPQ + batchPPQ) {
                        TJBox_UInt16 fr = (TJBox_UInt16)(((gOff-fPlaybackPPQ)/batchPPQ)*64.0);
                        TJBox_NoteEvent ev; ev.fNoteNumber=pitch; ev.fVelocity=0; ev.fAtFrameIndex=fr;
                        JBox_OutputNoteEvent(ev); fCurrentlyPlayingVoices[pitch] = 0;
                    }
                }
            }
        }
    }

    // -----------------------------------------------------------------------
    void AppendChar(char* b, int& o, char c) { if(o<2047) b[o++]=c; }
    void AppendString(char* b, int& o, const char* s) { while(*s&&o<2047) b[o++]=*s++; }
    void AppendInt(char* b, int& o, int v) {
        if(v==0){AppendChar(b,o,'0');return;} if(v<0){AppendChar(b,o,'-');v=-v;}
        char t[16]; int p=0; while(v>0){t[p++]='0'+(v%10);v/=10;} while(p>0)AppendChar(b,o,t[--p]);
    }
    void AppendFloat(char* b, int& o, float v) {
        if(v<0){AppendChar(b,o,'-');v=-v;}
        int w=(int)v; AppendInt(b,o,w); AppendChar(b,o,'.');
        int f=(int)((v-w)*100.0f); if(f<0)f=0; if(f<10)AppendChar(b,o,'0'); AppendInt(b,o,f);
    }

    void TriggerRipple(bool isOut, TJBox_UInt8 pitch, float tp) {
        RippleData* pool = isOut ? fOutputRipples : fInputRipples;
        for (int i = 0; i < 64; ++i) {
            if (!pool[i].active) {
                pool[i].active=true; pool[i].radius=1.0f; pool[i].alpha=255.0f; pool[i].x=tp;
                float np=(pitch-36.0f)/60.0f;
                if(np<0.05f)np=0.05f; if(np>0.95f)np=0.95f;
                pool[i].y=1.0f-np; pool[i].isOutput=isOut; break;
            }
        }
    }

    void UpdateAndSerializeVisuals() {
        fWavePhase += 0.005f;
        if (fWavePhase > 6.28318f) fWavePhase -= 6.28318f;
        fRippleTimer++;
        if (fRippleTimer >= 15) {
            fRippleTimer = 0;
            for (int i = 0; i < 128; ++i) {
                if (fActiveNoteBufferIndex[i] >= 0) TriggerRipple(false, i, fInputRippleX[i]);
                if (fCurrentlyPlayingVoices[i] > 0)  TriggerRipple(true,  i, fOutputRippleX[i]);
            }
        }
        char buf[2048]; int off = 0;
        AppendString(buf,off,"W:"); AppendFloat(buf,off,fWavePhase); AppendChar(buf,off,'|');
        for (int i = 0; i < 64; ++i) {
            if (!fInputRipples[i].active) continue;
            fInputRipples[i].radius+=0.9f; fInputRipples[i].alpha-=3.5f;
            if (fInputRipples[i].alpha<=0.0f){fInputRipples[i].active=false;continue;}
            AppendString(buf,off,"I:"); AppendFloat(buf,off,fInputRipples[i].x); AppendChar(buf,off,',');
            AppendFloat(buf,off,fInputRipples[i].y); AppendChar(buf,off,',');
            AppendFloat(buf,off,fInputRipples[i].radius); AppendChar(buf,off,',');
            AppendInt(buf,off,(int)fInputRipples[i].alpha); AppendChar(buf,off,'|');
        }
        for (int i = 0; i < 64; ++i) {
            if (!fOutputRipples[i].active) continue;
            fOutputRipples[i].radius+=0.9f; fOutputRipples[i].alpha-=3.5f;
            if (fOutputRipples[i].alpha<=0.0f){fOutputRipples[i].active=false;continue;}
            AppendString(buf,off,"O:"); AppendFloat(buf,off,fOutputRipples[i].x); AppendChar(buf,off,',');
            AppendFloat(buf,off,fOutputRipples[i].y); AppendChar(buf,off,',');
            AppendFloat(buf,off,fOutputRipples[i].radius); AppendChar(buf,off,',');
            AppendInt(buf,off,(int)fOutputRipples[i].alpha); AppendChar(buf,off,'|');
        }
        buf[off]='\0';
        JBox_SetRTStringData(fDisplayDataRef, off, reinterpret_cast<const TJBox_UInt8*>(buf));
    }

    // -----------------------------------------------------------------------
    void HandleLoopEnd(double activeWindowPPQ, bool feedbackOn, float mutationKnob,
                       int scaleMode, int responseMode, int octaveShift, int transposeShift,
                       int harmonyShift, float memoryKnob, double lengthMultiplier,
                       int lowLimit, int highLimit, bool bounceMode, int maxLoops) {
        fLoopCount++;

        if (feedbackOn) {
            fRecordedNoteCount = fGeneratedNoteCount;
            for (int j = 0; j < fGeneratedNoteCount; j++) {
                fRecordedPhrase[j].pitch       = fGeneratedPhrase[j].pitch;
                fRecordedPhrase[j].velocity    = fGeneratedPhrase[j].velocity;
                fRecordedPhrase[j].onsetPPQ    = fGeneratedPhrase[j].onsetPPQ;
                fRecordedPhrase[j].durationPPQ = fGeneratedPhrase[j].durationPPQ;
                fMutationOffsets[j] = 0;
            }
            fPhraseLengthPPQ = activeWindowPPQ;
        }

        if (mutationKnob > 0.0f) {
            for (int j = 0; j < fRecordedNoteCount; j++) {
                TJBox_UInt32 ms = fCurrentSeed + (j*444) + (fLoopCount*888);
                if ((MusicTheory::SeededRandom(ms) % 1000) < (TJBox_UInt32)(mutationKnob * 1000.0f)) {
                    TJBox_UInt32 jr = MusicTheory::SeededRandom(ms) % 100;
                    int jump = 0;
                    if(jr<40) jump=(jr%2==0)?1:-1; else if(jr<70) jump=(jr%2==0)?2:-2;
                    else if(jr<90) jump=(jr%2==0)?3:-3; else jump=(jr%2==0)?4:-4;
                    fMutationOffsets[j] += jump;
                }
            }
        }

        if (fLimitHit) {
            if (bounceMode) {
                fPitchDirectionMult *= -1;
            } else {
                fPitchDirectionMult = 1;
                for (int j = 0; j < fRecordedNoteCount; j++) fMutationOffsets[j] = 0;
            }
        }

        double newWindow = fPhraseLengthPPQ * lengthMultiplier;
        ComputeGeneratedPhrase(scaleMode, responseMode, octaveShift, transposeShift,
                               harmonyShift, memoryKnob, lengthMultiplier, newWindow,
                               lowLimit, highLimit, fGeneratedPhrase, fGeneratedNoteCount, fLimitHit);
        int alterEgoMode = (int)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 31));
        bool aeLimit = false;
        ComputeGeneratedPhrase(scaleMode, alterEgoMode, octaveShift, transposeShift,
                               harmonyShift, memoryKnob, lengthMultiplier, newWindow,
                               lowLimit, highLimit, fAlterEgoPhrase, fAlterEgoNoteCount, aeLimit);

        if (maxLoops != -1 && fLoopCount > maxLoops) {
            KillHangingNotes(63);
            fState = STATE_IDLE;
        }
    }

    // -----------------------------------------------------------------------
    void RenderBatch(const TJBox_PropertyDiff iPropertyDiffs[], TJBox_UInt32 iDiffCount) {
        if (!fRefsInitialized) {
            fNoteStatesRef       = JBox_GetMotherboardObjectRef("/note_states");
            fCustomPropertiesRef = JBox_GetMotherboardObjectRef("/custom_properties");
            fEnvironmentRef      = JBox_GetMotherboardObjectRef("/environment");
            fTransportRef        = JBox_GetMotherboardObjectRef("/transport");
            fNoteOnIndicatorRef  = JBox_MakePropertyRef(fCustomPropertiesRef, "NoteOnIndicator");
            fDisplayDataRef      = JBox_MakePropertyRef(fCustomPropertiesRef, "ReflectionDisplayData");
            fCVRateRef        = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_Rate"),      "value");
            fCVHarmonyRef     = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_Harmony"),   "value");
            fCVOctaveRef      = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_Octave"),    "value");
            fCVTransposeRef   = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_Transpose"), "value");
            fCVMemoryRef      = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_Memory"),    "value");
            fCVMutationRef    = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_Mutation"),  "value");
            fCVLowLimitRef    = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_LowLimit"),  "value");
            fCVHighLimitRef   = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_HighLimit"), "value");
            fCVDirectionRef   = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_Direction"), "value");
            fCVFreezeRef      = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_Freeze"),    "value");
            fCVSeedRef        = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_Seed"),      "value");
            fCVClearRef       = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_Clear"),     "value");
            fCVLoopRef        = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_Loop"),      "value");
            fCVConnFreezeRef  = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_Freeze"),    "connected");
            fCVConnDirRef     = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_inputs/CV_Direction"), "connected");
            fAlterEgoNoteRef  = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_outputs/CV_Out_AlterEgo_Note"), "value");
            fAlterEgoGateRef  = JBox_MakePropertyRef(JBox_GetMotherboardObjectRef("/cv_outputs/CV_Out_AlterEgo_Gate"), "value");
            fRefsInitialized = true;
        }

        bool lampChanged = false;

        for (TJBox_UInt32 i = 0; i < iDiffCount; ++i) {
            const TJBox_PropertyDiff& diff = iPropertyDiffs[i];
            if (diff.fPropertyRef.fObject == fCustomPropertiesRef) {
                switch (diff.fPropertyTag) {
                    case 1:  fIsOn = JBox_GetBoolean(diff.fCurrentValue); break;
                    case 11: { // Freeze
                        bool nf = JBox_GetBoolean(diff.fCurrentValue);
                        if (nf && !fFreezeOn) {
                            fShadowPlaybackPPQ = fPlaybackPPQ;
                        } else if (!nf && fFreezeOn) {
                            fFreezeJustReleased = true;
                        }
                        fFreezeOn = nf;
                        break;
                    }
                    case 62: // Seed
                        if (JBox_GetBoolean(diff.fCurrentValue))
                            fCurrentSeed = fCurrentSeed * 1664525 + 1013904223;
                        break;
                    case 75: // Clear
                        if (JBox_GetBoolean(diff.fCurrentValue)) fClearPending = true;
                        break;
                }
            } else if (diff.fPropertyRef.fObject == fEnvironmentRef && diff.fPropertyTag == kJBox_EnvironmentPlayerBypassed) {
                fIsBypassed = JBox_GetBoolean(diff.fCurrentValue);
            } else if (diff.fPropertyRef.fObject == fTransportRef && diff.fPropertyTag == kJBox_TransportTempo) {
                fTempo = JBox_GetNumber(diff.fCurrentValue);
            }
        }

        if (fIsBypassed) {
            if (fActiveNotes > 0) {
                fActiveNotes = 0;
                JBox_StoreMOMProperty(fNoteOnIndicatorRef, JBox_MakeBoolean(false));
            }
            KillHangingNotes(0); return;
        }

        // --- CLEAR ---
        if (fClearPending) {
            KillHangingNotes(0);
            fState = STATE_IDLE; fRecordedNoteCount = 0; fGeneratedNoteCount = 0;
            fPhraseLengthPPQ = 0.0; fPlaybackPPQ = 0.0; fShadowPlaybackPPQ = 0.0;
            fLoopCount = 0; fFreezeOn = false; fFreezeJustReleased = false;
            fLimitHit = false; fPitchDirectionMult = 1;
            for (int i=0;i<12;i++) fPitchClassHistogram[i]=0;
            for (int i=0;i<512;i++) fMutationOffsets[i]=0;
            fClearPending = false;
            if (fActiveNotes > 0) { fActiveNotes=0; lampChanged=true; }
            for (int i = 0; i < 128; i++) { fDuetPrimary[i] = -1; fDuetSecondary[i] = -1; }
            fDuetQueueSize = 0; fDuetClockPPQ = 0.0;
        }

        double batchPPQ = FramesToPPQ(64);

        // --- READ ALL PARAMS ---
        bool isAnswerMode = JBox_GetBoolean(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 61));
        bool isSync       = (JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 69)) > 0.5);
        bool keepInput    = JBox_GetBoolean(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 73));
        bool feedbackOn   = JBox_GetBoolean(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 10));
        if (feedbackOn) keepInput = false;

        double silenceFree  = JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 63));
        double silenceSync  = JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 71));
        double patienceFree = JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 64));
        double patienceSync = JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 72));

        int scaleRootKnob  = (int)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 21));
        int scaleMode      = (int)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 22));
        int responseMode   = (int)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 60));
        int octaveShift    = (int)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 51)) - 3;
        int transposeShift = (int)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 74)) - 16;
        int harmonyShift   = (int)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 68)) - 7;
        int loopSetting    = (int)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 70));
        int lengthMode     = (int)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 66));
        int rateMode       = (int)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 40));
        int lowLimit       = (int)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 76));
        int highLimit      = (int)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 77));
        bool bounceMode    = !JBox_GetBoolean(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 78));

        float densityKnob  = (float)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 41));
        float lengthKnob   = (float)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 42));
        float swingKnob    = (float)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 43));
        float velocityKnob = (float)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 44));
        float accentKnob   = (float)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 45));
        float mutationKnob = (float)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 50));
        float driftKnob    = (float)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 67));
        float memoryKnob   = (float)JBox_GetNumber(JBox_LoadMOMPropertyByTag(fCustomPropertiesRef, 65));

        // --- CV INPUTS ---
        float cvRate      = (float)JBox_GetNumber(JBox_LoadMOMProperty(fCVRateRef));
        float cvHarmony   = (float)JBox_GetNumber(JBox_LoadMOMProperty(fCVHarmonyRef));
        float cvOctave    = (float)JBox_GetNumber(JBox_LoadMOMProperty(fCVOctaveRef));
        float cvTranspose = (float)JBox_GetNumber(JBox_LoadMOMProperty(fCVTransposeRef));
        float cvMemory    = (float)JBox_GetNumber(JBox_LoadMOMProperty(fCVMemoryRef));
        float cvMutation  = (float)JBox_GetNumber(JBox_LoadMOMProperty(fCVMutationRef));
        float cvLowLim    = (float)JBox_GetNumber(JBox_LoadMOMProperty(fCVLowLimitRef));
        float cvHighLim   = (float)JBox_GetNumber(JBox_LoadMOMProperty(fCVHighLimitRef));
        float cvDirection = (float)JBox_GetNumber(JBox_LoadMOMProperty(fCVDirectionRef));
        float cvFreeze    = (float)JBox_GetNumber(JBox_LoadMOMProperty(fCVFreezeRef));
        float cvSeed      = (float)JBox_GetNumber(JBox_LoadMOMProperty(fCVSeedRef));
        float cvClear     = (float)JBox_GetNumber(JBox_LoadMOMProperty(fCVClearRef));
        float cvLoop      = (float)JBox_GetNumber(JBox_LoadMOMProperty(fCVLoopRef));

        rateMode      = rateMode + (int)std::round(cvRate * 10.0f);
        if (rateMode < 0) rateMode = 0; if (rateMode > 19) rateMode = 19;
        harmonyShift  = harmonyShift + (int)std::round(cvHarmony * 7.0f);
        if (harmonyShift < -7) harmonyShift = -7; if (harmonyShift > 7) harmonyShift = 7;
        octaveShift   = octaveShift + (int)std::round(cvOctave * 3.0f);
        if (octaveShift < -3) octaveShift = -3; if (octaveShift > 3) octaveShift = 3;
        transposeShift = transposeShift + (int)std::round(cvTranspose * 16.0f);
        if (transposeShift < -16) transposeShift = -16; if (transposeShift > 16) transposeShift = 16;
        memoryKnob  = memoryKnob + cvMemory;
        if (memoryKnob < 0.0f) memoryKnob = 0.0f; if (memoryKnob > 1.0f) memoryKnob = 1.0f;
        mutationKnob = mutationKnob + cvMutation;
        if (mutationKnob < 0.0f) mutationKnob = 0.0f; if (mutationKnob > 1.0f) mutationKnob = 1.0f;
        lowLimit  = lowLimit  + (int)std::round(cvLowLim  * 64.0f);
        if (lowLimit  < 0) lowLimit  = 0; if (lowLimit  > 127) lowLimit  = 127;
        highLimit = highLimit + (int)std::round(cvHighLim * 64.0f);
        if (highLimit < 0) highLimit = 0; if (highLimit > 127) highLimit = 127;
        if (cvLoop > 0.01f || cvLoop < -0.01f) {
            loopSetting = loopSetting + (int)std::round(cvLoop * 4.0f);
            if (loopSetting < 0) loopSetting = 0; if (loopSetting > 7) loopSetting = 7;
        }
        
        bool freezeCVConn = JBox_GetBoolean(JBox_LoadMOMProperty(fCVConnFreezeRef));
        bool dirCVConn    = JBox_GetBoolean(JBox_LoadMOMProperty(fCVConnDirRef));
        if (dirCVConn)    bounceMode = (cvDirection < 0.5f);
        if (freezeCVConn) {
            bool wantFreeze = (cvFreeze > 0.5f);
            if (wantFreeze && !fFreezeOn)  { fShadowPlaybackPPQ = fPlaybackPPQ; }
            else if (!wantFreeze && fFreezeOn) { fFreezeJustReleased = true; }
            fFreezeOn = wantFreeze;
        }
        if (cvSeed  > 0.5f && fPrevSeedCV  <= 0.5f) fCurrentSeed = fCurrentSeed * 1664525u + 1013904223u;
        if (cvClear > 0.5f && fPrevClearCV <= 0.5f) fClearPending = true;
        fPrevSeedCV  = cvSeed;
        fPrevClearCV = cvClear;

        double lengthMultiplier = 1.0;
        switch(lengthMode) {
            case 0: lengthMultiplier=0.25; break; case 1: lengthMultiplier=0.5;  break;
            case 2: lengthMultiplier=1.0;  break; case 3: lengthMultiplier=1.5;  break;
            case 4: lengthMultiplier=2.0;  break; case 5: lengthMultiplier=2.5;  break;
            case 6: lengthMultiplier=3.0;  break; case 7: lengthMultiplier=4.0;  break;
        }

        int maxLoops = 1;
        switch(loopSetting) {
            case 0: maxLoops=1;  break; case 1: maxLoops=2;  break; case 2: maxLoops=3;  break;
            case 3: maxLoops=4;  break; case 4: maxLoops=8;  break; case 5: maxLoops=16; break;
            case 6: maxLoops=32; break; case 7: maxLoops=-1; break;
        }

        double activeWindowPPQ = 0.0;

        // -----------------------------------------------------------------------
        // MIDI RECORDING & DUET QUEUEING
        // -----------------------------------------------------------------------
        for (TJBox_UInt32 i = 0; i < iDiffCount; ++i) {
            const TJBox_PropertyDiff& diff = iPropertyDiffs[i];
            if (diff.fPropertyRef.fObject != fNoteStatesRef) continue;

            const TJBox_NoteEvent& ev = JBox_AsNoteEvent(diff);
            TJBox_UInt8 pitch = ev.fNoteNumber;
            double notePPQ = FramesToPPQ(ev.fAtFrameIndex);

            if (ev.fVelocity > 0) {
                fActiveNotes++; lampChanged = true; fFramesSinceAllNotesOff = 0;
                fPitchClassHistogram[pitch % 12]++;

                bool wasPlaying = (fState == STATE_PLAYING || fState == STATE_PATIENCE);

                if (wasPlaying && isAnswerMode) {
                    KillHangingNotes(ev.fAtFrameIndex);
                    fState = STATE_LISTENING; fRecordedNoteCount = 0; fPhraseLengthPPQ = 0.0;
                    fFreezeOn = false; fFreezeJustReleased = false;
                    for (int h=0;h<12;h++) fPitchClassHistogram[h]=0;
                    for (int k=0;k<128;k++) fActiveNoteBufferIndex[k]=-1;
                } else if (fState == STATE_IDLE) {
                    fState = STATE_LISTENING; fRecordedNoteCount = 0; fPhraseLengthPPQ = 0.0;
                    for (int h=0;h<12;h++) fPitchClassHistogram[h]=0;
                    for (int k=0;k<128;k++) fActiveNoteBufferIndex[k]=-1;
                } else if (fState == STATE_EVALUATING_SILENCE) {
                    fState = STATE_LISTENING;
                }

                if (fState == STATE_LISTENING && fRecordedNoteCount < 512) {
                    int idx = fRecordedNoteCount;
                    fRecordedPhrase[idx].pitch       = pitch;
                    fRecordedPhrase[idx].velocity    = ev.fVelocity;
                    fRecordedPhrase[idx].onsetPPQ    = fPhraseLengthPPQ + notePPQ;
                    fRecordedPhrase[idx].durationPPQ = 0.0;
                    fMutationOffsets[idx] = 0;
                    fActiveNoteBufferIndex[pitch] = idx;
                    fRecordedNoteCount++;

                    float tp = (fPhraseLengthPPQ > 0.0) ?
                        std::max(0.0f, std::min(1.0f, (float)((fPhraseLengthPPQ + notePPQ) / 15360.0f))) : 0.5f;
                    fInputRippleX[pitch] = tp; TriggerRipple(false, pitch, tp);
                }

                if (isAnswerMode) {
                    if (fState == STATE_IDLE || fState == STATE_LISTENING || fState == STATE_EVALUATING_SILENCE)
                        JBox_OutputNoteEvent(ev);
                } else {
                    // DUET MODE: real-time, parallel pitch processor & harmonizer.
                    //   PRIMARY   voice = the live note shifted by Octave + Transpose.
                    //   SECONDARY voice = PRIMARY + Harmony (only when Harmony is engaged).
                    // Both transformed voices are queued so they sound in parallel with
                    // the player; the raw dry note is emitted ONLY when Keep Input is ON.
                    // Duet is purely 1:1 -- it does NOT build a generative loop, so resolve
                    // the scale root here (Answer mode only does it at loop start, which
                    // Duet no longer reaches) to keep the transforms in key.
                    ResolveActiveRoot(scaleRootKnob, scaleMode);
                    int origDeg = MusicTheory::GetScaleDegree((int)pitch, fActiveRoot, scaleMode);
                    int baseDeg = origDeg + transposeShift;
                    if (responseMode == 1) baseDeg = -baseDeg;
                    int baseOct = (int)pitch / 12 + octaveShift * fPitchDirectionMult;

                    int primary = MusicTheory::GetPitchFromDegree(baseDeg, baseOct, fActiveRoot, scaleMode);
                    primary = std::max(lowLimit, std::min(highLimit, std::max(1, primary)));

                    int secondary = -1;
                    if (harmonyShift != 0) {
                        int harmDeg = baseDeg + harmonyShift * fPitchDirectionMult;
                        secondary = MusicTheory::GetPitchFromDegree(harmDeg, baseOct, fActiveRoot, scaleMode);
                        secondary = std::max(lowLimit, std::min(highLimit, std::max(1, secondary)));
                    }

                    double delay = GetPatiencePPQ(isSync, patienceFree, patienceSync);
                    double targetPPQ;
                    if (delay <= 0.0) {
                        // Patience == 0 (or Bypass): real-time. Fire in the very next
                        // batch with ZERO added latency -- bypass grid quantization AND
                        // swing so the processed voices land in parallel with the player.
                        targetPPQ = fDuetClockPPQ;
                    } else {
                        // Patience > 0: time-delayed echo. Quantize/swing the delayed
                        // hit, but never let it round earlier than 'now'.
                        targetPPQ = fDuetClockPPQ + delay;
                        double gridPPQ = RateToPPQ(rateMode);
                        if (gridPPQ > 0.0) {
                            targetPPQ = std::round(targetPPQ / gridPPQ) * gridPPQ;
                        }
                        if (swingKnob != 0.5f) {
                            long ticks = (long)std::round(targetPPQ);
                            if      (ticks % 3840 == 1920) targetPPQ += (swingKnob - 0.5f) * 2.0 * 960.0;
                            else if (ticks % 1920 == 960)  targetPPQ += (swingKnob - 0.5f) * 2.0 * 480.0;
                        }
                        if (targetPPQ <= fDuetClockPPQ) targetPPQ = fDuetClockPPQ;
                    }

                    float vf = (float)ev.fVelocity + (velocityKnob - 0.5f) * 64.0f;
                    long rTicks = (long)std::round(targetPPQ);
                    bool isStrong = (rTicks % 3840  == 0);
                    if (isStrong) vf += accentKnob * 50.0f; else vf -= accentKnob * 50.0f;
                    int finalVel = std::max(1, std::min(127, (int)vf));

                    bool drop = false;
                    if (densityKnob < 0.5f) {
                        float dp = (0.5f - densityKnob) * 2.0f;
                        bool isDown = (rTicks % 15360 == 0);
                        if (isDown) dp *= 0.1f; else if (isStrong) dp *= 0.3f;
                        TJBox_UInt32 liveSeed = fCurrentSeed + (TJBox_UInt32)pitch;
                        if ((MusicTheory::SeededRandom(liveSeed) % 100) < (TJBox_UInt32)(dp * 100.0f)) drop = true;
                    }

                    // Raw dry input is only passed through when Keep Input is ON.
                    if (keepInput) JBox_OutputNoteEvent(ev);

                    if (!drop) {
                        if (fDuetQueueSize < kDuetQueueMax)
                            fDuetQueue[fDuetQueueSize++] = { (TJBox_UInt8)primary, (TJBox_UInt8)finalVel, targetPPQ };
                        fDuetPrimary[(int)pitch] = primary;

                        if (secondary >= 0) {
                            if (fDuetQueueSize < kDuetQueueMax)
                                fDuetQueue[fDuetQueueSize++] = { (TJBox_UInt8)secondary, (TJBox_UInt8)finalVel, targetPPQ };
                            fDuetSecondary[(int)pitch] = secondary;
                        } else {
                            fDuetSecondary[(int)pitch] = -1;
                        }
                    } else {
                        fDuetPrimary[(int)pitch] = -1;
                        fDuetSecondary[(int)pitch] = -1;
                    }
                }

            } else if (ev.fVelocity == 0 && fActiveNotes > 0) {
                fActiveNotes--; lampChanged = true;
                int bufIdx = fActiveNoteBufferIndex[pitch];
                if (bufIdx >= 0 && bufIdx < fRecordedNoteCount) {
                    double offT = fPhraseLengthPPQ + notePPQ;
                    fRecordedPhrase[bufIdx].durationPPQ = std::max(0.0, offT - fRecordedPhrase[bufIdx].onsetPPQ);
                    fActiveNoteBufferIndex[pitch] = -1;
                }
                
                if (isAnswerMode) {
                    if (fState == STATE_IDLE || fState == STATE_LISTENING || fState == STATE_EVALUATING_SILENCE)
                        JBox_OutputNoteEvent(ev);
                } else {
                    // Release the dry pass-through note only if Keep Input is ON.
                    if (keepInput) JBox_OutputNoteEvent(ev);

                    double delay = GetPatiencePPQ(isSync, patienceFree, patienceSync);
                    double targetPPQ;
                    if (delay <= 0.0) {
                        // Real-time: release the processed voices the instant the live
                        // note is released (no grid/swing latency).
                        targetPPQ = fDuetClockPPQ;
                    } else {
                        targetPPQ = fDuetClockPPQ + delay;
                        double gridPPQ = RateToPPQ(rateMode);
                        if (gridPPQ > 0.0) {
                            targetPPQ = std::round(targetPPQ / gridPPQ) * gridPPQ;
                        }
                        if (swingKnob != 0.5f) {
                            long ticks = (long)std::round(targetPPQ);
                            if      (ticks % 3840 == 1920) targetPPQ += (swingKnob - 0.5f) * 2.0 * 960.0;
                            else if (ticks % 1920 == 960)  targetPPQ += (swingKnob - 0.5f) * 2.0 * 480.0;
                        }
                        if (targetPPQ <= fDuetClockPPQ) targetPPQ = fDuetClockPPQ;
                    }

                    // Release BOTH transformed voices that this input note spawned.
                    int primOff = fDuetPrimary[(int)pitch];
                    if (primOff >= 0 && fDuetQueueSize < kDuetQueueMax)
                        fDuetQueue[fDuetQueueSize++] = { (TJBox_UInt8)primOff, 0, targetPPQ };
                    int secOff = fDuetSecondary[(int)pitch];
                    if (secOff >= 0 && fDuetQueueSize < kDuetQueueMax)
                        fDuetQueue[fDuetQueueSize++] = { (TJBox_UInt8)secOff, 0, targetPPQ };

                    fDuetPrimary[(int)pitch] = -1;
                    fDuetSecondary[(int)pitch] = -1;
                }
            }
        }

        if (lampChanged) JBox_StoreMOMProperty(fNoteOnIndicatorRef, JBox_MakeBoolean(fActiveNotes > 0));

        // Drain Duet Live Echo Queue accurately
        if (!isAnswerMode) {
            fDuetClockPPQ += batchPPQ;
            int keep = 0;
            for (int i = 0; i < fDuetQueueSize; i++) {
                if (fDuetQueue[i].firePPQ <= fDuetClockPPQ) {
                    TJBox_NoteEvent nev;
                    nev.fNoteNumber = fDuetQueue[i].pitch;
                    nev.fVelocity   = fDuetQueue[i].velocity;
                    
                    double diff = fDuetClockPPQ - fDuetQueue[i].firePPQ;
                    double frameOffset = 64.0 - (diff / batchPPQ) * 64.0;
                    nev.fAtFrameIndex = (TJBox_UInt16)std::max(0.0, std::min(63.0, frameOffset));
                    
                    JBox_OutputNoteEvent(nev);
                    
                    if (nev.fVelocity > 0) fCurrentlyPlayingVoices[nev.fNoteNumber] = 1;
                    else fCurrentlyPlayingVoices[nev.fNoteNumber] = 0;
                    
                    if (nev.fVelocity > 0) {
                        float tp = 0.5f;
                        fOutputRippleX[nev.fNoteNumber] = tp;
                        TriggerRipple(true, nev.fNoteNumber, tp);
                    }
                } else {
                    fDuetQueue[keep++] = fDuetQueue[i];
                }
            }
            fDuetQueueSize = keep;
        }

        // -----------------------------------------------------------------------
        // STATE MACHINE
        // -----------------------------------------------------------------------

        if (fState == STATE_LISTENING || fState == STATE_EVALUATING_SILENCE) {
            fPhraseLengthPPQ += batchPPQ;
        }

        if (fState == STATE_LISTENING && fActiveNotes == 0) {
            fFramesSinceAllNotesOff += 64;
            if (fFramesSinceAllNotesOff >= 64) {
                if (isAnswerMode) {
                    fState = STATE_EVALUATING_SILENCE;
                    fFramesSinceSilence = 0;
                    fFramesSinceAllNotesOff = 0;
                } else {
                    // DUET MODE is a pure live 1:1 processor -- it does NOT build a
                    // generative loop. When the player goes silent, just reset to IDLE so
                    // the next phrase starts fresh; the live response keeps running
                    // independently through fDuetQueue.
                    fState = STATE_IDLE;
                    fFramesSinceAllNotesOff = 0;
                }
            }
        } else if (fState == STATE_LISTENING) {
            fFramesSinceAllNotesOff = 0;
        }

        if (fState == STATE_EVALUATING_SILENCE) {
            if (!isAnswerMode) {
                // DUET MODE never loops and never waits on the Silence Threshold. If we
                // somehow land here (e.g. the mode was switched mid-evaluation) just
                // reset to IDLE; the live response keeps running through fDuetQueue.
                fState = STATE_IDLE;
                fFramesSinceSilence = 0;
            } else {
                fFramesSinceSilence += 64;
                double required = GetSilenceFrames(isSync, silenceFree, silenceSync);
                if (fFramesSinceSilence >= required) {
                    fPhraseLengthPPQ = SnapToNearestBeat(fPhraseLengthPPQ);
                    fState = STATE_PATIENCE;
                    fPatiencePPQRemaining = GetPatiencePPQ(isSync, patienceFree, patienceSync);
                }
            }
        }

        if (fState == STATE_PATIENCE) {
            if (fPatiencePPQRemaining <= batchPPQ) {
                ResolveActiveRoot(scaleRootKnob, scaleMode);
                {
                    double notesEnd = 0.0;
                    for (int j = 0; j < fRecordedNoteCount; j++) {
                        double end = fRecordedPhrase[j].onsetPPQ + std::max(fRecordedPhrase[j].durationPPQ, 1.0);
                        if (end > notesEnd) notesEnd = end;
                    }
                    if (notesEnd <= 0.0) notesEnd = fPhraseLengthPPQ;
                    const double kBeat = 3840.0;
                    fPhraseLengthPPQ = std::ceil(notesEnd / kBeat) * kBeat;
                    if (fPhraseLengthPPQ <= 0.0) fPhraseLengthPPQ = kBeat;
                }
                activeWindowPPQ  = fPhraseLengthPPQ * lengthMultiplier;
                fPlaybackPPQ = 0.0; fShadowPlaybackPPQ = 0.0; fLoopCount = 1;
                ComputeGeneratedPhrase(scaleMode, responseMode, octaveShift, transposeShift,
                                       harmonyShift, memoryKnob, lengthMultiplier, activeWindowPPQ,
                                       lowLimit, highLimit, fGeneratedPhrase, fGeneratedNoteCount, fLimitHit);
                fState = STATE_PLAYING;
            } else {
                fPatiencePPQRemaining -= batchPPQ;
            }
        }

        // -----------------------------------------------------------------------
        // GENERATIVE PLAYBACK (ANSWER MODE ONLY -- Duet is purely live 1:1)
        // -----------------------------------------------------------------------
        // If we are in Duet but still in PLAYING (the mode was switched mid-loop),
        // tear the loop down and return to live-only operation.
        if (fState == STATE_PLAYING && !isAnswerMode) {
            KillHangingNotes(0);
            fState = STATE_IDLE;
            fPlaybackPPQ = 0.0; fShadowPlaybackPPQ = 0.0;
        }

        if (fState == STATE_PLAYING) {
            activeWindowPPQ = fPhraseLengthPPQ * lengthMultiplier;

            if (fFreezeOn) {
                fShadowPlaybackPPQ += batchPPQ;
                if (fShadowPlaybackPPQ >= activeWindowPPQ) {
                    fShadowPlaybackPPQ = 0.0;
                    HandleLoopEnd(activeWindowPPQ, feedbackOn, mutationKnob,
                                  scaleMode, responseMode, octaveShift, transposeShift,
                                  harmonyShift, memoryKnob, lengthMultiplier,
                                  lowLimit, highLimit, bounceMode, maxLoops);
                }
            } else {
                if (fFreezeJustReleased) {
                    KillHangingNotes(0);
                    fPlaybackPPQ = fmod(fShadowPlaybackPPQ, activeWindowPPQ > 0.0 ? activeWindowPPQ : 1.0);
                    fFreezeJustReleased = false;
                }

                bool playRaw = keepInput && (fLoopCount % 2 == 0);

                if (playRaw && isAnswerMode) {
                    for (int j = 0; j < fRecordedNoteCount; j++) {
                        TJBox_UInt8 p  = fRecordedPhrase[j].pitch;
                        double onset   = fRecordedPhrase[j].onsetPPQ;
                        double dur     = fRecordedPhrase[j].durationPPQ;
                        if (dur <= 0.0) dur = 240.0;

                        if (activeWindowPPQ > 0.0) {
                            onset = std::fmod(onset, activeWindowPPQ);
                            if (onset < 0.0) onset += activeWindowPPQ;
                            if (onset + dur > activeWindowPPQ) dur = activeWindowPPQ - onset;
                        }

                        if (onset >= fPlaybackPPQ && onset < fPlaybackPPQ + batchPPQ) {
                            TJBox_UInt16 fr = (TJBox_UInt16)(((onset-fPlaybackPPQ)/batchPPQ)*64.0);
                            TJBox_NoteEvent ev; ev.fNoteNumber=p; ev.fVelocity=fRecordedPhrase[j].velocity; ev.fAtFrameIndex=fr;
                            JBox_OutputNoteEvent(ev); fCurrentlyPlayingVoices[p]=1;
                            float tp=(activeWindowPPQ>0.0)?(float)(onset/activeWindowPPQ):0.5f;
                            fInputRippleX[p]=std::max(0.0f,std::min(1.0f,tp)); TriggerRipple(false,p,fInputRippleX[p]);
                        }
                        double offT = onset + dur;
                        if (offT >= fPlaybackPPQ && offT < fPlaybackPPQ + batchPPQ) {
                            TJBox_UInt16 fr = (TJBox_UInt16)(((offT-fPlaybackPPQ)/batchPPQ)*64.0);
                            TJBox_NoteEvent ev; ev.fNoteNumber=p; ev.fVelocity=0; ev.fAtFrameIndex=fr;
                            JBox_OutputNoteEvent(ev); fCurrentlyPlayingVoices[p]=0;
                        }
                    }
                } else {
                    for (int j = 0; j < fGeneratedNoteCount; j++) {
                        ScheduleNote(fGeneratedPhrase[j].pitch, fGeneratedPhrase[j].velocity,
                                     fGeneratedPhrase[j].onsetPPQ, fGeneratedPhrase[j].durationPPQ,
                                     j, rateMode, swingKnob, driftKnob,
                                     densityKnob, velocityKnob, accentKnob, lengthKnob, activeWindowPPQ);
                    }
                }

                fPlaybackPPQ     += batchPPQ;
                fShadowPlaybackPPQ = fPlaybackPPQ;

                if (fPlaybackPPQ >= activeWindowPPQ) {
                    KillHangingNotes(0);
                    fPlaybackPPQ = 0.0; fShadowPlaybackPPQ = 0.0;
                    HandleLoopEnd(activeWindowPPQ, feedbackOn, mutationKnob,
                                  scaleMode, responseMode, octaveShift, transposeShift,
                                  harmonyShift, memoryKnob, lengthMultiplier,
                                  lowLimit, highLimit, bounceMode, maxLoops);
                }
            }
        }

        // --- ALTER EGO CV OUTPUT ---
        if (fState == 3 && fAlterEgoNoteCount > 0 && !fFreezeOn) {
            double batchEnd = fPlaybackPPQ + FramesToPPQ(64);
            for (int ni = 0; ni < fAlterEgoNoteCount; ni++) {
                double onset = fAlterEgoPhrase[ni].onsetPPQ;
                double offT  = onset + fAlterEgoPhrase[ni].durationPPQ;
                TJBox_UInt8 p = fAlterEgoPhrase[ni].pitch;
                if (onset >= fPlaybackPPQ && onset < batchEnd) {
                    if (!fAlterEgoActive[p]) { fAlterEgoActive[p] = true; fAlterEgoActiveCount++; }
                    JBox_StoreMOMProperty(fAlterEgoNoteRef, JBox_MakeNumber((double)p / 127.0));
                    JBox_StoreMOMProperty(fAlterEgoGateRef, JBox_MakeNumber(1.0));
                }
                if (offT >= fPlaybackPPQ && offT < batchEnd) {
                    if (fAlterEgoActive[p]) { fAlterEgoActive[p] = false; fAlterEgoActiveCount--; }
                    if (fAlterEgoActiveCount <= 0) {
                        fAlterEgoActiveCount = 0;
                        JBox_StoreMOMProperty(fAlterEgoGateRef, JBox_MakeNumber(0.0));
                    }
                }
            }
        }

        UpdateAndSerializeVisuals();
    }

private:
    TJBox_ObjectRef  fNoteStatesRef, fCustomPropertiesRef, fEnvironmentRef, fTransportRef;

    TJBox_PropertyRef fNoteOnIndicatorRef, fDisplayDataRef;
    TJBox_PropertyRef fCVRateRef, fCVHarmonyRef, fCVOctaveRef, fCVTransposeRef;
    TJBox_PropertyRef fCVMemoryRef, fCVMutationRef, fCVLowLimitRef, fCVHighLimitRef;
    TJBox_PropertyRef fCVDirectionRef, fCVFreezeRef, fCVSeedRef, fCVClearRef, fCVLoopRef;
    TJBox_PropertyRef fCVConnFreezeRef, fCVConnDirRef;
    TJBox_PropertyRef fAlterEgoNoteRef, fAlterEgoGateRef;

    TJBox_Float64 fSampleRate, fTempo;
    bool fIsOn, fIsBypassed, fRefsInitialized;

    int fState, fActiveNotes;

    RecordedNote  fRecordedPhrase[512];
    int           fRecordedNoteCount;
    GeneratedNote fGeneratedPhrase[512];
    int           fGeneratedNoteCount;
    GeneratedNote fAlterEgoPhrase[512];
    int           fAlterEgoNoteCount;
    bool          fAlterEgoActive[128];
    int           fAlterEgoActiveCount;
    int           fMutationOffsets[512];
    int           fActiveNoteBufferIndex[128];
    int           fDuetPrimary[128];   // live Duet PRIMARY  output pitch per held input note
    int           fDuetSecondary[128]; // live Duet SECONDARY harmony pitch per held input note
    TJBox_UInt8   fCurrentlyPlayingVoices[128];

    static const int kDuetQueueMax = 512;
    struct DuetNote { TJBox_UInt8 pitch; TJBox_UInt8 velocity; double firePPQ; };
    DuetNote      fDuetQueue[kDuetQueueMax];
    int           fDuetQueueSize;
    double        fDuetClockPPQ;

    TJBox_UInt32  fFramesSinceAllNotesOff, fFramesSinceSilence;

    TJBox_Float64 fPhraseLengthPPQ, fPatiencePPQRemaining;
    TJBox_Float64 fPlaybackPPQ, fShadowPlaybackPPQ;
    int           fLoopCount;

    bool          fFreezeOn, fFreezeJustReleased, fClearPending;
    bool          fLimitHit;
    int           fPitchDirectionMult;
    float         fPrevSeedCV, fPrevClearCV;

    int fPitchClassHistogram[12];
    int fActiveRoot;

    RippleData fInputRipples[64], fOutputRipples[64];
    float fWavePhase;
    int   fRippleTimer;
    float fInputRippleX[128], fOutputRippleX[128];
    TJBox_UInt32 fCurrentSeed;
};

// ---------------------------------------------------------------------------
extern "C" {
    void* JBox_Export_CreateNativeObject(const char iOperation[], const TJBox_Value iParams[], TJBox_UInt32 iCount) {
        if (std::strcmp(iOperation, "Instance") == 0)
            return new Narcissus(JBox_GetNumber(iParams[0]));
        return nullptr;
    }
    void JBox_Export_RenderRealtime(void* iPrivateState, const TJBox_PropertyDiff iPropertyDiffs[], TJBox_UInt32 iDiffCount) {
        if (iPrivateState) static_cast<Narcissus*>(iPrivateState)->RenderBatch(iPropertyDiffs, iDiffCount);
    }
}
