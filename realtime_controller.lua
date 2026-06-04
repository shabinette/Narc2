format_version = "1.0"

rtc_bindings = {
    {
        source = "/environment/system_sample_rate",
        dest = "/global_rtc/new_instance"
    }
}

global_rtc = {
    new_instance = function(source_path, new_sample_rate, old_sample_rate)
        -- We pass the 'new_sample_rate' in the table here so iCount == 1 in C++
        local new_no = jbox.make_native_object_rw("Instance", { new_sample_rate })
        jbox.store_property("/custom_properties/instance", new_no)
    end
}

sample_rate_setup = {
    native = { 22050, 44100, 48000, 88200, 96000, 192000 },
    converted = {}
}

rt_input_setup = {
    notify = {
        "/custom_properties/*",
        "/transport/*",
        "/note_states/*",
        "/environment/player_bypassed" -- Required to detect when the host bypasses us
    }
}
