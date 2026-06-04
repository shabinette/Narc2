#pragma once

#include "JukeboxTypes.h"
#include <stddef.h>
#include <cstdint>

const std::size_t  kBatchSize          = 64;
const std::uint64_t kPPQResolution     = 15360; // 1 quarter note = 15360 ticks
const std::uint64_t kBeatPPQ           = 3840;  // 1 beat (quarter note)
const std::uint8_t  kInvalidNoteNumber = 128;
const int           kMaxRecordedNotes  = 512;   // max notes per phrase

// --- STATE CONTROLS (tag 1, 10-11) ---
const TJBox_Tag kOnOffTag    = 1;
const TJBox_Tag kFeedbackTag = 10;
const TJBox_Tag kFreezeTag   = 11;

// --- GLOBAL PITCH & SCALE (tags 20-22) ---
const TJBox_Tag kKeyLockTag   = 20;
const TJBox_Tag kScaleRootTag = 21;
const TJBox_Tag kScaleModeTag = 22;

// --- VOICES (tags 30-31) ---
const TJBox_Tag kVoiceTag    = 30;
const TJBox_Tag kAlterEgoTag = 31;

// --- RHYTHM & DYNAMICS (tags 40-45) ---
const TJBox_Tag kRateTag     = 40;
const TJBox_Tag kDensityTag  = 41;
const TJBox_Tag kLengthTag   = 42;
const TJBox_Tag kSwingTag    = 43;
const TJBox_Tag kVelocityTag = 44;
const TJBox_Tag kAccentTag   = 45;

// --- PITCH MANIPULATION (tags 50-51) ---
const TJBox_Tag kMutationTag = 50;
const TJBox_Tag kOctaveTag   = 51;

// --- ENGINE (tags 60-74) ---
const TJBox_Tag kResponseModeTag          = 60;
const TJBox_Tag kChokeTag                 = 61;
const TJBox_Tag kSeedTag                  = 62;
const TJBox_Tag kSilenceThresholdTag      = 63;
const TJBox_Tag kPatienceTag              = 64;
const TJBox_Tag kMemoryTag                = 65;
const TJBox_Tag kResponseLengthTag        = 66;
const TJBox_Tag kDriftTag                 = 67;
const TJBox_Tag kHarmonyTag               = 68;
const TJBox_Tag kSyncTag                  = 69;
const TJBox_Tag kLoopTag                  = 70;
const TJBox_Tag kSilenceThresholdSyncTag  = 71;
const TJBox_Tag kPatienceSyncTag          = 72;
const TJBox_Tag kKeepInputTag             = 73;
const TJBox_Tag kTransposeTag             = 74;

// --- CONTROL SECTION (tags 75-78) ---
const TJBox_Tag kClearTag      = 75;
const TJBox_Tag kLowLimitTag   = 76;
const TJBox_Tag kHighLimitTag  = 77;
const TJBox_Tag kDirectionTag  = 78;

// --- CV TRIMS (tags 100-116) ---
const TJBox_Tag kTrimFeedbackTag          = 100;
const TJBox_Tag kTrimFreezeTag            = 101;
const TJBox_Tag kTrimRateTag              = 102;
const TJBox_Tag kTrimDensityTag           = 103;
const TJBox_Tag kTrimVelocityTag          = 104;
const TJBox_Tag kTrimAccentTag            = 105;
const TJBox_Tag kTrimMutationTag          = 106;
const TJBox_Tag kTrimOctaveTag            = 107;
const TJBox_Tag kTrimResponseTag          = 108;
const TJBox_Tag kTrimChokeTag             = 109;
const TJBox_Tag kTrimSeedTag              = 110;
const TJBox_Tag kTrimSilenceThresholdTag  = 111;
const TJBox_Tag kTrimPatienceTag          = 112;
const TJBox_Tag kTrimMemoryTag            = 113;
const TJBox_Tag kTrimLengthTag            = 114;
const TJBox_Tag kTrimSwingTag             = 115;
const TJBox_Tag kTrimTransposeTag         = 116;
const TJBox_Tag kTrimHarmonyTag           = 121;
const TJBox_Tag kTrimLoopTag              = 122;
const TJBox_Tag kTrimClearTag             = 117;
const TJBox_Tag kTrimLowLimitTag          = 118;
const TJBox_Tag kTrimHighLimitTag         = 119;
const TJBox_Tag kTrimDirectionTag         = 120;
