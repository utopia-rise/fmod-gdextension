extends "res://addons/gut/test.gd"

class TestVCA:
	extends "res://addons/gut/test.gd"
	
	var sprite: Sprite2D = Sprite2D.new()
	
	func before_all():
		# load banks
		# warning-ignore:return_value_discarded
		FmodServer.load_bank("res://assets/Banks/Master.strings.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		FmodServer.load_bank("res://assets/Banks/Master.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		FmodServer.load_bank("res://assets/Banks/Music.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		FmodServer.load_bank("res://assets/Banks/Vehicles.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		FmodServer.set_listener_number(1)
		get_tree().get_root().add_child(sprite)
		FmodServer.add_listener(0, sprite)
	
	func after_all():
		FmodServer.remove_listener(0)
		FmodServer.unload_bank("res://assets/Banks/Master.strings.bank")
		FmodServer.unload_bank("res://assets/Banks/Master.bank")
		FmodServer.unload_bank("res://assets/Banks/Music.bank")
		FmodServer.unload_bank("res://assets/Banks/Vehicles.bank")
	
	func test_assert_valid_paths():
		assert_true(FmodServer.check_VCA_path("vca:/Environment"), "vca:/Environment should be present")
		assert_true(FmodServer.check_VCA_path("vca:/Player"), "vca:/Player should be present")
		assert_true(FmodServer.check_VCA_path("vca:/Equipment"), "vca:/Equipment should be present")
	
	func test_assert_invalid_path():
		assert_false(FmodServer.check_VCA_path("vca:/undefined"), "Invalid vca should not be present")
	
	func test_assert_volume():
		var desired_value: float = 1.0
		var environment_vca = FmodServer.get_vca("vca:/Environment")
		assert_eq(environment_vca.volume, desired_value, "VCA volume should be " + str(desired_value))
		desired_value = 0.5
		environment_vca.volume = desired_value
		assert_eq(environment_vca.volume, desired_value, "VCA volume should be " + str(desired_value))
		desired_value = 1.0
		environment_vca.volume = desired_value
