extends "res://addons/gut/test.gd"

class TestEvent:
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
		get_tree().get_root().add_child(sprite)
		Fmod.add_listener(0, sprite)
		id = Fmod.create_event_instance("event:/Vehicles/Car Engine")
		Fmod.set_event_parameter_by_name(id, "RPM", 600)
		Fmod.start_event(id)
	
	func after_all():
		Fmod.stop_event(id, Fmod.FMOD_STUDIO_STOP_IMMEDIATE)
		Fmod.release_event(id)
		Fmod.remove_listener(0)
		Fmod.unload_bank("res://assets/Banks/Master Bank.strings.bank")
		Fmod.unload_bank("res://assets/Banks/Master Bank.bank")
		Fmod.unload_bank("res://assets/Banks/Music.bank")
		Fmod.unload_bank("res://assets/Banks/Vehicles.bank")
	
	func test_should_has_event():
		var wanted: String = "event:/Vehicles/Car Engine"
		assert_true(Fmod.check_event_path(wanted), wanted + " should be present")
	
	func test_should_not_has_event():
		var wanted: String = "undefined"
		assert_false(Fmod.check_event_path(wanted), wanted + " should not be present")
	
	func test_assert_set_volume():
		var desired_value: float = 4.0
		Fmod.set_event_volume(id, desired_value)
		assert_eq(Fmod.get_event_volume(id), desired_value, "Event volume should be 4")
	
	func test_assert_set_pitch():
		var desired_value: float = 0.75
		Fmod.set_event_pitch(id, desired_value)
		assert_eq(Fmod.get_event_pitch(id), desired_value, "Event pitch should be 0.75")
	
	func test_assert_paused():
		Fmod.set_event_paused(id, true)
		assert_true(Fmod.get_event_paused(id), "Event should be paused")
	
	func test_assert_timeline_position():
		var desired_value: int = 10
		Fmod.set_event_paused(id, true)
		Fmod.set_event_timeline_position(id, desired_value)
		yield(yield_for(2), YIELD)
		assert_eq(Fmod.get_event_timeline_position(id), 10, "Event timeline should be at " + str(desired_value))
	
	func test_assert_event_reverb():
		var desired_value: float = 1.5
		Fmod.set_event_reverb_level(id, 0, desired_value)
		assert_eq(Fmod.get_event_reverb_level(id, 0), desired_value, "Event reverb level should be " + str(desired_value))
	
	func test_assert_event_parameter_by_name():
		var desired_value: float = 600.0
		assert_eq(Fmod.get_event_parameter_by_name(id, "RPM"), desired_value, "Event parameter RPM should be " + str(desired_value))
	
	func test_assert_should_pause_all():
		Fmod.set_event_paused(id, false)
		var id2 = Fmod.create_event_instance("event:/Vehicles/Car Engine")
		Fmod.start_event(id2)
		Fmod.pause_all_events(true)
		assert_true(Fmod.get_event_paused(id), "Event " + str(id) + " should be paused")
		assert_true(Fmod.get_event_paused(id2), "Event " + str(id2) + " should be paused")
		Fmod.stop_event(id2, Fmod.FMOD_STUDIO_STOP_IMMEDIATE)
		Fmod.release_event(id2)
	
	func test_assert_should_mute_unmute_all():
		Fmod.mute_all_events()
		assert_true(Fmod.get_bus_mute("bus:/"), "Master bus should be muted")
		Fmod.unmute_all_events()
		assert_false(Fmod.get_bus_mute("bus:/"), "Master bus should not be muted")
	
	func test_assert_attached_to_node():
		Fmod.attach_instance_to_node(id, sprite)
		var node_instance: Object = Fmod.get_object_attached_to_instance(id)
		var node_instance_id: int = node_instance.get_instance_id()
		assert_false(node_instance == null, "Instance " + str(id) + " should be attached to Node")
		var id2: int = Fmod.create_event_instance("event:/Vehicles/Car Engine")
		var object: Object = Fmod.get_object_attached_to_instance(id2)
		assert_true(object == null, "Instance " + str(id2) + " should not be attached to any Node")
		Fmod.attach_instance_to_node(id2, sprite)
		var object_instance_id = Fmod.get_object_attached_to_instance(id2).get_instance_id()
		assert_true(node_instance_id == object_instance_id, "Both instances should be attached to same Node")
		Fmod.detach_instance_from_node(id2)
		object = Fmod.get_object_attached_to_instance(id2)
		assert_true(object == null, "Instance " + str(id2) + " should be detached")
		Fmod.release_event(id2)
		Fmod.detach_instance_from_node(id)
