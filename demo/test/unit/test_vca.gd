extends "res://addons/gut/test.gd"

class TestVCA:
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
		FmodServer.remove_listener(0, sprite)
	
	func test_assert_valid_paths():
		assert_true(FmodServer.check_vca_path("vca:/Environment"), "vca:/Environment should be present")
		assert_true(FmodServer.check_vca_path("vca:/Player"), "vca:/Player should be present")
		assert_true(FmodServer.check_vca_path("vca:/Equipment"), "vca:/Equipment should be present")
	
	func test_assert_invalid_path():
		assert_false(FmodServer.check_vca_path("vca:/undefined"), "Invalid vca should not be present")
	
	func test_assert_volume():
		_test_assert_volume(false)
		_test_assert_volume(true)
	
	func _test_assert_volume(is_guid: bool):
		var desired_value: float = 1.0
		var environment_vca = FmodServer.get_vca_from_guid("{3f0b7d64-e765-400e-ae74-c2d973ad4ca1}") if is_guid else FmodServer.get_vca("vca:/Environment")
		assert_eq(environment_vca.volume, desired_value, "VCA volume should be " + str(desired_value))
		desired_value = 0.5
		environment_vca.volume = desired_value
		assert_eq(environment_vca.volume, desired_value, "VCA volume should be " + str(desired_value))
		desired_value = 1.0
		environment_vca.volume = desired_value
