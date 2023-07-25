extends "res://addons/gut/test.gd"

class TestBank:
	extends "res://addons/gut/test.gd"
	
	var sprite: Sprite2D = Sprite2D.new()
	
	var masterBank: FmodBank
	var musicBank: FmodBank
	var vehicleBank: FmodBank
	
	func before_all():
		# load banks
		# warning-ignore:return_value_discarded
		FmodServer.load_bank("res://assets/Banks/Master.strings.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		masterBank = FmodServer.load_bank("res://assets/Banks/Master.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		musicBank = FmodServer.load_bank("res://assets/Banks/Music.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		vehicleBank = FmodServer.load_bank("res://assets/Banks/Vehicles.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		var desired_value = FmodServer.FMOD_STUDIO_LOADING_STATE_LOADED
		assert_eq(vehicleBank.get_loading_state(), desired_value, "Loading state should be FMOD_STUDIO_LOADING_STATE_LOADED")
		FmodServer.set_listener_number(1)
		get_tree().get_root().add_child(sprite)
		FmodServer.add_listener(0, sprite)
	
	func after_all():
		FmodServer.remove_listener(0)
		FmodServer.unload_bank("res://assets/Banks/Master.strings.bank")
		FmodServer.unload_bank("res://assets/Banks/Master.bank")
		FmodServer.unload_bank("res://assets/Banks/Music.bank")
		FmodServer.unload_bank("res://assets/Banks/Vehicles.bank")
		assert_eq(vehicleBank.get_loading_state(), -1, "Loading state should be -1")
	
	func test_assert_bank_bus_count():
		var desiredValue: int = 0
		assert_eq(musicBank.get_bus_count(), desiredValue, "Music bank should have " + str(desiredValue) + " buses")
		desiredValue = 12
		assert_eq(masterBank.get_bus_count(), desiredValue, "Master bank should have " + str(desiredValue) + " buses")
		desiredValue = 0
		assert_eq(vehicleBank.get_bus_count(), desiredValue, "Vehicles bank should have " + str(desiredValue) + " buses")
	
	func test_assert_bank_event_count():
		var desiredValue: int = 4
		assert_eq(musicBank.get_event_description_count(), desiredValue, "Music bank should have " + str(desiredValue) + " events")
		desiredValue = 5
		assert_eq(masterBank.get_event_description_count(), desiredValue, "Master bank should have " + str(desiredValue) + " events")
		desiredValue = 2
		assert_eq(vehicleBank.get_event_description_count(), desiredValue, "Vehicles bank should have " + str(desiredValue) + " events")
	
	func test_assert_bank_string_count():
		var desiredValue: int = 0
		assert_eq(musicBank.get_string_count(), desiredValue, "Music bank should have " + str(desiredValue) + " strings")
		assert_eq(masterBank.get_string_count(), desiredValue, "Master bank should have " + str(desiredValue) + " strings")
		assert_eq(vehicleBank.get_string_count(), desiredValue, "Vehicles bank should have " + str(desiredValue) + " strings")
	
	func test_assert_bank_vca_count():
		var desiredValue: int = 0
		assert_eq(musicBank.get_VCA_count(), desiredValue, "Music bank should have " + str(desiredValue) + " VCAs")
		desiredValue = 3
		assert_eq(masterBank.get_VCA_count(), desiredValue, "Master bank should have " + str(desiredValue) + " VCAs")
		desiredValue = 0
		assert_eq(vehicleBank.get_VCA_count(), desiredValue, "Vehicles bank should have " + str(desiredValue) + " VCAs")
