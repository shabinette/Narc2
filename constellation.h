#pragma once

#include "Constants.h"
#include "Jukebox.h"
#include <cstdint>

// Represents a single captured or generated note
struct TBufferedNote {
    std::uint8_t pitch;
    std::uint8_t velocity;
    double onsetPPQ;    // Relative to the start of the phrase
    double durationPPQ; // Length of the note
};

enum ENarcissusState {
    STATE_IDLE,
    STATE_LISTENING,
    STATE_EVALUATING_SILENCE,
    STATE_PATIENCE,
    STATE_PLAYING
};

class CConstellation {
public:
    CConstellation(TJBox_Float64 iSampleRate);
    void RenderBatch(const TJBox_PropertyDiff iPropertyDiffs[], TJBox_UInt32 iDiffCount);

private:
    void HandleDiffs(const TJBox_PropertyDiff iPropertyDiffs[], TJBox_UInt32 iDiffCount);
    void ProcessStateMachine();
    void GenerateResponse();
    void SilenceAllOutputs();
    float GetTrimmedCV(TJBox_PropertyRef cvRef, TJBox_PropertyRef trimRef);
    double FrameToPPQ(std::uint16_t frameIndex);

    TJBox_ObjectRef fCustomPropertiesRef;
    TJBox_ObjectRef fTransportRef;
    TJBox_ObjectRef fNoteStates;

    TJBox_Float64 fSampleRate;
    TJBox_Float64 fTempo;
    bool fIsMainSequencerPlaying;

    bool fRT_RefsInitialized;
    bool fWasOn;

    // --- NARCISSUS ENGINE STATE ---
    ENarcissusState fState;
    std::int64_t fFramesSinceLastInput;
    std::int64_t fPatienceFramesRemaining;
    
    // Free-running internal clocks
    double fEnginePPQ;
    double fPhraseStartPPQ;
    double fPhraseLengthPPQ;
    double fPlaybackPPQ;

    // --- BUFFERS ---
    TBufferedNote fInputBuffer[kMaxBufferNotes];
    int fInputNoteCount;
    TBufferedNote fOutputBuffer[kMaxBufferNotes];
    int fOutputNoteCount;

    // Tracking active notes for gate calculation and duration
    int fActiveInputNotesCount;
    int fActiveNoteBufferIndex[128]; // Maps MIDI pitch to its index in fInputBuffer

    // --- OUTPUT REFS ---
    TJBox_PropertyRef fCVOutNoteMainRef;
    TJBox_PropertyRef fCVOutGateMainRef;
    TJBox_PropertyRef fCVOutNoteAlterEgoRef;
    TJBox_PropertyRef fCVOutGateAlterEgoRef;

    // --- INPUT & TRIM REFS ---
    // (Omitted for brevity, these remain exactly the same as the previous file)
    TJBox_PropertyRef fCVInFeedbackRef, fTrimFeedbackRef;
    TJBox_PropertyRef fCVInFreezeRef, fTrimFreezeRef;
    TJBox_PropertyRef fCVInRateRef, fTrimRateRef;
    TJBox_PropertyRef fCVInDensityRef, fTrimDensityRef;
    TJBox_PropertyRef fCVInVelocityRef, fTrimVelocityRef;
    TJBox_PropertyRef fCVInAccentRef, fTrimAccentRef;
    TJBox_PropertyRef fCVInMutationRef, fTrimMutationRef;
    TJBox_PropertyRef fCVInOctaveRef, fTrimOctaveRef;
    TJBox_PropertyRef fCVInResponseRef, fTrimResponseRef;
    TJBox_PropertyRef fCVInChokeRef, fTrimChokeRef;
    TJBox_PropertyRef fCVInSeedRef, fTrimSeedRef;
    TJBox_PropertyRef fCVInSilenceThresholdRef, fTrimSilenceThresholdRef;
    TJBox_PropertyRef fCVInPatienceRef, fTrimPatienceRef;
    TJBox_PropertyRef fCVInMemoryRef, fTrimMemoryRef;
};
