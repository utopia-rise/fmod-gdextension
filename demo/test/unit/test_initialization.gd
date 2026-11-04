extends GdUnitTestSuite

func after():
	FmodServer.shutdown()
	FmodServer.init()

func test_assert_init_without_settings_uses_project_settings():
	FmodServer.shutdown()
	FmodServer.init()
	var defaults := FmodDspSettings.new()
	assert_int(FmodServer.get_system_dsp_buffer_length()).is_equal(ProjectSettings.get_setting("Fmod/DSP/dsp_buffer_size", defaults.dsp_buffer_size))
	assert_int(FmodServer.get_system_dsp_num_buffers()).is_equal(ProjectSettings.get_setting("Fmod/DSP/dsp_buffer_count", defaults.dsp_buffer_count))

func test_assert_init_applies_the_settings_it_holds():
	var settings := FmodGeneralSettings.new()
	settings.dsp = FmodDspSettings.new()
	settings.dsp.dsp_buffer_size = 1024
	FmodServer.shutdown()
	FmodServer.init(settings)
	assert_int(FmodServer.get_system_dsp_buffer_length()).is_equal(1024)

func test_assert_init_keeps_what_its_empty_settings_leave():
	var dsp := FmodDspSettings.new()
	dsp.dsp_buffer_size = 2048
	FmodServer.shutdown()
	FmodServer.set_software_format(FmodSoftwareFormatSettings.new())
	FmodServer.set_system_dsp_buffer_size(dsp)
	FmodServer.init(FmodGeneralSettings.new())
	assert_int(FmodServer.get_system_dsp_buffer_length()).is_equal(2048)
