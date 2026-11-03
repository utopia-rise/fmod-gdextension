extends GdUnitTestSuite

const CAR_ENGINE_PATH = "event:/Vehicles/Car Engine"
const CAR_ENGINE_GUID = "{0c8363b4-23af-4f9c-af4b-0951bfd37d84}"

var fmodEvent: FmodEvent
var sprite: Sprite2D

var banks := Array()

func before():
	for bank in ["Master.strings", "Master", "Music", "Vehicles"]:
		banks.append(FmodServer.load_bank("res://assets/Banks/%s.bank" % bank, FmodBank.StudioLoadBankFlags.NORMAL))
	FmodServer.set_listener_number(1)
	sprite = Sprite2D.new()
	get_tree().get_root().add_child(sprite)
	FmodServer.add_listener(0, sprite)
	fmodEvent = FmodServer.create_event_instance(CAR_ENGINE_PATH)

func after():
	fmodEvent.release()
	FmodServer.remove_listener(0, sprite)
	sprite.free()

func test_assert_should_create_and_release():
	var fmodEvent2: FmodEvent = FmodServer.create_event_instance(CAR_ENGINE_PATH)
	assert_array(FmodServer.get_event(CAR_ENGINE_PATH).get_instance_list()).has_size(2)
	fmodEvent2.release()
	await await_millis(2000)
	assert_array(FmodServer.get_event(CAR_ENGINE_PATH).get_instance_list()).has_size(1)
	fmodEvent2 = FmodServer.create_event_instance(CAR_ENGINE_PATH)
	var fmodEvent3: FmodEvent = FmodServer.create_event_instance(CAR_ENGINE_PATH)
	assert_array(FmodServer.get_event(CAR_ENGINE_PATH).get_instance_list()).has_size(3)
	FmodServer.get_event(CAR_ENGINE_PATH).release_all_instances()
	await await_millis(2000)
	assert_int(FmodServer.get_event(CAR_ENGINE_PATH).get_instance_count()).is_equal(0)
	fmodEvent = FmodServer.create_event_instance(CAR_ENGINE_PATH)

func test_assert_should_create_and_release_using_guid():
	var fmodEvent2: FmodEvent = FmodServer.create_event_instance_with_guid(CAR_ENGINE_GUID)
	assert_array(FmodServer.get_event_from_guid(CAR_ENGINE_GUID).get_instance_list()).has_size(2)
	fmodEvent2.release()
	await await_millis(2000)
	assert_array(FmodServer.get_event(CAR_ENGINE_PATH).get_instance_list()).has_size(1)
	fmodEvent2 = FmodServer.create_event_instance_with_guid(CAR_ENGINE_GUID)
	var fmodEvent3: FmodEvent = FmodServer.create_event_instance(CAR_ENGINE_PATH)
	assert_array(FmodServer.get_event_from_guid(CAR_ENGINE_GUID).get_instance_list()).has_size(3)
	FmodServer.get_event_from_guid(CAR_ENGINE_GUID).release_all_instances()
	await await_millis(2000)
	assert_int(FmodServer.get_event_from_guid(CAR_ENGINE_GUID).get_instance_count()).is_equal(0)
	fmodEvent = FmodServer.create_event_instance(CAR_ENGINE_PATH)

func test_assert_should_be_3d():
	assert_bool(FmodServer.get_event(CAR_ENGINE_PATH).is_3d()).is_true()
	assert_bool(FmodServer.get_event_from_guid(CAR_ENGINE_GUID).is_3d()).is_true()

func test_assert_should_not_be_oneshot():
	assert_bool(FmodServer.get_event(CAR_ENGINE_PATH).is_one_shot()).is_false()

func test_assert_should_not_be_snapshot():
	assert_bool(FmodServer.get_event(CAR_ENGINE_PATH).is_snapshot()).is_false()

func test_assert_should_not_be_stream():
	assert_bool(FmodServer.get_event(CAR_ENGINE_PATH).is_stream()).is_false()

func test_assert_should_not_have_cue():
	assert_bool(FmodServer.get_event(CAR_ENGINE_PATH).has_sustain_point()).is_false()

func test_assert_min_max_distance():
	var minMaxDistance = FmodServer.get_event(CAR_ENGINE_PATH).get_min_max_distance()
	assert_float(minMaxDistance[0]).is_equal(1.0)
	assert_float(minMaxDistance[1]).is_equal(20.0)

func test_assert_sound_size():
	assert_float(FmodServer.get_event(CAR_ENGINE_PATH).get_sound_size()).is_equal(2.0)

func test_assert_should_retrieve_user_property_by_name():
	assert_dict(FmodServer.get_event(CAR_ENGINE_PATH).get_user_property("abc")).is_empty()

func test_assert_should_retrieve_user_property_by_index():
	assert_dict(FmodServer.get_event(CAR_ENGINE_PATH).user_property_by_index(0)).is_empty()

func test_assert_should_retrieve_user_property_count():
	assert_int(FmodServer.get_event(CAR_ENGINE_PATH).get_user_property_count()).is_equal(0)
