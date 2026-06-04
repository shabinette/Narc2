format_version = "2.0"
panel_type = "note_player"

front = {
    Bg = { { path = "Panel_Front_4U" } },

    -- GHOST NODES (satisfy SDK back-panel knob rule, always hidden)
    GhostNode_Knob = { offset = { 1300, 50 }, { path = "Knob_Blue", frames = 63 } },
    GhostNode_Display = { offset = { 1600, 50 }, { path = "Placeholder" } },

    -- REQUIRED SYSTEM ELEMENTS
    DeviceName = { offset = { 3290, 30 }, { path = "TapeHorz" } },
    PatchName = { offset = { 432, 79 }, { path = "Placeholder" } },
    PatchBrowser = { offset = { 772, 62 }, { path = "PatchBrowseGroup" } },
    OnOff = { offset = { 150, 62 }, { path = "ONOFF maker x5 002", frames = 4 } },
    NoteLamp = { offset = { 320, 62 }, { path = "Lamp_05_2frames", frames = 2 } },

    -- GLOBAL PITCH & SCALE (keep positions)
    KeyLock = { offset = { 951, 280 }, { path = "ob_button", frames = 2 } },
    RootNameDisplay = { offset = { 957, 394 }, { path = "KeyDisplayBg" } },
    ModeNameDisplay = { offset = { 849, 481 }, { path = "Placeholder" } },

    -- RESPONSE MODE
    Knob_ResponseMode = { offset = { 2432, 114 }, { path = "Knob_15_63frames", frames = 63 } },
    ResponseModeDisplay = { offset = { 2354, 439 }, { path = "Placeholder" } },

    -- STATE CONTROLS
    Feedback = { offset = { 2850, 326 }, { path = "ob_button", frames = 2 } },
    Freeze = { offset = { 3147, 326 }, { path = "ob_button", frames = 2 } },
    Button_Seed = { offset = { 3426, 326 }, { path = "ob_button", frames = 2 } },

    -- ENGINE SWITCHES (row at Y=597)
    Switch_Choke = { offset = { 2964, 597 }, { path = "Switch_06_2frames", frames = 2 } },
    Switch_KeepInput = { offset = { 3285, 597 }, { path = "Switch_06_2frames", frames = 2 } },

    -- SYNC & TIMING
    Switch_Sync = { offset = { 410, 408 }, { path = "Switch_06_2frames", frames = 2 } },
    Knob_SilenceThreshold = { offset = { 176, 405 }, { path = "Knob_Blue", frames = 63 } },
    Knob_SilenceThreshold_Sync = { offset = { 176, 405 }, { path = "Knob_Blue", frames = 63 } },
    Knob_Patience = { offset = { 516, 405 }, { path = "Knob_Blue", frames = 63 } },
    Knob_Patience_Sync = { offset = { 516, 405 }, { path = "Knob_Blue", frames = 63 } },

    -- RHYTHM & DYNAMICS
    Knob_Rate    = { offset = { 230, 751 }, { path = "Knob_11_63frames", frames = 63 } },
    Knob_Density = { offset = { 532, 751 }, { path = "Knob_11_63frames", frames = 63 } },
    Knob_Swing   = { offset = { 230, 934 }, { path = "Knob_11_63frames", frames = 63 } },
    Knob_Length  = { offset = { 532, 934 }, { path = "Knob_11_63frames", frames = 63 } },
    Knob_Velocity = { offset = { 230, 1135 }, { path = "Knob_11_63frames", frames = 63 } },
    Knob_Accent  = { offset = { 532, 1135 }, { path = "Knob_11_63frames", frames = 63 } },

    -- PITCH MANIPULATION
    Knob_Octave    = { offset = { 886, 611 }, { path = "Knob_Green", frames = 63 } },
    Knob_Harmony   = { offset = { 886, 845 }, { path = "Knob_Green", frames = 63 } },
    Knob_Transpose = { offset = { 886, 1088 }, { path = "Knob_Green", frames = 63 } },

    -- THE CUSTOM DISPLAY & BEZEL
    ReflectionPool = { offset = { 1287, 588 }, { path = "Pool" } },
    ReflectionBevil = { offset = { 1264, 570 }, { path = "Bevil" } },

    -- CONTROL SECTION
    Knob_LowLimit    = { offset = { 1534, 1180 }, { path = "Knob_27_63frames", frames = 63 } },
    LowLimitDisplay  = { offset = { 1501, 1320 }, { path = "Placeholder" } },
    Switch_Direction = { offset = { 1818, 1180 }, { path = "Switch_07_2frames", frames = 2 } },
    Knob_HighLimit   = { offset = { 2116, 1180 }, { path = "Knob_27_63frames", frames = 63 } },
    HighLimitDisplay = { offset = { 2083, 1320 }, { path = "Placeholder" } },

    -- ENGINE KNOBS (right column)
    Knob_Mutation      = { offset = { 2877, 848 }, { path = "Knob_27_63frames", frames = 63 } },
    Knob_Memory        = { offset = { 3154, 848 }, { path = "Knob_27_63frames", frames = 63 } },
    Knob_Drift         = { offset = { 3430, 848 }, { path = "Knob_27_63frames", frames = 63 } },
    Knob_ResponseLength = { offset = { 2881, 1068 }, { path = "Knob_Orange", frames = 63 } },
    Knob_Loop          = { offset = { 3308, 1068 }, { path = "Knob_Orange", frames = 63 } },

    -- CLEAR BUTTON
    Button_Clear = { offset = { 3149, 71 }, { path = "ob_button", frames = 2 } }

}

back = {
    Bg = { { path = "Panel_Back_4U" } },
    DeviceName = { offset = { 2880, 30 }, { path = "TapeHorz" } },
    Placeholder = { offset = { 150, 150 }, { path = "Placeholder" } },

    -- ALTER EGO CONTROLS
    Knob_AlterEgo = { offset = { 1453, 746 }, { path = "Knob_15_63frames", frames = 63 } },
    AlterEgoNameDisplay = { offset = { 1371, 1038 }, { path = "Placeholder" } },

    -- CV OUTPUT JACKS (keep current positions)
    Jack_Out_Note = { offset = { 1995, 327 }, { path = "SharedCVJack", frames = 3 } },
    Jack_Out_Gate = { offset = { 2128, 327 }, { path = "SharedCVJack", frames = 3 } },
    Jack_Out_AlterEgo_Note = { offset = { 1995, 959 }, { path = "SharedCVJack", frames = 3 } },
    Jack_Out_AlterEgo_Gate = { offset = { 2128, 959 }, { path = "SharedCVJack", frames = 3 } },

    -- CV INPUT JACKS & TRIMS
    -- State Controls (front: Feedback=2830, Freeze=3127, Seed=3436 — all X>1885, subtract 325)
    Jack_Feedback = { offset = { 2505, 326 }, { path = "SharedCVJack", frames = 3 } }, Trim_Feedback = { offset = { 2395, 326 }, { path = "TrimKnob" } },
    Jack_Freeze   = { offset = { 2802, 326 }, { path = "SharedCVJack", frames = 3 } }, Trim_Freeze   = { offset = { 2692, 326 }, { path = "TrimKnob" } },
    Jack_Seed     = { offset = { 3111, 326 }, { path = "SharedCVJack", frames = 3 } }, Trim_Seed     = { offset = { 3001, 326 }, { path = "TrimKnob" } },

    -- Clear (y=100 keeps top edge below DeviceName bottom at y=95)
    Jack_Clear = { offset = { 2824, 100 }, { path = "SharedCVJack", frames = 3 } }, Trim_Clear = { offset = { 2714, 100 }, { path = "TrimKnob" } },

    -- Engine Switches (front: Choke=2984 — X>1885, subtract 325)
    Jack_Choke = { offset = { 2659, 597 }, { path = "SharedCVJack", frames = 3 } }, Trim_Choke = { offset = { 2549, 597 }, { path = "TrimKnob" } },

    -- Sync/Timing (front: SilenceThreshold=226, Patience=566 — X<1885, add 325)
    Jack_SilenceThreshold = { offset = { 551, 435 }, { path = "SharedCVJack", frames = 3 } }, Trim_SilenceThreshold = { offset = { 441, 435 }, { path = "TrimKnob" } },
    Jack_Patience         = { offset = { 891, 435 }, { path = "SharedCVJack", frames = 3 } }, Trim_Patience         = { offset = { 781, 435 }, { path = "TrimKnob" } },

    -- Rhythm & Dynamics (front X<1885 for all: Rate=260, Density=562, Swing=260, Length=562, Velocity=260, Accent=562 — add 325)
    Jack_Rate    = { offset = { 585, 781 }, { path = "SharedCVJack", frames = 3 } }, Trim_Rate    = { offset = { 475, 781 }, { path = "TrimKnob" } },
    Jack_Density = { offset = { 887, 781 }, { path = "SharedCVJack", frames = 3 } }, Trim_Density = { offset = { 777, 781 }, { path = "TrimKnob" } },
    Jack_Swing   = { offset = { 585, 964 }, { path = "SharedCVJack", frames = 3 } }, Trim_Swing   = { offset = { 475, 964 }, { path = "TrimKnob" } },
    Jack_Length  = { offset = { 887, 964 }, { path = "SharedCVJack", frames = 3 } }, Trim_Length  = { offset = { 777, 964 }, { path = "TrimKnob" } },
    Jack_Velocity = { offset = { 585, 1165 }, { path = "SharedCVJack", frames = 3 } }, Trim_Velocity = { offset = { 475, 1165 }, { path = "TrimKnob" } },
    Jack_Accent  = { offset = { 887, 1165 }, { path = "SharedCVJack", frames = 3 } }, Trim_Accent  = { offset = { 777, 1165 }, { path = "TrimKnob" } },

    -- Pitch Manipulation (front X=936 <1885, add 325 → 1261)
    Jack_Octave    = { offset = { 1261, 661 }, { path = "SharedCVJack", frames = 3 } }, Trim_Octave    = { offset = { 1151, 661 }, { path = "TrimKnob" } },
    Jack_Harmony   = { offset = { 1261, 895 }, { path = "SharedCVJack", frames = 3 } }, Trim_Harmony   = { offset = { 1151, 895 }, { path = "TrimKnob" } },
    Jack_Transpose = { offset = { 1261, 1138 }, { path = "SharedCVJack", frames = 3 } }, Trim_Transpose = { offset = { 1151, 1138 }, { path = "TrimKnob" } },

    -- Control Section (evenly spaced 300px apart, centered on X=1885; HighLimit|Direction|LowLimit left-to-right)
    Jack_HighLimit = { offset = { 1585, 1180 }, { path = "SharedCVJack", frames = 3 } }, Trim_HighLimit = { offset = { 1475, 1180 }, { path = "TrimKnob" } },
    Jack_Direction = { offset = { 1885, 1180 }, { path = "SharedCVJack", frames = 3 } }, Trim_Direction = { offset = { 1775, 1180 }, { path = "TrimKnob" } },
    Jack_LowLimit  = { offset = { 2185, 1180 }, { path = "SharedCVJack", frames = 3 } }, Trim_LowLimit  = { offset = { 2075, 1180 }, { path = "TrimKnob" } },

    -- Response Mode (front: 2447>1885, subtract 325 → 2122)
    Jack_Response = { offset = { 2122, 144 }, { path = "SharedCVJack", frames = 3 } }, Trim_Response = { offset = { 2012, 144 }, { path = "TrimKnob" } },

    -- Engine Knobs (front: Mutation=2877, Memory=3154 — X>1885, subtract 325)
    Jack_Mutation = { offset = { 2552, 863 }, { path = "SharedCVJack", frames = 3 } }, Trim_Mutation = { offset = { 2442, 863 }, { path = "TrimKnob" } },
    Jack_Memory   = { offset = { 2829, 863 }, { path = "SharedCVJack", frames = 3 } }, Trim_Memory   = { offset = { 2719, 863 }, { path = "TrimKnob" } },

    -- Response Length & Loop (front: ResponseLength=2921, Loop=3348 — X>1885, subtract 325)
    Jack_Loop = { offset = { 3023, 1108 }, { path = "SharedCVJack", frames = 3 } }, Trim_Loop = { offset = { 2913, 1108 }, { path = "TrimKnob" } }
}
