extends "res://addons/gut/test.gd"

class TestBank:
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
		var desired_value = Fmod.FMOD_STUDIO_LOADING_STATE_LOADED
		assert_eq(Fmod.get_bank_loading_state("res://assets/Banks/Vehicles.bank"), desired_value, "Loading state should be FMOD_STUDIO_LOADING_STATE_LOADED")
		Fmod.set_listener_number(1)
		get_tree().get_root().add_child(sprite)
		Fmod.add_listener(0, sprite)
	
	func after_all():
		Fmod.remove_listener(0)
		Fmod.unload_bank("res://assets/Banks/Master Bank.strings.bank")
		Fmod.unload_bank("res://assets/Banks/Master Bank.bank")
		Fmod.unload_bank("res://assets/Banks/Music.bank")
		Fmod.unload_bank("res://assets/Banks/Vehicles.bank")
		assert_eq(Fmod.get_bank_loading_state("res://assets/Banks/Vehicles.bank"), -1, "Loading state should be -1")
	
	func test_assert_bank_bus_count():
		var desiredValue: int = 0
		assert_eq(Fmod.get_bank_bus_count("res://assets/Banks/Music.bank"), desiredValue, "Music bank should have " + str(desiredValue) + " buses")
		desiredValue = 12
		assert_eq(Fmod.get_bank_bus_count("res://assets/Banks/Master Bank.bank"), desiredValue, "Master bank should have " + str(desiredValue) + " buses")
		desiredValue = 0
		assert_eq(Fmod.get_bank_bus_count("res://assets/Banks/Vehicles.bank"), desiredValue, "Vehicles bank should have " + str(desiredValue) + " buses")
	
	func test_assert_bank_event_count():
		var desiredValue: int = 4
		assert_eq(Fmod.get_bank_event_count("res://assets/Banks/Music.bank"), desiredValue, "Music bank should have " + str(desiredValue) + " events")
		desiredValue = 5
		assert_eq(Fmod.get_bank_event_count("res://assets/Banks/Master Bank.bank"), desiredValue, "Master bank should have " + str(desiredValue) + " events")
		desiredValue = 2
		assert_eq(Fmod.get_bank_event_count("res://assets/Banks/Vehicles.bank"), desiredValue, "Vehicles bank should have " + str(desiredValue) + " events")
	
	func test_assert_bank_string_count():
		var desiredValue: int = 0
		assert_eq(Fmod.get_bank_string_count("res://assets/Banks/Music.bank"), desiredValue, "Music bank should have " + str(desiredValue) + " strings")
		assert_eq(Fmod.get_bank_string_count("res://assets/Banks/Master Bank.bank"), desiredValue, "Master bank should have " + str(desiredValue) + " strings")
		assert_eq(Fmod.get_bank_string_count("res://assets/Banks/Vehicles.bank"), desiredValue, "Vehicles bank should have " + str(desiredValue) + " strings")
	
	func test_assert_bank_vca_count():
		var desiredValue: int = 0
		assert_eq(Fmod.get_bank_VCA_count("res://assets/Banks/Music.bank"), desiredValue, "Music bank should have " + str(desiredValue) + " VCAs")
		desiredValue = 3
		assert_eq(Fmod.get_bank_VCA_count("res://assets/Banks/Master Bank.bank"), desiredValue, "Master bank should have " + str(desiredValue) + " VCAs")
		desiredValue = 0
		assert_eq(Fmod.get_bank_VCA_count("res://assets/Banks/Vehicles.bank"), desiredValue, "Vehicles bank should have " + str(desiredValue) + " VCAs")
