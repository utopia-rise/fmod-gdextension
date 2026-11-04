extends GdUnitTestSuite

static func _is_desktop() -> bool:
	return not OS.has_feature("mobile") and not OS.has_feature("web")

static func _is_web(threads: bool) -> bool:
	return OS.has_feature("web") and OS.has_feature("threads" if threads else "nothreads")

func _assert_platform_defaults(channel_count: int, buffer_size: int, buffer_count: int, processing: FmodSettings.StudioProcessing, live_update: bool):
	var settings := FmodSettings.new()
	assert_int(settings.channel_count).is_equal(channel_count)
	assert_int(settings.dsp_buffer_size).is_equal(buffer_size)
	assert_int(settings.dsp_buffer_count).is_equal(buffer_count)
	assert_int(settings.studio_processing).is_equal(processing)
	assert_bool(settings.live_update).is_equal(live_update)

func test_assert_common_defaults():
	var settings := FmodSettings.new()
	assert_str(settings.banks_path).is_equal("res://")
	assert_bool(settings.load_by_name).is_false()
	assert_int(settings.sample_rate).is_equal(48000)
	assert_int(settings.speaker_mode).is_equal(FmodSettings.SpeakerMode.STEREO)
	assert_int(settings.raw_speaker_count).is_equal(0)
	assert_int(settings.output_routing).is_equal(FmodSettings.OutputRouting.DEVICE)
	assert_str(settings.godot_audio_bus).is_equal("Master")
	assert_int(settings.godot_bus_mixing).is_equal(FmodSettings.GodotBusMixing.MIX_AHEAD)
	assert_bool(settings.sync_to_godot_mix_rate).is_false()
	assert_int(settings.listener_count).is_equal(1)
	assert_float(settings.distance_factor).is_equal(1.0)
	assert_float(settings.rolloff_scale).is_equal(1.0)
	assert_float(settings.doppler_scale).is_equal(1.0)
	assert_bool(settings.memory_tracking).is_false()
	assert_int(settings.log_level).is_equal(FmodSettings.LogLevel.LOG_LEVEL_INHERIT)
	assert_int(settings.log_output).is_equal(FmodSettings.LogOutput.LOG_OUTPUT_GODOT)
	assert_str(settings.log_file_path).is_equal("user://fmod.log")
	assert_object(settings.plugins).is_null()

func test_assert_desktop_defaults(do_skip := not _is_desktop(), skip_reason := "Desktop only"):
	_assert_platform_defaults(1024, 512, 4, FmodSettings.StudioProcessing.BACKGROUND_THREAD, true)

func test_assert_mobile_defaults(do_skip := not OS.has_feature("mobile"), skip_reason := "Mobile only"):
	_assert_platform_defaults(512, 1024, 4, FmodSettings.StudioProcessing.BACKGROUND_THREAD, true)

func test_assert_web_defaults(do_skip := not _is_web(false), skip_reason := "Single-threaded web only"):
	_assert_platform_defaults(512, 2048, 2, FmodSettings.StudioProcessing.MAIN_THREAD, false)

func test_assert_web_with_threads_defaults(do_skip := not _is_web(true), skip_reason := "Threaded web only"):
	_assert_platform_defaults(512, 1024, 4, FmodSettings.StudioProcessing.BACKGROUND_THREAD, false)
