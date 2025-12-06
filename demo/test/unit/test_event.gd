extends "res://addons/gut/test.gd"

class TestEvent:
	extends "res://addons/gut/test.gd"
	
	var fmodEvent: FmodEvent
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
		fmodEvent = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
		fmodEvent.set_parameter_by_name("RPM", 600)
		fmodEvent.start()
	
	func after_all():
		fmodEvent.stop(FmodServer.FMOD_STUDIO_STOP_IMMEDIATE)
		fmodEvent.release()
		FmodServer.remove_listener(0, sprite)
	
	func test_should_has_event():
		var wanted: String = "event:/Vehicles/Car Engine"
		assert_true(FmodServer.check_event_path(wanted), wanted + " should be present")
	
	func test_should_has_event_guid():
		var wanted: String = "{0c8363b4-23af-4f9c-af4b-0951bfd37d84}"
		assert_true(FmodServer.check_event_guid(wanted), wanted + " should be present")
	
	func test_should_not_has_event():
		var wanted: String = "undefined"
		assert_false(FmodServer.check_event_path(wanted), wanted + " should not be present")
	
	func test_should_not_has_event_guid():
		var wanted: String = "{29583fe9-eee9-4c67-94e1-57d5f6c552af}"
		assert_false(FmodServer.check_event_guid(wanted), wanted + " should not be present")
	
	func test_assert_set_volume():
		var desired_value: float = 4.0
		fmodEvent.volume = desired_value
		assert_eq(fmodEvent.volume, desired_value, "Event volume should be 4")
	
	func test_assert_set_pitch():
		var desired_value: float = 0.75
		fmodEvent.pitch = desired_value
		assert_eq(fmodEvent.pitch, desired_value, "Event pitch should be 0.75")
	
	func test_assert_paused():
		fmodEvent.paused = true
		assert_true(fmodEvent.paused, "Event should be paused")
	
	func test_assert_timeline_position():
		var desired_value: int = 10
		fmodEvent.paused = true
		fmodEvent.position = desired_value
		await wait_seconds(2)
		assert_eq(fmodEvent.position, desired_value, "Event timeline should be at " + str(desired_value))
	
	func test_assert_event_reverb():
		var desired_value: float = 1.5
		fmodEvent.set_reverb_level(0, desired_value)
		assert_eq(fmodEvent.get_reverb_level(0), desired_value, "Event reverb level should be " + str(desired_value))
	
	func test_assert_event_parameter_by_name():
		var desired_value: float = 600.0
		assert_eq(fmodEvent.get_parameter_by_name("RPM"), desired_value, "Event parameter RPM should be " + str(desired_value))
	
	func test_assert_should_pause_all():
		fmodEvent.paused = false
		var fmodEvent2 = FmodServer.create_event_instance_with_guid("{0c8363b4-23af-4f9c-af4b-0951bfd37d84}")
		fmodEvent2.start()
		FmodServer.pause_all_events()
		assert_true(fmodEvent.paused, "Event " + str(fmodEvent) + " should be paused")
		assert_true(fmodEvent2.paused, "Event " + str(fmodEvent2) + " should be paused")
		fmodEvent2.stop(FmodServer.FMOD_STUDIO_STOP_IMMEDIATE)
		fmodEvent2.release()
	
	func test_assert_should_mute_unmute_all():
		FmodServer.mute_all_events()
		
		var bus = FmodServer.get_bus("bus:/")
		
		assert_true(bus.mute, "Master bus should be muted")
		FmodServer.unmute_all_events()
		assert_false(bus.mute, "Master bus should not be muted")
