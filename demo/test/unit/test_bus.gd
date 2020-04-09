extends "res://addons/gut/test.gd"

class TestBus:
	extends "res://addons/gut/test.gd"
	
	var id: int
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
		id = Fmod.create_event_instance("event:/Vehicles/Car Engine")
		get_tree().get_root().add_child(sprite)
		Fmod.add_listener(0, sprite)
	
	func after_all():
		Fmod.release_event(id)
		Fmod.remove_listener(0)
		Fmod.unload_bank("res://assets/Banks/Master Bank.strings.bank")
		Fmod.unload_bank("res://assets/Banks/Master Bank.bank")
		Fmod.unload_bank("res://assets/Banks/Music.bank")
		Fmod.unload_bank("res://assets/Banks/Vehicles.bank")
	
	func test_assert_should_has_master_bus():
		var wanted: String = "bus:/"
		assert_true(Fmod.check_bus_path(wanted), wanted + " should be present")
	
	func test_assert_should_not_has_bus():
		var wanted: String = "undefined"
		assert_false(Fmod.check_bus_path(wanted), wanted + " should not be present")
	
	func test_assert_mute_unmute():
		assert_false(Fmod.get_bus_mute("bus:/"), "Master bus should not be muted")
		Fmod.set_bus_mute("bus:/", true)
		assert_true(Fmod.get_bus_mute("bus:/"), "Master bus should be muted")
		Fmod.set_bus_mute("bus:/", false)
		assert_false(Fmod.get_bus_mute("bus:/"), "Master bus should not be muted")
	
	func test_assert_pause_unpause():
		assert_false(Fmod.get_bus_paused("bus:/"), "Master bus should not be paused")
		Fmod.set_bus_paused("bus:/", true)
		assert_true(Fmod.get_bus_paused("bus:/"), "Master bus should be paused")
		Fmod.set_bus_paused("bus:/", false)
		assert_false(Fmod.get_bus_paused("bus:/"), "Master bus should not be paused")
	
	func test_assert_volume():
		var desiredValue: float = 1.0
		assert_eq(Fmod.get_bus_volume("bus:/"), desiredValue, "Bus volume should be " + str(desiredValue))
		desiredValue = 0.5
		Fmod.set_bus_volume("bus:/", desiredValue)
		assert_eq(Fmod.get_bus_volume("bus:/"), desiredValue, "Bus volume should be " + str(desiredValue))
		Fmod.set_bus_volume("bus:/", 1)
	
	func test_assert_bus_stop_events():
		var id2: int = Fmod.create_event_instance("event:/Vehicles/Car Engine")
		Fmod.start_event(id)
		Fmod.start_event(id2)
		yield(yield_for(2), YIELD)
		assert_eq(Fmod.get_event_playback_state(id), Fmod.FMOD_STUDIO_PLAYBACK_PLAYING, "Event " + str(id) + " playback state should be " + str(Fmod.FMOD_STUDIO_PLAYBACK_PLAYING))
		assert_eq(Fmod.get_event_playback_state(id2), Fmod.FMOD_STUDIO_PLAYBACK_PLAYING, "Event " + str(id2) + " playback state should be " + str(Fmod.FMOD_STUDIO_PLAYBACK_PLAYING))
		Fmod.stop_all_bus_events("bus:/", Fmod.FMOD_STUDIO_STOP_IMMEDIATE)
		yield(yield_for(2), YIELD)
		assert_eq(Fmod.get_event_playback_state(id), Fmod.FMOD_STUDIO_PLAYBACK_STOPPED, "Event " + str(id) + " playback state should be " + str(Fmod.FMOD_STUDIO_PLAYBACK_STOPPED))
		assert_eq(Fmod.get_event_playback_state(id2), Fmod.FMOD_STUDIO_PLAYBACK_STOPPED, "Event " + str(id2) + " playback state should be " + str(Fmod.FMOD_STUDIO_PLAYBACK_STOPPED))
		Fmod.release_event(id2)
