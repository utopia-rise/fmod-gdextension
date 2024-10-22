extends "res://addons/gut/test.gd"

class TestEventDescription:
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
	
	func after_all():
		fmodEvent.release()
		FmodServer.remove_listener(0)
	
	func test_assert_should_create_and_release():
		var desired_value: int = 2
		var fmodEvent2: FmodEvent = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
		var instance_list: Array = FmodServer.get_event("event:/Vehicles/Car Engine").get_instance_list()
		assert_eq(instance_list.size(), desired_value, "Event description list size should be " + str(desired_value))
		fmodEvent2.release()
		desired_value = 1
		await wait_seconds(2)
		assert_eq(FmodServer.get_event("event:/Vehicles/Car Engine").get_instance_list().size(), desired_value, "Event description list size should be " + str(desired_value))
		fmodEvent2 = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
		var fmodEvent3: FmodEvent = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
		desired_value = 3
		assert_eq(FmodServer.get_event("event:/Vehicles/Car Engine").get_instance_list().size(), desired_value, "Event description list size should be " + str(desired_value))
		FmodServer.get_event("event:/Vehicles/Car Engine").release_all_instances()
		desired_value = 0
		await wait_seconds(2)
		assert_eq(FmodServer.get_event("event:/Vehicles/Car Engine").get_instance_count(), desired_value, "Event description list size should be " + str(desired_value))
		fmodEvent = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
	
	func test_assert_should_create_and_release_using_guid():
		var desired_value: int = 2
		var fmodEvent2: FmodEvent = FmodServer.create_event_instance_with_guid("{0c8363b4-23af-4f9c-af4b-0951bfd37d84}")
		var instance_list: Array = FmodServer.get_event_from_guid("{0c8363b4-23af-4f9c-af4b-0951bfd37d84}").get_instance_list()
		assert_eq(instance_list.size(), desired_value, "Event description list size should be " + str(desired_value))
		fmodEvent2.release()
		desired_value = 1
		await wait_seconds(2)
		assert_eq(FmodServer.get_event("event:/Vehicles/Car Engine").get_instance_list().size(), desired_value, "Event description list size should be " + str(desired_value))
		fmodEvent2 = FmodServer.create_event_instance_with_guid("{0c8363b4-23af-4f9c-af4b-0951bfd37d84}")
		var fmodEvent3: FmodEvent = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
		desired_value = 3
		assert_eq(FmodServer.get_event_from_guid("{0c8363b4-23af-4f9c-af4b-0951bfd37d84}").get_instance_list().size(), desired_value, "Event description list size should be " + str(desired_value))
		FmodServer.get_event_from_guid("{0c8363b4-23af-4f9c-af4b-0951bfd37d84}").release_all_instances()
		desired_value = 0
		await wait_seconds(2)
		assert_eq(FmodServer.get_event_from_guid("{0c8363b4-23af-4f9c-af4b-0951bfd37d84}").get_instance_count(), desired_value, "Event description list size should be " + str(desired_value))
		fmodEvent = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
	
	func test_assert_should_be_3d():
		_test_assert_should_be_3d(false)
		_test_assert_should_be_3d(true)
	
	func _test_assert_should_be_3d(is_guid: bool):
		var event: FmodEventDescription = FmodServer.get_event_from_guid("{0c8363b4-23af-4f9c-af4b-0951bfd37d84}") if is_guid else FmodServer.get_event("event:/Vehicles/Car Engine")
		assert_true(event.is_3d(), "Event description should be 3D")
	
	func test_assert_should_not_be_oneshot():
		assert_false(FmodServer.get_event("event:/Vehicles/Car Engine").is_one_shot(), "Event description should not be oneshot")
	
	func test_assert_should_not_be_snapshot():
		assert_false(FmodServer.get_event("event:/Vehicles/Car Engine").is_snapshot(), "Event description should not be snapshot")
	
	func test_assert_should_not_be_stream():
		assert_false(FmodServer.get_event("event:/Vehicles/Car Engine").is_stream(), "Event description should not be stream")
	
	func test_assert_should_not_have_cue():
		assert_false(FmodServer.get_event("event:/Vehicles/Car Engine").has_sustain_point(), "Event description should not have cue")
	
	func test_assert_min_max_distance():
		var desiredMin: float = 1.0
		var desiredMax: float = 20.0
		var minMaxDistance = FmodServer.get_event("event:/Vehicles/Car Engine").get_min_max_distance()
		assert_eq(minMaxDistance[0], desiredMin, "Event description minimum distance should be " + str(desiredMin))
		assert_eq(minMaxDistance[1], desiredMax, "Event description maximum distance should be " + str(desiredMax))
	
	func test_assert_sound_size():
		var desiredValue: float = 2.0
		assert_eq(FmodServer.get_event("event:/Vehicles/Car Engine").get_sound_size(), desiredValue, "Event description sound size should be " + str(desiredValue))
		
	func test_assert_should_retrieve_user_property_by_name():
		var desiredSize: int = 0
		var property = FmodServer.get_event("event:/Vehicles/Car Engine").get_user_property("abc")
		assert_eq(property.size(), desiredSize, "Number of user properties should be " + str(desiredSize))
		
	func test_assert_should_retrieve_user_property_by_index():
		var desiredSize: int = 0
		var property = FmodServer.get_event("event:/Vehicles/Car Engine").user_property_by_index(0)
		assert_eq(property.size(), desiredSize, "Number of user properties should be " + str(desiredSize))

	func test_assert_should_retrieve_user_property_count():
		var desiredSize: int = 0
		var property = FmodServer.get_event("event:/Vehicles/Car Engine").get_user_property_count()
		assert_eq(property, desiredSize, "Number of user properties should be " + str(desiredSize))
