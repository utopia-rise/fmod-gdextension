extends "res://addons/gut/test.gd"

class TestEvent:
	extends "res://addons/gut/test.gd"
	
	var fmodEvent: FmodEvent
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
		fmodEvent = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
		fmodEvent.set_parameter_by_name("RPM", 600)
		fmodEvent.start()
	
	func after_all():
		fmodEvent.stop(FmodServer.FMOD_STUDIO_STOP_IMMEDIATE)
		fmodEvent.release()
		FmodServer.remove_listener(0)
		FmodServer.unload_bank("res://assets/Banks/Master.strings.bank")
		FmodServer.unload_bank("res://assets/Banks/Master.bank")
		FmodServer.unload_bank("res://assets/Banks/Music.bank")
		FmodServer.unload_bank("res://assets/Banks/Vehicles.bank")
	
	func test_should_has_event():
		var wanted: String = "event:/Vehicles/Car Engine"
		assert_true(FmodServer.check_event_path(wanted), wanted + " should be present")
	
	func test_should_not_has_event():
		var wanted: String = "undefined"
		assert_false(FmodServer.check_event_path(wanted), wanted + " should not be present")
	
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
		var fmodEvent2 = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
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

#TODO: reimplement when emitters are ok.
#	func test_assert_attached_to_node():
#		FmodServer.attach_instance_to_node(id, sprite)
#		var node_instance: Object = FmodServer.get_object_attached_to_instance(id)
#		var node_instance_id: int = node_instance.get_instance_id()
#		assert_false(node_instance == null, "Instance " + str(id) + " should be attached to Node")
#		var id2: int = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
#		var object: Object = FmodServer.get_object_attached_to_instance(id2)
#		assert_true(object == null, "Instance " + str(id2) + " should not be attached to any Node")
#		FmodServer.attach_instance_to_node(id2, sprite)
#		var object_instance_id = FmodServer.get_object_attached_to_instance(id2).get_instance_id()
#		assert_true(node_instance_id == object_instance_id, "Both instances should be attached to same Node")
#		FmodServer.detach_instance_from_node(id2)
#		object = FmodServer.get_object_attached_to_instance(id2)
#		assert_true(object == null, "Instance " + str(id2) + " should be detached")
#		FmodServer.release_event(id2)
#		FmodServer.detach_instance_from_node(id)
