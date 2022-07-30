extends "res://addons/gut/test.gd"

class TestEventDescription:
	extends "res://addons/gut/test.gd"
	
	var id: int
	var sprite: Sprite2D = Sprite2D.new()
	
	func before_all():
		# load banks
		# warning-ignore:return_value_discarded
		Fmod.load_bank("res://assets/Banks/Master.strings.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		Fmod.load_bank("res://assets/Banks/Master.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		Fmod.load_bank("res://assets/Banks/Music.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		Fmod.load_bank("res://assets/Banks/Vehicles.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		Fmod.set_listener_number(1)
		get_tree().get_root().add_child(sprite)
		Fmod.add_listener(0, sprite)
		id = Fmod.create_event_instance("event:/Vehicles/Car Engine")
	
	func after_all():
		Fmod.release_event(id)
		Fmod.remove_listener(0)
		Fmod.unload_bank("res://assets/Banks/Master.strings.bank")
		Fmod.unload_bank("res://assets/Banks/Master.bank")
		Fmod.unload_bank("res://assets/Banks/Music.bank")
		Fmod.unload_bank("res://assets/Banks/Vehicles.bank")
	
	func test_assert_should_create_and_release():
		var desired_value: int = 2
		var id2: int = Fmod.create_event_instance("event:/Vehicles/Car Engine")
		var instance_list: Array = Fmod.desc_get_instance_list("event:/Vehicles/Car Engine")
		assert_eq(instance_list.size(), desired_value, "Event description list size should be " + str(desired_value))
		Fmod.release_event(id2)
		desired_value = 1
		await yield_for(2)
		assert_eq(Fmod.desc_get_instance_list("event:/Vehicles/Car Engine").size(), desired_value, "Event description list size should be " + str(desired_value))
		id2 = Fmod.create_event_instance("event:/Vehicles/Car Engine")
		var id3: int = Fmod.create_event_instance("event:/Vehicles/Car Engine")
		desired_value = 3
		assert_eq(Fmod.desc_get_instance_list("event:/Vehicles/Car Engine").size(), desired_value, "Event description list size should be " + str(desired_value))
		Fmod.desc_release_all_instances("event:/Vehicles/Car Engine")
		desired_value = 0
		await yield_for(2)
		assert_eq(Fmod.desc_get_instance_count("event:/Vehicles/Car Engine"), desired_value, "Event description list size should be " + str(desired_value))
		id = Fmod.create_event_instance("event:/Vehicles/Car Engine")
	
	func test_assert_should_be_3d():
		assert_true(Fmod.desc_is_3d("event:/Vehicles/Car Engine"), "Event description should be 3D")
	
	func test_assert_should_not_be_oneshot():
		assert_false(Fmod.desc_is_one_shot("event:/Vehicles/Car Engine"), "Event description should not be oneshot")
	
	func test_assert_should_not_be_snapshot():
		assert_false(Fmod.desc_is_snapshot("event:/Vehicles/Car Engine"), "Event description should not be snapshot")
	
	func test_assert_should_not_be_stream():
		assert_false(Fmod.desc_is_stream("event:/Vehicles/Car Engine"), "Event description should not be stream")
	
	func test_assert_should_not_have_cue():
		assert_false(Fmod.desc_has_sustain_point("event:/Vehicles/Car Engine"), "Event description should not have cue")
	
	func test_assert_min_max_distance():
		var desiredMin: float = 1.0
		var desiredMax: float = 20.0
		var minMaxDistance = Fmod.desc_get_min_max_distance("event:/Vehicles/Car Engine")
		assert_eq(minMaxDistance[0], desiredMin, "Event description minimum distance should be " + str(desiredMin))
		assert_eq(minMaxDistance[1], desiredMax, "Event description maximum distance should be " + str(desiredMax))
	
	func test_assert_sound_size():
		var desiredValue: float = 2.0
		assert_eq(Fmod.desc_get_sound_size("event:/Vehicles/Car Engine"), desiredValue, "Event description sound size should be " + str(desiredValue))
		
	func test_assert_should_retrieve_user_property_by_name():
		var desiredSize: int = 0
		var property = Fmod.desc_get_user_property("event:/Vehicles/Car Engine", "abc")
		assert_eq(property.size(), desiredSize, "Number of user properties should be " + str(desiredSize))
		
	func test_assert_should_retrieve_user_property_by_index():
		var desiredSize: int = 0
		var property = Fmod.desc_user_property_by_index("event:/Vehicles/Car Engine", 0)
		assert_eq(property.size(), desiredSize, "Number of user properties should be " + str(desiredSize))

	func test_assert_should_retrieve_user_property_count():
		var desiredSize: int = 0
		var property = Fmod.desc_get_user_property_count("event:/Vehicles/Car Engine")
		assert_eq(property, desiredSize, "Number of user properties should be " + str(desiredSize))
