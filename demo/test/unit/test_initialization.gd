extends GdUnitTestSuite

func after():
	FmodServer.shutdown()
	FmodServer.init()

func test_assert_init_without_settings_uses_project_settings():
	FmodServer.shutdown()
	FmodServer.init()
	var defaults := FmodSettings.new()
	assert_int(FmodServer.get_system_dsp_buffer_length()).is_equal(ProjectSettings.get_setting("Fmod/Mixer/latency/buffer_size", defaults.dsp_buffer_size))
	assert_int(FmodServer.get_system_dsp_num_buffers()).is_equal(ProjectSettings.get_setting("Fmod/Mixer/latency/buffer_count", defaults.dsp_buffer_count))

func test_assert_init_applies_its_settings():
	var settings := FmodSettings.new()
	settings.dsp_buffer_size = 1024
	settings.dsp_buffer_count = 6
	FmodServer.shutdown()
	FmodServer.init(settings)
	assert_int(FmodServer.get_system_dsp_buffer_length()).is_equal(1024)
	assert_int(FmodServer.get_system_dsp_num_buffers()).is_equal(6)
