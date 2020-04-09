extends "res://addons/gut/test.gd"

class TestVCA:
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
	
	func test_assert_valid_paths():
		assert_true(Fmod.check_VCA_path("vca:/Environment"), "vca:/Environment should be present")
		assert_true(Fmod.check_VCA_path("vca:/Player"), "vca:/Player should be present")
		assert_true(Fmod.check_VCA_path("vca:/Equipment"), "vca:/Equipment should be present")
	
	func test_assert_invalid_path():
		assert_false(Fmod.check_VCA_path("vca:/undefined"), "Invalid vca should not be present")
	
	func test_assert_volume():
		var desired_value: float = 1.0
		assert_eq(Fmod.get_VCA_volume("vca:/Environment"), desired_value, "VCA volume should be " + str(desired_value))
		desired_value = 0.5
		Fmod.set_VCA_volume("vca:/Environment", desired_value)
		assert_eq(Fmod.get_VCA_volume("vca:/Environment"), desired_value, "VCA volume should be " + str(desired_value))
		desired_value = 1.0
		Fmod.set_VCA_volume("vca:/Environment", desired_value)
