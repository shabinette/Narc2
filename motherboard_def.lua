format_version = "3.0"

custom_properties = jbox.property_set{
    gui_owner = {
        properties = {
            ["AlterEgo_BackGUI"] = jbox.number{
                default = 1, steps = 8,
                ui_name = jbox.ui_text("text_AlterEgo"),
                ui_type = jbox.ui_selector{ jbox.ui_text("text_voice_0"), jbox.ui_text("text_voice_1"), jbox.ui_text("text_voice_2"), jbox.ui_text("text_voice_3"), jbox.ui_text("text_voice_4"), jbox.ui_text("text_voice_5"), jbox.ui_text("text_voice_6"), jbox.ui_text("text_voice_7") }
            },
            ["GhostHide"] = jbox.number{
                default = 0, steps = 2,
                ui_name = jbox.ui_text("text_OnOff"),
                ui_type = jbox.ui_selector{ jbox.ui_text("text_Off"), jbox.ui_text("text_On") }
            }
        }
    },
    
    document_owner = {
        properties = {
            ["OnOff"] = jbox.boolean{ property_tag = 1, default = true, ui_name = jbox.ui_text("text_OnOff"), ui_type = jbox.ui_selector{ jbox.ui_text("text_Off"), jbox.ui_text("text_On") } },

            ["Feedback"] = jbox.boolean{ property_tag = 10, default = false, ui_name = jbox.ui_text("text_Feedback"), ui_type = jbox.ui_selector{ jbox.ui_text("text_Off"), jbox.ui_text("text_On") } },
            ["Freeze"] = jbox.boolean{ property_tag = 11, default = false, ui_name = jbox.ui_text("text_Freeze"), ui_type = jbox.ui_selector{ jbox.ui_text("text_Off"), jbox.ui_text("text_On") } },

            ["KeyLock"] = jbox.boolean{ property_tag = 20, default = false, ui_name = jbox.ui_text("text_KeyLock"), ui_type = jbox.ui_selector{ jbox.ui_text("text_Off"), jbox.ui_text("text_On") }, persistence = "song" },
            ["ScaleRoot"] = jbox.number{ property_tag = 21, steps = 14, default = 0, ui_name = jbox.ui_text("text_ScaleRoot"), ui_type = jbox.ui_selector{ jbox.ui_text("text_root_0"), jbox.ui_text("text_root_1"), jbox.ui_text("text_root_2"), jbox.ui_text("text_root_3"), jbox.ui_text("text_root_4"), jbox.ui_text("text_root_5"), jbox.ui_text("text_root_6"), jbox.ui_text("text_root_7"), jbox.ui_text("text_root_8"), jbox.ui_text("text_root_9"), jbox.ui_text("text_root_10"), jbox.ui_text("text_root_11"), jbox.ui_text("text_root_12"), jbox.ui_text("text_root_13") } },
            ["ScaleMode"] = jbox.number{ property_tag = 22, steps = 9, default = 0, ui_name = jbox.ui_text("text_ScaleMode"), ui_type = jbox.ui_selector{ jbox.ui_text("text_mode_0"), jbox.ui_text("text_mode_1"), jbox.ui_text("text_mode_2"), jbox.ui_text("text_mode_3"), jbox.ui_text("text_mode_4"), jbox.ui_text("text_mode_5"), jbox.ui_text("text_mode_6"), jbox.ui_text("text_mode_7"), jbox.ui_text("text_mode_8") } },

            ["Voice"] = jbox.number{ property_tag = 30, steps = 8, default = 0, ui_name = jbox.ui_text("text_Voice"), ui_type = jbox.ui_selector{ jbox.ui_text("text_voice_0"), jbox.ui_text("text_voice_1"), jbox.ui_text("text_voice_2"), jbox.ui_text("text_voice_3"), jbox.ui_text("text_voice_4"), jbox.ui_text("text_voice_5"), jbox.ui_text("text_voice_6"), jbox.ui_text("text_voice_7") } },
            ["AlterEgo"] = jbox.number{ property_tag = 31, steps = 9, default = 0, ui_name = jbox.ui_text("text_AlterEgo"), ui_type = jbox.ui_selector{ jbox.ui_text("text_resp_mirror"), jbox.ui_text("text_resp_upsidedown"), jbox.ui_text("text_resp_warp"), jbox.ui_text("text_resp_retro"), jbox.ui_text("text_resp_shuffle"), jbox.ui_text("text_resp_pendulum"), jbox.ui_text("text_resp_cascadeup"), jbox.ui_text("text_resp_cascadedown"), jbox.ui_text("text_resp_chord") } },

            -- RHYTHM & DYNAMICS
            ["Rate"] = jbox.number{ property_tag = 40, steps = 20, default = 0, ui_name = jbox.ui_text("text_Rate"), ui_type = jbox.ui_selector{ jbox.ui_text("text_rate_0"), jbox.ui_text("text_rate_1"), jbox.ui_text("text_rate_2"), jbox.ui_text("text_rate_3"), jbox.ui_text("text_rate_4"), jbox.ui_text("text_rate_5"), jbox.ui_text("text_rate_6"), jbox.ui_text("text_rate_7"), jbox.ui_text("text_rate_8"), jbox.ui_text("text_rate_9"), jbox.ui_text("text_rate_10"), jbox.ui_text("text_rate_11"), jbox.ui_text("text_rate_12"), jbox.ui_text("text_rate_13"), jbox.ui_text("text_rate_14"), jbox.ui_text("text_rate_15"), jbox.ui_text("text_rate_16"), jbox.ui_text("text_rate_17"), jbox.ui_text("text_rate_18"), jbox.ui_text("text_rate_19") } },
            ["Density"] = jbox.number{ property_tag = 41, default = 0.5, ui_name = jbox.ui_text("text_Density"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Length"] = jbox.number{ property_tag = 42, default = 0.5, ui_name = jbox.ui_text("text_Length"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Swing"] = jbox.number{ property_tag = 43, default = 0.5, ui_name = jbox.ui_text("text_Swing"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Velocity"] = jbox.number{ property_tag = 44, default = 0.5, ui_name = jbox.ui_text("text_Velocity"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Accent"] = jbox.number{ property_tag = 45, default = 0.0, ui_name = jbox.ui_text("text_Accent"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },

            -- PITCH MANIPULATION
            ["Mutation"] = jbox.number{ property_tag = 50, default = 0.0, ui_name = jbox.ui_text("text_Mutation"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Octave"] = jbox.number{ property_tag = 51, steps = 7, default = 3, ui_name = jbox.ui_text("text_Octave"), ui_type = jbox.ui_selector{ jbox.ui_text("text_oct_m3"), jbox.ui_text("text_oct_m2"), jbox.ui_text("text_oct_m1"), jbox.ui_text("text_oct_0"), jbox.ui_text("text_oct_p1"), jbox.ui_text("text_oct_p2"), jbox.ui_text("text_oct_p3") } },

            -- THE ENGINE
            ["ResponseMode"] = jbox.number{ property_tag = 60, steps = 9, default = 0, ui_name = jbox.ui_text("text_ResponseMode"), ui_type = jbox.ui_selector{ jbox.ui_text("text_resp_mirror"), jbox.ui_text("text_resp_upsidedown"), jbox.ui_text("text_resp_warp"), jbox.ui_text("text_resp_retro"), jbox.ui_text("text_resp_shuffle"), jbox.ui_text("text_resp_pendulum"), jbox.ui_text("text_resp_cascadeup"), jbox.ui_text("text_resp_cascadedown"), jbox.ui_text("text_resp_chord") } },
            ["Choke"] = jbox.boolean{ property_tag = 61, default = true, ui_name = jbox.ui_text("text_Choke"), ui_type = jbox.ui_selector{ jbox.ui_text("text_Duet"), jbox.ui_text("text_ChokeMode") } },
            ["Seed"] = jbox.boolean{ property_tag = 62, default = false, ui_name = jbox.ui_text("text_Seed"), ui_type = jbox.ui_selector{ jbox.ui_text("text_Off"), jbox.ui_text("text_On") } },
            ["Clear"] = jbox.boolean{ property_tag = 75, default = false, ui_name = jbox.ui_text("text_Clear"), ui_type = jbox.ui_selector{ jbox.ui_text("text_Off"), jbox.ui_text("text_On") } },
            ["SilenceThreshold"] = jbox.number{ property_tag = 63, default = 0.25, ui_name = jbox.ui_text("text_SilenceThreshold"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Patience"] = jbox.number{ property_tag = 64, default = 0.0, ui_name = jbox.ui_text("text_Patience"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Memory"] = jbox.number{ property_tag = 65, default = 1.0, ui_name = jbox.ui_text("text_Memory"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["ResponseLength"] = jbox.number{ property_tag = 66, steps = 8, default = 2, ui_name = jbox.ui_text("text_ResponseLength"), ui_type = jbox.ui_selector{ jbox.ui_text("text_len_025"), jbox.ui_text("text_len_05"), jbox.ui_text("text_len_1"), jbox.ui_text("text_len_15"), jbox.ui_text("text_len_2"), jbox.ui_text("text_len_25"), jbox.ui_text("text_len_3"), jbox.ui_text("text_len_4") } },
            ["Drift"] = jbox.number{ property_tag = 67, default = 0.0, ui_name = jbox.ui_text("text_Drift"), ui_type = jbox.ui_linear({min=0.0, max=1.0, units={{decimals=2}}}) },
            ["Harmony"] = jbox.number{ property_tag = 68, steps = 15, default = 7, ui_name = jbox.ui_text("text_Harmony"), ui_type = jbox.ui_linear({min=-7, max=7, units={{decimals=0}}}) },
            ["Sync"] = jbox.number{ property_tag = 69, steps = 2, default = 0, ui_name = jbox.ui_text("text_Sync"), ui_type = jbox.ui_selector{ jbox.ui_text("text_Free"), jbox.ui_text("text_Sync") } },
            ["Loop"] = jbox.number{ property_tag = 70, steps = 8, default = 7, ui_name = jbox.ui_text("text_Loop"), ui_type = jbox.ui_selector{ jbox.ui_text("text_loop_1"), jbox.ui_text("text_loop_2"), jbox.ui_text("text_loop_3"), jbox.ui_text("text_loop_4"), jbox.ui_text("text_loop_8"), jbox.ui_text("text_loop_16"), jbox.ui_text("text_loop_32"), jbox.ui_text("text_loop_inf") } },
            ["SilenceThreshold_Sync"] = jbox.number{ property_tag = 71, steps = 10, default = 3, ui_name = jbox.ui_text("text_SilenceThreshold"), ui_type = jbox.ui_selector{ jbox.ui_text("text_sync_off"), jbox.ui_text("text_sync_1_64"), jbox.ui_text("text_sync_1_32"), jbox.ui_text("text_sync_1_16"), jbox.ui_text("text_sync_1_8"), jbox.ui_text("text_sync_1_4"), jbox.ui_text("text_sync_1_2"), jbox.ui_text("text_sync_1_bar"), jbox.ui_text("text_sync_2_bar"), jbox.ui_text("text_sync_4_bar") } },
            ["Patience_Sync"] = jbox.number{ property_tag = 72, steps = 10, default = 0, ui_name = jbox.ui_text("text_Patience"), ui_type = jbox.ui_selector{ jbox.ui_text("text_sync_off"), jbox.ui_text("text_sync_1_64"), jbox.ui_text("text_sync_1_32"), jbox.ui_text("text_sync_1_16"), jbox.ui_text("text_sync_1_8"), jbox.ui_text("text_sync_1_4"), jbox.ui_text("text_sync_1_2"), jbox.ui_text("text_sync_1_bar"), jbox.ui_text("text_sync_2_bar"), jbox.ui_text("text_sync_4_bar") } },
            
            -- NEW LOOPER CONTROLS
            ["KeepInput"] = jbox.boolean{ property_tag = 73, default = false, ui_name = jbox.ui_text("text_KeepInput"), ui_type = jbox.ui_selector{ jbox.ui_text("text_Off"), jbox.ui_text("text_On") } },
            ["Transpose"] = jbox.number{ property_tag = 74, steps = 33, default = 16, ui_name = jbox.ui_text("text_Transpose"), ui_type = jbox.ui_linear({min=-16, max=16, units={{decimals=0}}}) },

            -- CONTROL SECTION
            ["LowLimit"] = jbox.number{ property_tag = 76, steps = 128, default = 0, ui_name = jbox.ui_text("text_LowLimit"),
                ui_type = jbox.ui_selector{ jbox.ui_text("text_note_0"), jbox.ui_text("text_note_1"), jbox.ui_text("text_note_2"), jbox.ui_text("text_note_3"), jbox.ui_text("text_note_4"), jbox.ui_text("text_note_5"), jbox.ui_text("text_note_6"), jbox.ui_text("text_note_7"), jbox.ui_text("text_note_8"), jbox.ui_text("text_note_9"), jbox.ui_text("text_note_10"), jbox.ui_text("text_note_11"), jbox.ui_text("text_note_12"), jbox.ui_text("text_note_13"), jbox.ui_text("text_note_14"), jbox.ui_text("text_note_15"), jbox.ui_text("text_note_16"), jbox.ui_text("text_note_17"), jbox.ui_text("text_note_18"), jbox.ui_text("text_note_19"), jbox.ui_text("text_note_20"), jbox.ui_text("text_note_21"), jbox.ui_text("text_note_22"), jbox.ui_text("text_note_23"), jbox.ui_text("text_note_24"), jbox.ui_text("text_note_25"), jbox.ui_text("text_note_26"), jbox.ui_text("text_note_27"), jbox.ui_text("text_note_28"), jbox.ui_text("text_note_29"), jbox.ui_text("text_note_30"), jbox.ui_text("text_note_31"), jbox.ui_text("text_note_32"), jbox.ui_text("text_note_33"), jbox.ui_text("text_note_34"), jbox.ui_text("text_note_35"), jbox.ui_text("text_note_36"), jbox.ui_text("text_note_37"), jbox.ui_text("text_note_38"), jbox.ui_text("text_note_39"), jbox.ui_text("text_note_40"), jbox.ui_text("text_note_41"), jbox.ui_text("text_note_42"), jbox.ui_text("text_note_43"), jbox.ui_text("text_note_44"), jbox.ui_text("text_note_45"), jbox.ui_text("text_note_46"), jbox.ui_text("text_note_47"), jbox.ui_text("text_note_48"), jbox.ui_text("text_note_49"), jbox.ui_text("text_note_50"), jbox.ui_text("text_note_51"), jbox.ui_text("text_note_52"), jbox.ui_text("text_note_53"), jbox.ui_text("text_note_54"), jbox.ui_text("text_note_55"), jbox.ui_text("text_note_56"), jbox.ui_text("text_note_57"), jbox.ui_text("text_note_58"), jbox.ui_text("text_note_59"), jbox.ui_text("text_note_60"), jbox.ui_text("text_note_61"), jbox.ui_text("text_note_62"), jbox.ui_text("text_note_63"), jbox.ui_text("text_note_64"), jbox.ui_text("text_note_65"), jbox.ui_text("text_note_66"), jbox.ui_text("text_note_67"), jbox.ui_text("text_note_68"), jbox.ui_text("text_note_69"), jbox.ui_text("text_note_70"), jbox.ui_text("text_note_71"), jbox.ui_text("text_note_72"), jbox.ui_text("text_note_73"), jbox.ui_text("text_note_74"), jbox.ui_text("text_note_75"), jbox.ui_text("text_note_76"), jbox.ui_text("text_note_77"), jbox.ui_text("text_note_78"), jbox.ui_text("text_note_79"), jbox.ui_text("text_note_80"), jbox.ui_text("text_note_81"), jbox.ui_text("text_note_82"), jbox.ui_text("text_note_83"), jbox.ui_text("text_note_84"), jbox.ui_text("text_note_85"), jbox.ui_text("text_note_86"), jbox.ui_text("text_note_87"), jbox.ui_text("text_note_88"), jbox.ui_text("text_note_89"), jbox.ui_text("text_note_90"), jbox.ui_text("text_note_91"), jbox.ui_text("text_note_92"), jbox.ui_text("text_note_93"), jbox.ui_text("text_note_94"), jbox.ui_text("text_note_95"), jbox.ui_text("text_note_96"), jbox.ui_text("text_note_97"), jbox.ui_text("text_note_98"), jbox.ui_text("text_note_99"), jbox.ui_text("text_note_100"), jbox.ui_text("text_note_101"), jbox.ui_text("text_note_102"), jbox.ui_text("text_note_103"), jbox.ui_text("text_note_104"), jbox.ui_text("text_note_105"), jbox.ui_text("text_note_106"), jbox.ui_text("text_note_107"), jbox.ui_text("text_note_108"), jbox.ui_text("text_note_109"), jbox.ui_text("text_note_110"), jbox.ui_text("text_note_111"), jbox.ui_text("text_note_112"), jbox.ui_text("text_note_113"), jbox.ui_text("text_note_114"), jbox.ui_text("text_note_115"), jbox.ui_text("text_note_116"), jbox.ui_text("text_note_117"), jbox.ui_text("text_note_118"), jbox.ui_text("text_note_119"), jbox.ui_text("text_note_120"), jbox.ui_text("text_note_121"), jbox.ui_text("text_note_122"), jbox.ui_text("text_note_123"), jbox.ui_text("text_note_124"), jbox.ui_text("text_note_125"), jbox.ui_text("text_note_126"), jbox.ui_text("text_note_127") }
            },
            ["HighLimit"] = jbox.number{ property_tag = 77, steps = 128, default = 127, ui_name = jbox.ui_text("text_HighLimit"),
                ui_type = jbox.ui_selector{ jbox.ui_text("text_note_0"), jbox.ui_text("text_note_1"), jbox.ui_text("text_note_2"), jbox.ui_text("text_note_3"), jbox.ui_text("text_note_4"), jbox.ui_text("text_note_5"), jbox.ui_text("text_note_6"), jbox.ui_text("text_note_7"), jbox.ui_text("text_note_8"), jbox.ui_text("text_note_9"), jbox.ui_text("text_note_10"), jbox.ui_text("text_note_11"), jbox.ui_text("text_note_12"), jbox.ui_text("text_note_13"), jbox.ui_text("text_note_14"), jbox.ui_text("text_note_15"), jbox.ui_text("text_note_16"), jbox.ui_text("text_note_17"), jbox.ui_text("text_note_18"), jbox.ui_text("text_note_19"), jbox.ui_text("text_note_20"), jbox.ui_text("text_note_21"), jbox.ui_text("text_note_22"), jbox.ui_text("text_note_23"), jbox.ui_text("text_note_24"), jbox.ui_text("text_note_25"), jbox.ui_text("text_note_26"), jbox.ui_text("text_note_27"), jbox.ui_text("text_note_28"), jbox.ui_text("text_note_29"), jbox.ui_text("text_note_30"), jbox.ui_text("text_note_31"), jbox.ui_text("text_note_32"), jbox.ui_text("text_note_33"), jbox.ui_text("text_note_34"), jbox.ui_text("text_note_35"), jbox.ui_text("text_note_36"), jbox.ui_text("text_note_37"), jbox.ui_text("text_note_38"), jbox.ui_text("text_note_39"), jbox.ui_text("text_note_40"), jbox.ui_text("text_note_41"), jbox.ui_text("text_note_42"), jbox.ui_text("text_note_43"), jbox.ui_text("text_note_44"), jbox.ui_text("text_note_45"), jbox.ui_text("text_note_46"), jbox.ui_text("text_note_47"), jbox.ui_text("text_note_48"), jbox.ui_text("text_note_49"), jbox.ui_text("text_note_50"), jbox.ui_text("text_note_51"), jbox.ui_text("text_note_52"), jbox.ui_text("text_note_53"), jbox.ui_text("text_note_54"), jbox.ui_text("text_note_55"), jbox.ui_text("text_note_56"), jbox.ui_text("text_note_57"), jbox.ui_text("text_note_58"), jbox.ui_text("text_note_59"), jbox.ui_text("text_note_60"), jbox.ui_text("text_note_61"), jbox.ui_text("text_note_62"), jbox.ui_text("text_note_63"), jbox.ui_text("text_note_64"), jbox.ui_text("text_note_65"), jbox.ui_text("text_note_66"), jbox.ui_text("text_note_67"), jbox.ui_text("text_note_68"), jbox.ui_text("text_note_69"), jbox.ui_text("text_note_70"), jbox.ui_text("text_note_71"), jbox.ui_text("text_note_72"), jbox.ui_text("text_note_73"), jbox.ui_text("text_note_74"), jbox.ui_text("text_note_75"), jbox.ui_text("text_note_76"), jbox.ui_text("text_note_77"), jbox.ui_text("text_note_78"), jbox.ui_text("text_note_79"), jbox.ui_text("text_note_80"), jbox.ui_text("text_note_81"), jbox.ui_text("text_note_82"), jbox.ui_text("text_note_83"), jbox.ui_text("text_note_84"), jbox.ui_text("text_note_85"), jbox.ui_text("text_note_86"), jbox.ui_text("text_note_87"), jbox.ui_text("text_note_88"), jbox.ui_text("text_note_89"), jbox.ui_text("text_note_90"), jbox.ui_text("text_note_91"), jbox.ui_text("text_note_92"), jbox.ui_text("text_note_93"), jbox.ui_text("text_note_94"), jbox.ui_text("text_note_95"), jbox.ui_text("text_note_96"), jbox.ui_text("text_note_97"), jbox.ui_text("text_note_98"), jbox.ui_text("text_note_99"), jbox.ui_text("text_note_100"), jbox.ui_text("text_note_101"), jbox.ui_text("text_note_102"), jbox.ui_text("text_note_103"), jbox.ui_text("text_note_104"), jbox.ui_text("text_note_105"), jbox.ui_text("text_note_106"), jbox.ui_text("text_note_107"), jbox.ui_text("text_note_108"), jbox.ui_text("text_note_109"), jbox.ui_text("text_note_110"), jbox.ui_text("text_note_111"), jbox.ui_text("text_note_112"), jbox.ui_text("text_note_113"), jbox.ui_text("text_note_114"), jbox.ui_text("text_note_115"), jbox.ui_text("text_note_116"), jbox.ui_text("text_note_117"), jbox.ui_text("text_note_118"), jbox.ui_text("text_note_119"), jbox.ui_text("text_note_120"), jbox.ui_text("text_note_121"), jbox.ui_text("text_note_122"), jbox.ui_text("text_note_123"), jbox.ui_text("text_note_124"), jbox.ui_text("text_note_125"), jbox.ui_text("text_note_126"), jbox.ui_text("text_note_127") }
            },
            ["Direction"] = jbox.boolean{ property_tag = 78, default = false, ui_name = jbox.ui_text("text_Direction"), ui_type = jbox.ui_selector{ jbox.ui_text("text_dir_bounce"), jbox.ui_text("text_dir_reset") } },

            -- CV TRIMS
            ["Trim_Feedback"] = jbox.number{ property_tag = 100, default = 1.0, ui_name = jbox.ui_text("text_Trim_Feedback"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Freeze"] = jbox.number{ property_tag = 101, default = 1.0, ui_name = jbox.ui_text("text_Trim_Freeze"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Rate"] = jbox.number{ property_tag = 102, default = 1.0, ui_name = jbox.ui_text("text_Trim_Rate"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Density"] = jbox.number{ property_tag = 103, default = 1.0, ui_name = jbox.ui_text("text_Trim_Density"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Length"] = jbox.number{ property_tag = 114, default = 1.0, ui_name = jbox.ui_text("text_Trim_Length"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Swing"] = jbox.number{ property_tag = 115, default = 1.0, ui_name = jbox.ui_text("text_Trim_Swing"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Velocity"] = jbox.number{ property_tag = 104, default = 1.0, ui_name = jbox.ui_text("text_Trim_Velocity"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Accent"] = jbox.number{ property_tag = 105, default = 1.0, ui_name = jbox.ui_text("text_Trim_Accent"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Mutation"] = jbox.number{ property_tag = 106, default = 1.0, ui_name = jbox.ui_text("text_Trim_Mutation"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Octave"] = jbox.number{ property_tag = 107, default = 1.0, ui_name = jbox.ui_text("text_Trim_Octave"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Response"] = jbox.number{ property_tag = 108, default = 1.0, ui_name = jbox.ui_text("text_Trim_Response"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Choke"] = jbox.number{ property_tag = 109, default = 1.0, ui_name = jbox.ui_text("text_Trim_Choke"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Seed"] = jbox.number{ property_tag = 110, default = 1.0, ui_name = jbox.ui_text("text_Trim_Seed"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_SilenceThreshold"] = jbox.number{ property_tag = 111, default = 1.0, ui_name = jbox.ui_text("text_Trim_SilenceThreshold"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Patience"] = jbox.number{ property_tag = 112, default = 1.0, ui_name = jbox.ui_text("text_Trim_Patience"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Memory"] = jbox.number{ property_tag = 113, default = 1.0, ui_name = jbox.ui_text("text_Trim_Memory"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Transpose"] = jbox.number{ property_tag = 116, default = 1.0, ui_name = jbox.ui_text("text_Trim_Transpose"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Harmony"] = jbox.number{ property_tag = 121, default = 1.0, ui_name = jbox.ui_text("text_Trim_Harmony"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Loop"] = jbox.number{ property_tag = 122, default = 1.0, ui_name = jbox.ui_text("text_Trim_Loop"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Clear"] = jbox.number{ property_tag = 117, default = 1.0, ui_name = jbox.ui_text("text_Trim_Clear"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_LowLimit"] = jbox.number{ property_tag = 118, default = 1.0, ui_name = jbox.ui_text("text_Trim_LowLimit"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_HighLimit"] = jbox.number{ property_tag = 119, default = 1.0, ui_name = jbox.ui_text("text_Trim_HighLimit"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) },
            ["Trim_Direction"] = jbox.number{ property_tag = 120, default = 1.0, ui_name = jbox.ui_text("text_Trim_Direction"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=2}}}) }
        }
    },
    rtc_owner = { properties = { ["instance"] = jbox.native_object{} } },
    rt_owner = {
        properties = {
            ["ReflectionDisplayData"] = jbox.string{ max_size = 2048 },
            ["NoteOnIndicator"] = jbox.boolean{ default = false, ui_name = jbox.ui_text("text_On"), ui_type = jbox.ui_linear({min=0, max=1, units={{decimals=0}}}) }
        }
    },
    ui_groups = {
        { ui_name = jbox.ui_text("text_GroupState"), properties = { "/custom_properties/Feedback", "/custom_properties/Freeze", "/custom_properties/KeepInput", "/custom_properties/Transpose" } },
        { ui_name = jbox.ui_text("text_GroupPitch"), properties = { "/custom_properties/KeyLock", "/custom_properties/ScaleRoot", "/custom_properties/ScaleMode", "/custom_properties/Mutation", "/custom_properties/Octave" } },
        { ui_name = jbox.ui_text("text_GroupVoices"), properties = { "/custom_properties/Voice", "/custom_properties/AlterEgo" } },
        { ui_name = jbox.ui_text("text_GroupRhythm"), properties = { "/custom_properties/Rate", "/custom_properties/Density", "/custom_properties/Length", "/custom_properties/Swing", "/custom_properties/Velocity", "/custom_properties/Accent" } },
        { ui_name = jbox.ui_text("text_GroupEngine"), properties = { "/custom_properties/ResponseMode", "/custom_properties/ResponseLength", "/custom_properties/Drift", "/custom_properties/Harmony", "/custom_properties/Choke", "/custom_properties/Seed", "/custom_properties/SilenceThreshold", "/custom_properties/Patience", "/custom_properties/Memory", "/custom_properties/Sync", "/custom_properties/Loop" } }
    }
}

cv_inputs = {
    CV_Feedback = jbox.cv_input{ ui_name = jbox.ui_text("text_FeedbackCV") },
    CV_Freeze = jbox.cv_input{ ui_name = jbox.ui_text("text_FreezeCV") },
    CV_Rate = jbox.cv_input{ ui_name = jbox.ui_text("text_RateCV") },
    CV_Density = jbox.cv_input{ ui_name = jbox.ui_text("text_DensityCV") },
    CV_Length = jbox.cv_input{ ui_name = jbox.ui_text("text_LengthCV") },
    CV_Swing = jbox.cv_input{ ui_name = jbox.ui_text("text_SwingCV") },
    CV_Velocity = jbox.cv_input{ ui_name = jbox.ui_text("text_VelocityCV") },
    CV_Accent = jbox.cv_input{ ui_name = jbox.ui_text("text_AccentCV") },
    CV_Mutation = jbox.cv_input{ ui_name = jbox.ui_text("text_MutationCV") },
    CV_Octave = jbox.cv_input{ ui_name = jbox.ui_text("text_OctaveCV") },
    CV_Response = jbox.cv_input{ ui_name = jbox.ui_text("text_ResponseCV") },
    CV_Choke = jbox.cv_input{ ui_name = jbox.ui_text("text_ChokeCV") },
    CV_Seed = jbox.cv_input{ ui_name = jbox.ui_text("text_SeedCV") },
    CV_SilenceThreshold = jbox.cv_input{ ui_name = jbox.ui_text("text_SilenceThresholdCV") },
    CV_Patience = jbox.cv_input{ ui_name = jbox.ui_text("text_PatienceCV") },
    CV_Memory = jbox.cv_input{ ui_name = jbox.ui_text("text_MemoryCV") },
    CV_Transpose = jbox.cv_input{ ui_name = jbox.ui_text("text_TransposeCV") },
    CV_Harmony = jbox.cv_input{ ui_name = jbox.ui_text("text_HarmonyCV") },
    CV_Loop = jbox.cv_input{ ui_name = jbox.ui_text("text_LoopCV") },
    CV_Clear = jbox.cv_input{ ui_name = jbox.ui_text("text_ClearCV") },
    CV_LowLimit = jbox.cv_input{ ui_name = jbox.ui_text("text_LowLimitCV") },
    CV_HighLimit = jbox.cv_input{ ui_name = jbox.ui_text("text_HighLimitCV") },
    CV_Direction = jbox.cv_input{ ui_name = jbox.ui_text("text_DirectionCV") }
}

cv_outputs = {
    CV_Out_Note = jbox.cv_output{ ui_name = jbox.ui_text("text_CVNote_Main") },
    CV_Out_Gate = jbox.cv_output{ ui_name = jbox.ui_text("text_CVGate_Main") },
    CV_Out_AlterEgo_Note = jbox.cv_output{ ui_name = jbox.ui_text("text_CVNote_AlterEgo") },
    CV_Out_AlterEgo_Gate = jbox.cv_output{ ui_name = jbox.ui_text("text_CVGate_AlterEgo") }
}

midi_implementation_chart = {
    midi_cc_chart = {
        [12] = "/custom_properties/OnOff", [13] = "/custom_properties/Feedback", [14] = "/custom_properties/Freeze",
        [15] = "/custom_properties/KeyLock", [16] = "/custom_properties/ScaleRoot", [17] = "/custom_properties/ScaleMode",
        [18] = "/custom_properties/Voice", [19] = "/custom_properties/AlterEgo", [20] = "/custom_properties/Rate",
        [21] = "/custom_properties/Density", [22] = "/custom_properties/Velocity", [23] = "/custom_properties/Accent",
        [24] = "/custom_properties/Mutation", [25] = "/custom_properties/Octave", [26] = "/custom_properties/ResponseMode",
        [27] = "/custom_properties/Choke", [28] = "/custom_properties/Seed", [29] = "/custom_properties/SilenceThreshold",
        [30] = "/custom_properties/Patience", [31] = "/custom_properties/Memory",
        [35] = "/custom_properties/ResponseLength", [36] = "/custom_properties/Drift", [37] = "/custom_properties/Harmony",
        [44] = "/custom_properties/Sync", [45] = "/custom_properties/Loop", [46] = "/custom_properties/Length", [47] = "/custom_properties/Swing",
        [48] = "/custom_properties/KeepInput", [49] = "/custom_properties/Transpose"
    }
}

remote_implementation_chart = {
    ["/custom_properties/OnOff"] = { internal_name = "OnOff", short_ui_name = jbox.ui_text("text_short_OnOff"), shortest_ui_name = jbox.ui_text("text_shortest_OnOff") },
    ["/custom_properties/Feedback"] = { internal_name = "Feedback", short_ui_name = jbox.ui_text("text_short_Feedback"), shortest_ui_name = jbox.ui_text("text_shortest_Feedback") },
    ["/custom_properties/Freeze"] = { internal_name = "Freeze", short_ui_name = jbox.ui_text("text_short_Freeze"), shortest_ui_name = jbox.ui_text("text_shortest_Freeze") },
    ["/custom_properties/KeyLock"] = { internal_name = "KeyLock", short_ui_name = jbox.ui_text("text_short_KeyLock"), shortest_ui_name = jbox.ui_text("text_shortest_KeyLock") },
    ["/custom_properties/ScaleRoot"] = { internal_name = "ScaleRoot", short_ui_name = jbox.ui_text("text_short_ScaleRoot"), shortest_ui_name = jbox.ui_text("text_shortest_ScaleRoot") },
    ["/custom_properties/ScaleMode"] = { internal_name = "ScaleMode", short_ui_name = jbox.ui_text("text_short_ScaleMode"), shortest_ui_name = jbox.ui_text("text_shortest_ScaleMode") },
    ["/custom_properties/Voice"] = { internal_name = "Voice", short_ui_name = jbox.ui_text("text_short_Voice"), shortest_ui_name = jbox.ui_text("text_shortest_Voice") },
    ["/custom_properties/AlterEgo"] = { internal_name = "AlterEgo", short_ui_name = jbox.ui_text("text_short_AlterEgo"), shortest_ui_name = jbox.ui_text("text_shortest_AlterEgo") },
    ["/custom_properties/Rate"] = { internal_name = "Rate", short_ui_name = jbox.ui_text("text_short_Rate"), shortest_ui_name = jbox.ui_text("text_shortest_Rate") },
    ["/custom_properties/Density"] = { internal_name = "Density", short_ui_name = jbox.ui_text("text_short_Density"), shortest_ui_name = jbox.ui_text("text_shortest_Density") },
    ["/custom_properties/Length"] = { internal_name = "Length", short_ui_name = jbox.ui_text("text_short_Length"), shortest_ui_name = jbox.ui_text("text_shortest_Length") },
    ["/custom_properties/Swing"] = { internal_name = "Swing", short_ui_name = jbox.ui_text("text_short_Swing"), shortest_ui_name = jbox.ui_text("text_shortest_Swing") },
    ["/custom_properties/Velocity"] = { internal_name = "Velocity", short_ui_name = jbox.ui_text("text_short_Velocity"), shortest_ui_name = jbox.ui_text("text_shortest_Velocity") },
    ["/custom_properties/Accent"] = { internal_name = "Accent", short_ui_name = jbox.ui_text("text_short_Accent"), shortest_ui_name = jbox.ui_text("text_shortest_Accent") },
    ["/custom_properties/Mutation"] = { internal_name = "Mutation", short_ui_name = jbox.ui_text("text_short_Mutation"), shortest_ui_name = jbox.ui_text("text_shortest_Mutation") },
    ["/custom_properties/Octave"] = { internal_name = "Octave", short_ui_name = jbox.ui_text("text_short_Octave"), shortest_ui_name = jbox.ui_text("text_shortest_Octave") },
    ["/custom_properties/ResponseMode"] = { internal_name = "ResponseMode", short_ui_name = jbox.ui_text("text_short_ResponseMode"), shortest_ui_name = jbox.ui_text("text_shortest_ResponseMode") },
    ["/custom_properties/ResponseLength"] = { internal_name = "ResponseLength", short_ui_name = jbox.ui_text("text_short_ResponseLen"), shortest_ui_name = jbox.ui_text("text_shortest_ResponseLen") },
    ["/custom_properties/Drift"] = { internal_name = "Drift", short_ui_name = jbox.ui_text("text_short_Drift"), shortest_ui_name = jbox.ui_text("text_shortest_Drift") },
    ["/custom_properties/Harmony"] = { internal_name = "Harmony", short_ui_name = jbox.ui_text("text_short_Harmony"), shortest_ui_name = jbox.ui_text("text_shortest_Harmony") },
    ["/custom_properties/Choke"] = { internal_name = "Choke", short_ui_name = jbox.ui_text("text_short_Choke"), shortest_ui_name = jbox.ui_text("text_shortest_Choke") },
    ["/custom_properties/Seed"] = { internal_name = "Seed", short_ui_name = jbox.ui_text("text_short_Seed"), shortest_ui_name = jbox.ui_text("text_shortest_Seed") },
    ["/custom_properties/SilenceThreshold"] = { internal_name = "SilenceThreshold", short_ui_name = jbox.ui_text("text_short_SilenceThreshold"), shortest_ui_name = jbox.ui_text("text_shortest_SilenceThreshold") },
    ["/custom_properties/SilenceThreshold_Sync"] = { internal_name = "Silence_Sync", short_ui_name = jbox.ui_text("text_short_SilenceThreshold"), shortest_ui_name = jbox.ui_text("text_shortest_SilenceThreshold") },
    ["/custom_properties/Patience"] = { internal_name = "Patience", short_ui_name = jbox.ui_text("text_short_Patience"), shortest_ui_name = jbox.ui_text("text_shortest_Patience") },
    ["/custom_properties/Patience_Sync"] = { internal_name = "Patience_Sync", short_ui_name = jbox.ui_text("text_short_Patience"), shortest_ui_name = jbox.ui_text("text_shortest_Patience") },
    ["/custom_properties/Memory"] = { internal_name = "Memory", short_ui_name = jbox.ui_text("text_short_Memory"), shortest_ui_name = jbox.ui_text("text_shortest_Memory") },
    ["/custom_properties/Sync"] = { internal_name = "Sync", short_ui_name = jbox.ui_text("text_short_Sync"), shortest_ui_name = jbox.ui_text("text_shortest_Sync") },
    ["/custom_properties/Loop"] = { internal_name = "Loop", short_ui_name = jbox.ui_text("text_short_Loop"), shortest_ui_name = jbox.ui_text("text_shortest_Loop") },
    ["/custom_properties/KeepInput"] = { internal_name = "KeepInput", short_ui_name = jbox.ui_text("text_short_KeepInput"), shortest_ui_name = jbox.ui_text("text_shortest_KeepInput") },
    ["/custom_properties/Transpose"] = { internal_name = "Transpose", short_ui_name = jbox.ui_text("text_short_Transpose"), shortest_ui_name = jbox.ui_text("text_shortest_Transpose") },
    ["/custom_properties/Clear"] = { internal_name = "Clear", short_ui_name = jbox.ui_text("text_short_Clear"), shortest_ui_name = jbox.ui_text("text_shortest_Clear") },
    ["/custom_properties/LowLimit"] = { internal_name = "LowLimit", short_ui_name = jbox.ui_text("text_short_LowLimit"), shortest_ui_name = jbox.ui_text("text_shortest_LowLimit") },
    ["/custom_properties/HighLimit"] = { internal_name = "HighLimit", short_ui_name = jbox.ui_text("text_short_HighLimit"), shortest_ui_name = jbox.ui_text("text_shortest_HighLimit") },
    ["/custom_properties/Direction"] = { internal_name = "Direction", short_ui_name = jbox.ui_text("text_short_Direction"), shortest_ui_name = jbox.ui_text("text_shortest_Direction") }
}
