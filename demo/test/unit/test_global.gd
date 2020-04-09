extends "res://addons/gut/test.gd"

class TestGlobal:
	extends "res://addons/gut/test.gd"
	
	var sprite: Sprite = Sprite.new()
	
	func before_all():
		# load banks
		# warning-ignore:return_value_discarded
		Fmod.load_bank("res://assets/Banks/Master Bank.strings.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		Fmod.load_bank("res://assets/Banks/Master Bank.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		Fmod.load_bank("res://assets/Banks/Music.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		Fmod.load_bank("res://assets/Banks/Vehicles.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		Fmod.set_listener_number(1)
		get_tree().get_root().add_child(sprite)
		Fmod.add_listener(0, sprite)
	
	func after_all():
		Fmod.remove_listener(0)
		Fmod.unload_bank("res://assets/Banks/Master Bank.strings.bank")
		Fmod.unload_bank("res://assets/Banks/Master Bank.bank")
		Fmod.unload_bank("res://assets/Banks/Music.bank")
		Fmod.unload_bank("res://assets/Banks/Vehicles.bank")
	
	func test_assert_should_have_cpu_performance_data():
		var perf_data: Dictionary = Fmod.get_performance_data()
		assert_contains_in_dict(perf_data, "CPU")
		var cpu_perf_data = perf_data["CPU"]
		assert_contains_in_dict(cpu_perf_data, "dsp")
		assert_contains_in_dict(cpu_perf_data, "geometry")
		assert_contains_in_dict(cpu_perf_data, "stream")
		assert_contains_in_dict(cpu_perf_data, "studio")
		assert_contains_in_dict(cpu_perf_data, "update")
	
	func test_assert_should_have_memory_performance_data():
		var perf_data: Dictionary = Fmod.get_performance_data()
		assert_contains_in_dict(perf_data, "memory")
		var mem_perf_data = perf_data["memory"]
		assert_contains_in_dict(mem_perf_data, "currently_allocated")
		assert_contains_in_dict(mem_perf_data, "max_allocated")
	
	func test_assert_should_have_file_performance_data():
		var perf_data: Dictionary = Fmod.get_performance_data()
		assert_contains_in_dict(perf_data, "file")
		var file_perf_data = perf_data["file"]
		assert_contains_in_dict(file_perf_data, "sample_bytes_read")
		assert_contains_in_dict(file_perf_data, "stream_bytes_read")
		assert_contains_in_dict(file_perf_data, "other_bytes_read")
	
	func assert_contains_in_dict(dict: Dictionary, key: String):
		assert_has(dict, key, "Performance data should contains " + key + " data")
