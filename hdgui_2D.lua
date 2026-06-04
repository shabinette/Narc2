format_version = "2.0"
panel_type = "note_player"

front = jbox.panel{
    graphics = { node = "Bg" },
    widgets = {
        jbox.device_name{ graphics = { node = "DeviceName" } },
        jbox.patch_name{ graphics = { node = "PatchName" }, text_style = "LCD font", fg_color = { 255, 255, 255 }, loader_alt_color = { 0, 150, 255 }, center = true },
        jbox.patch_browse_group{ graphics = { node = "PatchBrowser" } },
        jbox.toggle_button{ graphics = { node = "OnOff" }, value = "/custom_properties/OnOff" },
        jbox.sequence_meter{ graphics = { node = "NoteLamp" }, value = "/custom_properties/NoteOnIndicator" },

        jbox.toggle_button{ graphics = { node = "KeyLock" }, value = "/custom_properties/KeyLock" },
        jbox.popup_button{ graphics = { node = "RootNameDisplay" }, value = "/custom_properties/ScaleRoot", text_style = "LCD font", text_color = { 0, 150, 255 } },
        jbox.popup_button{ graphics = { node = "ModeNameDisplay" }, value = "/custom_properties/ScaleMode", text_style = "LCD font", text_color = { 0, 150, 255 } },

        jbox.toggle_button{ graphics = { node = "Feedback" }, value = "/custom_properties/Feedback" },
        jbox.toggle_button{ graphics = { node = "Freeze" }, value = "/custom_properties/Freeze" },

        jbox.analog_knob{ graphics = { node = "Knob_Rate" }, value = "/custom_properties/Rate" },
        jbox.analog_knob{ graphics = { node = "Knob_Velocity" }, value = "/custom_properties/Velocity" },
        jbox.analog_knob{ graphics = { node = "Knob_Density" }, value = "/custom_properties/Density" },
        jbox.analog_knob{ graphics = { node = "Knob_Length" }, value = "/custom_properties/Length" },
        jbox.analog_knob{ graphics = { node = "Knob_Swing" }, value = "/custom_properties/Swing" },
        jbox.analog_knob{ graphics = { node = "Knob_Accent" }, value = "/custom_properties/Accent" },

        jbox.analog_knob{ graphics = { node = "Knob_Mutation" }, value = "/custom_properties/Mutation" },
        jbox.analog_knob{ graphics = { node = "Knob_Octave" }, value = "/custom_properties/Octave" },

        jbox.analog_knob{ graphics = { node = "GhostNode_Knob" }, value = "/custom_properties/AlterEgo", visibility_switch = "/custom_properties/GhostHide", visibility_values = { 1 } },
        jbox.popup_button{ graphics = { node = "GhostNode_Display" }, value = "/custom_properties/AlterEgo", text_style = "LCD font", text_color = { 0, 150, 255 }, visibility_switch = "/custom_properties/GhostHide", visibility_values = { 1 } },

        jbox.analog_knob{ graphics = { node = "Knob_Drift" }, value = "/custom_properties/Drift" },
        jbox.analog_knob{ graphics = { node = "Knob_Transpose" }, value = "/custom_properties/Transpose" },
        jbox.toggle_button{ graphics = { node = "Switch_KeepInput" }, value = "/custom_properties/KeepInput" },
        
        jbox.analog_knob{ graphics = { node = "Knob_Loop" }, value = "/custom_properties/Loop" },
        jbox.analog_knob{ graphics = { node = "Knob_ResponseLength" }, value = "/custom_properties/ResponseLength" },
        jbox.analog_knob{ graphics = { node = "Knob_Harmony" }, value = "/custom_properties/Harmony" },
        
        jbox.analog_knob{ graphics = { node = "Knob_ResponseMode" }, value = "/custom_properties/ResponseMode" },
        jbox.popup_button{ graphics = { node = "ResponseModeDisplay" }, value = "/custom_properties/ResponseMode", text_style = "LCD font", text_color = { 0, 150, 255 } },
        jbox.toggle_button{ graphics = { node = "Switch_Choke" }, value = "/custom_properties/Choke" },
        jbox.momentary_button{ graphics = { node = "Button_Seed" }, value = "/custom_properties/Seed" },
        jbox.momentary_button{ graphics = { node = "Button_Clear" }, value = "/custom_properties/Clear" },
        
        jbox.analog_knob{ graphics = { node = "Knob_SilenceThreshold" }, value = "/custom_properties/SilenceThreshold", visibility_switch = "/custom_properties/Sync", visibility_values = { 0 } },
        jbox.analog_knob{ graphics = { node = "Knob_SilenceThreshold_Sync" }, value = "/custom_properties/SilenceThreshold_Sync", visibility_switch = "/custom_properties/Sync", visibility_values = { 1 } },
        
        jbox.analog_knob{ graphics = { node = "Knob_Patience" }, value = "/custom_properties/Patience", visibility_switch = "/custom_properties/Sync", visibility_values = { 0 } },
        jbox.analog_knob{ graphics = { node = "Knob_Patience_Sync" }, value = "/custom_properties/Patience_Sync", visibility_switch = "/custom_properties/Sync", visibility_values = { 1 } },

        jbox.toggle_button{ graphics = { node = "Switch_Sync" }, value = "/custom_properties/Sync" },
        jbox.analog_knob{ graphics = { node = "Knob_Memory" }, value = "/custom_properties/Memory" },

        jbox.custom_display{
            graphics = { node = "ReflectionPool" },
            display_width_pixels = 1395,
            display_height_pixels = 555,
            invalidate_function = "ReflectionPool_Invalidate",
            draw_function = "ReflectionPool_Draw",
            values = { "/custom_properties/ReflectionDisplayData" },
            background = jbox.image{ path = "Pool" }
        },
        jbox.static_decoration{ graphics = { node = "ReflectionBevil" } },

        -- CONTROL SECTION (below display)
        jbox.analog_knob{ graphics = { node = "Knob_LowLimit" },  value = "/custom_properties/LowLimit" },
        jbox.popup_button{ graphics = { node = "LowLimitDisplay" }, value = "/custom_properties/LowLimit", text_style = "LCD font", text_color = { 0, 150, 255 } },
        jbox.toggle_button{ graphics = { node = "Switch_Direction" }, value = "/custom_properties/Direction" },
        jbox.analog_knob{ graphics = { node = "Knob_HighLimit" }, value = "/custom_properties/HighLimit" },
        jbox.popup_button{ graphics = { node = "HighLimitDisplay" }, value = "/custom_properties/HighLimit", text_style = "LCD font", text_color = { 0, 150, 255 } }
    }
}

back = jbox.panel{
    graphics = { node = "Bg" },
    widgets = {
        jbox.device_name{ graphics = { node = "DeviceName" } },
        jbox.placeholder{ graphics = { node = "Placeholder" } },
        
        jbox.analog_knob{ graphics = { node = "Knob_AlterEgo" }, value = "/custom_properties/AlterEgo" },
        jbox.popup_button{ graphics = { node = "AlterEgoNameDisplay" }, value = "/custom_properties/AlterEgo", text_style = "LCD font", text_color = { 0, 150, 255 } },

        jbox.cv_input_socket{ graphics = { node = "Jack_Feedback" }, socket = "/cv_inputs/CV_Feedback" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Feedback" }, socket = "/cv_inputs/CV_Feedback" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Freeze" }, socket = "/cv_inputs/CV_Freeze" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Freeze" }, socket = "/cv_inputs/CV_Freeze" },
        
        jbox.cv_input_socket{ graphics = { node = "Jack_Rate" }, socket = "/cv_inputs/CV_Rate" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Rate" }, socket = "/cv_inputs/CV_Rate" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Density" }, socket = "/cv_inputs/CV_Density" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Density" }, socket = "/cv_inputs/CV_Density" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Swing" }, socket = "/cv_inputs/CV_Swing" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Swing" }, socket = "/cv_inputs/CV_Swing" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Mutation" }, socket = "/cv_inputs/CV_Mutation" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Mutation" }, socket = "/cv_inputs/CV_Mutation" },

        jbox.cv_input_socket{ graphics = { node = "Jack_Velocity" }, socket = "/cv_inputs/CV_Velocity" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Velocity" }, socket = "/cv_inputs/CV_Velocity" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Length" }, socket = "/cv_inputs/CV_Length" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Length" }, socket = "/cv_inputs/CV_Length" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Accent" }, socket = "/cv_inputs/CV_Accent" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Accent" }, socket = "/cv_inputs/CV_Accent" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Octave" }, socket = "/cv_inputs/CV_Octave" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Octave" }, socket = "/cv_inputs/CV_Octave" },
        
        jbox.cv_input_socket{ graphics = { node = "Jack_Response" }, socket = "/cv_inputs/CV_Response" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Response" }, socket = "/cv_inputs/CV_Response" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Choke" }, socket = "/cv_inputs/CV_Choke" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Choke" }, socket = "/cv_inputs/CV_Choke" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Seed" }, socket = "/cv_inputs/CV_Seed" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Seed" }, socket = "/cv_inputs/CV_Seed" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Transpose" }, socket = "/cv_inputs/CV_Transpose" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Transpose" }, socket = "/cv_inputs/CV_Transpose" },

        jbox.cv_input_socket{ graphics = { node = "Jack_SilenceThreshold" }, socket = "/cv_inputs/CV_SilenceThreshold" }, jbox.cv_trim_knob{ graphics = { node = "Trim_SilenceThreshold" }, socket = "/cv_inputs/CV_SilenceThreshold" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Patience" }, socket = "/cv_inputs/CV_Patience" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Patience" }, socket = "/cv_inputs/CV_Patience" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Memory" }, socket = "/cv_inputs/CV_Memory" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Memory" }, socket = "/cv_inputs/CV_Memory" },

        jbox.cv_input_socket{ graphics = { node = "Jack_Harmony" }, socket = "/cv_inputs/CV_Harmony" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Harmony" }, socket = "/cv_inputs/CV_Harmony" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Loop" }, socket = "/cv_inputs/CV_Loop" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Loop" }, socket = "/cv_inputs/CV_Loop" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Clear" }, socket = "/cv_inputs/CV_Clear" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Clear" }, socket = "/cv_inputs/CV_Clear" },
        jbox.cv_input_socket{ graphics = { node = "Jack_LowLimit" }, socket = "/cv_inputs/CV_LowLimit" }, jbox.cv_trim_knob{ graphics = { node = "Trim_LowLimit" }, socket = "/cv_inputs/CV_LowLimit" },
        jbox.cv_input_socket{ graphics = { node = "Jack_HighLimit" }, socket = "/cv_inputs/CV_HighLimit" }, jbox.cv_trim_knob{ graphics = { node = "Trim_HighLimit" }, socket = "/cv_inputs/CV_HighLimit" },
        jbox.cv_input_socket{ graphics = { node = "Jack_Direction" }, socket = "/cv_inputs/CV_Direction" }, jbox.cv_trim_knob{ graphics = { node = "Trim_Direction" }, socket = "/cv_inputs/CV_Direction" },

        jbox.cv_output_socket{ graphics = { node = "Jack_Out_Note" }, socket = "/cv_outputs/CV_Out_Note" },
        jbox.cv_output_socket{ graphics = { node = "Jack_Out_Gate" }, socket = "/cv_outputs/CV_Out_Gate" },
        jbox.cv_output_socket{ graphics = { node = "Jack_Out_AlterEgo_Note" }, socket = "/cv_outputs/CV_Out_AlterEgo_Note" },
        jbox.cv_output_socket{ graphics = { node = "Jack_Out_AlterEgo_Gate" }, socket = "/cv_outputs/CV_Out_AlterEgo_Gate" }
    }
}
