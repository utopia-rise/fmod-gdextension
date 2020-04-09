extends "res://addons/gut/test.gd"

class TestCallbacks:
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
	
	func test_assert_has_signals():
		assert_has_signal(Fmod, "timeline_beat")
		assert_has_signal(Fmod, "timeline_marker")
		assert_has_signal(Fmod, "sound_played")
		assert_has_signal(Fmod, "sound_stopped")
	
	func test_assert_set_callback():
		watch_signals(Fmod)
		var id: int = Fmod.create_event_instance("event:/Music/Level 02")
		Fmod.set_callback(id, Fmod.FMOD_STUDIO_EVENT_CALLBACK_SOUND_STOPPED)
		Fmod.set_event_volume(id, 0)
		Fmod.start_event(id)
		yield(yield_for(2), YIELD)
		assert_signal_not_emitted(Fmod, "timeline_beat", "Fmod should not have emitted timeline_beat signal")
		Fmod.set_callback(id, Fmod.FMOD_STUDIO_EVENT_CALLBACK_ALL)
		yield(yield_to(Fmod, "timeline_beat", 3), YIELD)
		assert_signal_emitted(Fmod, "timeline_beat", "Fmod should have emitted timeline_beat signal")
		Fmod.stop_event(id, Fmod.FMOD_STUDIO_STOP_IMMEDIATE)
		Fmod.release_event(id)
