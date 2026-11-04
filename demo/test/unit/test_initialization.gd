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

func _runtimes() -> Array:
	return get_tree().root.get_children().filter(func(node: Node) -> bool: return node.get_class() == "FmodRuntime")

func test_assert_scripts_cannot_create_the_runtime_node():
	assert_bool(ClassDB.can_instantiate("FmodRuntime")).is_false()

func test_assert_init_adds_one_runtime_node_until_shutdown():
	FmodServer.shutdown()
	await await_idle_frame()
	assert_array(_runtimes()).is_empty()
	FmodServer.init()
	await await_idle_frame()
	assert_int(_runtimes().size()).is_equal(1)
	assert_int(_runtimes()[0].process_mode).is_equal(Node.PROCESS_MODE_ALWAYS)
	FmodServer.shutdown()
	await await_idle_frame()
	assert_array(_runtimes()).is_empty()
