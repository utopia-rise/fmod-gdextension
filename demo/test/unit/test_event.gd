extends GdUnitTestSuite

var fmodEvent: FmodEvent
var sprite: Sprite2D

var banks := Array()

func before():
	for bank in ["Master.strings", "Master", "Music", "Vehicles"]:
		banks.append(FmodServer.load_bank("res://assets/Banks/%s.bank" % bank, FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL))
	FmodServer.set_listener_number(1)
	sprite = Sprite2D.new()
	get_tree().get_root().add_child(sprite)
	FmodServer.add_listener(0, sprite)
	fmodEvent = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
	fmodEvent.set_parameter_by_name("RPM", 600)
	fmodEvent.start()

func after():
	fmodEvent.stop(FmodServer.FMOD_STUDIO_STOP_IMMEDIATE)
	fmodEvent.release()
	FmodServer.remove_listener(0, sprite)
	sprite.free()

func test_should_has_event():
	assert_bool(FmodServer.check_event_path("event:/Vehicles/Car Engine")).is_true()

func test_should_has_event_guid():
	assert_bool(FmodServer.check_event_guid("{0c8363b4-23af-4f9c-af4b-0951bfd37d84}")).is_true()

func test_should_not_has_event():
	assert_bool(FmodServer.check_event_path("undefined")).is_false()

func test_should_not_has_event_guid():
	assert_bool(FmodServer.check_event_guid("{29583fe9-eee9-4c67-94e1-57d5f6c552af}")).is_false()

func test_assert_set_volume():
	fmodEvent.volume = 4.0
	assert_float(fmodEvent.volume).is_equal(4.0)

func test_assert_set_pitch():
	fmodEvent.pitch = 0.75
	assert_float(fmodEvent.pitch).is_equal(0.75)

func test_assert_paused():
	fmodEvent.paused = true
	assert_bool(fmodEvent.paused).is_true()

func test_assert_timeline_position():
	fmodEvent.paused = true
	fmodEvent.position = 10
	await await_millis(2000)
	assert_int(fmodEvent.position).is_equal(10)

func test_assert_event_reverb():
	fmodEvent.set_reverb_level(0, 1.5)
	assert_float(fmodEvent.get_reverb_level(0)).is_equal(1.5)

func test_assert_event_parameter_by_name():
	assert_float(fmodEvent.get_parameter_by_name("RPM")).is_equal(600.0)

func test_assert_event_parameter_reflects_automatic_value():
	var sfx_bank := FmodServer.load_bank("res://assets/Banks/SFX.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	var explosion := FmodServer.create_event_instance("event:/Weapons/Explosion")
	explosion.set_2d_attributes(Transform2D(0, Vector2(500, 0)))
	explosion.start()
	await await_millis(200)
	var distance := explosion.get_parameter_by_name("Distance")
	var distance_id: int = FmodServer.get_event("event:/Weapons/Explosion").get_parameter_by_name("Distance").get_id()
	var distance_by_id := explosion.get_parameter_by_id(distance_id)
	explosion.stop(FmodServer.FMOD_STUDIO_STOP_IMMEDIATE)
	explosion.release()
	assert_object(sfx_bank).is_not_null()
	assert_float(distance).is_greater(0.0)
	assert_float(distance).is_less_equal(100.0)
	assert_float(distance_by_id).is_equal(distance)

func test_assert_event_parameter_user_value():
	fmodEvent.set_parameter_by_name("RPM", 1200)
	assert_float(fmodEvent.get_parameter_user_value_by_name("RPM")).is_equal(1200.0)
	var rpm_id: int = FmodServer.get_event("event:/Vehicles/Car Engine").get_parameter_by_name("RPM").get_id()
	assert_float(fmodEvent.get_parameter_user_value_by_id(rpm_id)).is_equal(1200.0)
	fmodEvent.set_parameter_by_name("RPM", 600)
	assert_float(fmodEvent.get_parameter_user_value_by_name("Load")).is_equal(-1.0)

func test_assert_should_pause_all():
	fmodEvent.paused = false
	var fmodEvent2: FmodEvent = FmodServer.create_event_instance_with_guid("{0c8363b4-23af-4f9c-af4b-0951bfd37d84}")
	fmodEvent2.start()
	FmodServer.pause_all_events()
	assert_bool(fmodEvent.paused).is_true()
	assert_bool(fmodEvent2.paused).is_true()
	fmodEvent2.stop(FmodServer.FMOD_STUDIO_STOP_IMMEDIATE)
	fmodEvent2.release()

func test_assert_should_mute_unmute_all():
	var bus: FmodBus = FmodServer.get_bus("bus:/")
	FmodServer.mute_all_events()
	assert_bool(bus.mute).is_true()
	FmodServer.unmute_all_events()
	assert_bool(bus.mute).is_false()
