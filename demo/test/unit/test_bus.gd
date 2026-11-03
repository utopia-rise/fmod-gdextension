extends GdUnitTestSuite

const MASTER_BUS_GUID = "{af9d027a-3a1f-49a8-a9ef-4cbe20673632}"

var fmodEvent: FmodEvent
var sprite: Sprite2D

var banks := Array()

func before():
	for bank in ["Master.strings", "Master", "Music", "Vehicles"]:
		banks.append(FmodServer.load_bank("res://assets/Banks/%s.bank" % bank, FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL))
	FmodServer.set_listener_number(1)
	fmodEvent = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
	sprite = Sprite2D.new()
	get_tree().get_root().add_child(sprite)
	FmodServer.add_listener(0, sprite)

func after():
	fmodEvent.release()
	FmodServer.remove_listener(0, sprite)
	sprite.free()

func test_assert_should_has_master_bus():
	assert_bool(FmodServer.check_bus_path("bus:/")).is_true()

func test_assert_should_has_master_bus_using_guid():
	assert_bool(FmodServer.check_bus_guid(MASTER_BUS_GUID)).is_true()

func test_assert_should_not_has_bus():
	assert_bool(FmodServer.check_bus_path("undefined")).is_false()

func test_assert_should_not_has_bus_using_guid():
	assert_bool(FmodServer.check_bus_guid("{29583fe9-eee9-4c67-94e1-57d5f6c552af}")).is_false()

func test_assert_mute_unmute():
	var masterBus: FmodBus = FmodServer.get_bus("bus:/")
	assert_bool(masterBus.mute).is_false()
	masterBus.mute = true
	assert_bool(masterBus.mute).is_true()
	masterBus.mute = false
	assert_bool(masterBus.mute).is_false()

func test_assert_pause_unpause():
	var masterBus: FmodBus = FmodServer.get_bus("bus:/")
	assert_bool(masterBus.paused).is_false()
	masterBus.paused = true
	assert_bool(masterBus.paused).is_true()
	masterBus.paused = false
	assert_bool(masterBus.paused).is_false()

func test_assert_volume():
	_test_assert_volume(false)
	_test_assert_volume(true)

func _test_assert_volume(is_guid: bool):
	var masterBus: FmodBus = FmodServer.get_bus_from_guid(MASTER_BUS_GUID) if is_guid else FmodServer.get_bus("bus:/")
	assert_float(masterBus.volume).is_equal(1.0)
	masterBus.volume = 0.5
	assert_float(masterBus.volume).is_equal(0.5)
	masterBus.volume = 1

func test_assert_bus_stop_events():
	var fmodEvent2: FmodEvent = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
	fmodEvent.start()
	fmodEvent2.start()
	await await_millis(2000)
	assert_int(fmodEvent.get_playback_state()).is_equal(FmodEvent.FMOD_STUDIO_PLAYBACK_PLAYING)
	assert_int(fmodEvent2.get_playback_state()).is_equal(FmodEvent.FMOD_STUDIO_PLAYBACK_PLAYING)
	FmodServer.get_bus("bus:/").stop_all_events(FmodEvent.FMOD_STUDIO_STOP_IMMEDIATE)
	await await_millis(2000)
	assert_int(fmodEvent.get_playback_state()).is_equal(FmodEvent.FMOD_STUDIO_PLAYBACK_STOPPED)
	assert_int(fmodEvent2.get_playback_state()).is_equal(FmodEvent.FMOD_STUDIO_PLAYBACK_STOPPED)
	fmodEvent2.release()
