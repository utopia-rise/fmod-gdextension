extends "res://addons/gut/test.gd"

class TestGlobal:
	extends "res://addons/gut/test.gd"
	
	var sprite: Sprite2D = Sprite2D.new()
	
	var banks := Array()
	
	func before_all():
		# load banks
		# warning-ignore:return_value_discarded
		banks.append(
			FmodServer.load_bank("res://assets/Banks/Master.strings.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		)
		# warning-ignore:return_value_discarded
		banks.append(
			FmodServer.load_bank("res://assets/Banks/Master.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		)
		# warning-ignore:return_value_discarded
		banks.append(
			FmodServer.load_bank("res://assets/Banks/Music.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		)
		# warning-ignore:return_value_discarded
		banks.append(
			FmodServer.load_bank("res://assets/Banks/Vehicles.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		)
		FmodServer.set_listener_number(1)
		get_tree().get_root().add_child(sprite)
		FmodServer.add_listener(0, sprite)
	
	func after_all():
		FmodServer.remove_listener(0)
	
	func test_assert_should_have_performance_data():
		var perf_data: FmodPerformanceData = FmodServer.get_performance_data()
		assert_not_null(perf_data, "Performance data should not be null.")
		
	func test_assert_should_have_dsp_buffer_length():
		var buffer_length = FmodServer.get_system_dsp_buffer_length()
		assert_eq(buffer_length, 512)
		
	func test_assert_should_have_dsp_num_buffers():
		var num_buffers = FmodServer.get_system_dsp_num_buffers()
		assert_eq(num_buffers, 4)

	func test_assert_should_have_dsp_buffer_size():
		var buffer_size: FmodDspSettings = FmodServer.get_system_dsp_buffer_settings()
		assert_eq(buffer_size.dsp_buffer_size, 512)
		assert_eq(buffer_size.dsp_buffer_count, 4)
