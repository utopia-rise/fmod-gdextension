extends "res://addons/gut/test.gd"

class TestEventDescription:
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
	
	func after_all():
		Fmod.release_event(id)
		Fmod.remove_listener(0)
		Fmod.unload_bank("res://assets/Banks/Master Bank.strings.bank")
		Fmod.unload_bank("res://assets/Banks/Master Bank.bank")
		Fmod.unload_bank("res://assets/Banks/Music.bank")
		Fmod.unload_bank("res://assets/Banks/Vehicles.bank")
	
	func test_assert_should_create_and_release():
		var desired_value: int = 2
		var id2: int = Fmod.create_event_instance("event:/Vehicles/Car Engine")
		var instance_list: Array = Fmod.desc_get_instance_list("event:/Vehicles/Car Engine")
		assert_eq(instance_list.size(), desired_value, "Event description list size should be " + str(desired_value))
		Fmod.release_event(id2)
		desired_value = 1
		yield(yield_for(2), YIELD)
		assert_eq(Fmod.desc_get_instance_list("event:/Vehicles/Car Engine").size(), desired_value, "Event description list size should be " + str(desired_value))
		id2 = Fmod.create_event_instance("event:/Vehicles/Car Engine")
		var id3: int = Fmod.create_event_instance("event:/Vehicles/Car Engine")
		desired_value = 3
		assert_eq(Fmod.desc_get_instance_list("event:/Vehicles/Car Engine").size(), desired_value, "Event description list size should be " + str(desired_value))
		Fmod.desc_release_all_instances("event:/Vehicles/Car Engine")
		desired_value = 0
		yield(yield_for(2), YIELD)
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
		assert_false(Fmod.desc_has_cue("event:/Vehicles/Car Engine"), "Event description should not have cue")
	
	func test_assert_minimum_distance():
		var desiredValue: float = 1.0
		assert_eq(Fmod.desc_get_minimum_distance("event:/Vehicles/Car Engine"), desiredValue, "Event description minimum distance should be " + str(desiredValue))
	
	func test_assert_maximum_distance():
		var desiredValue: float = 20.0
		assert_eq(Fmod.desc_get_maximum_distance("event:/Vehicles/Car Engine"), desiredValue, "Event description maximum distance should be " + str(desiredValue))
	
	func test_assert_sound_size():
		var desiredValue: float = 2.0
		assert_eq(Fmod.desc_get_sound_size("event:/Vehicles/Car Engine"), desiredValue, "Event description sound size should be " + str(desiredValue))
