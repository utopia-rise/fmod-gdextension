extends GdUnitTestSuite

var sprite: Sprite2D

var banks := Array()

var callback_called = false

func before():
	for bank in ["Master.strings", "Master", "Music", "Vehicles"]:
		banks.append(FmodServer.load_bank("res://assets/Banks/%s.bank" % bank, FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL))
	FmodServer.set_listener_number(1)
	sprite = Sprite2D.new()
	get_tree().get_root().add_child(sprite)
	FmodServer.add_listener(0, sprite)

func after():
	FmodServer.remove_listener(0, sprite)
	sprite.free()

func test_assert_has_signals():
	var emitter: FmodEventEmitter2D = auto_free(FmodEventEmitter2D.new())
	for signal_name in ["timeline_beat", "timeline_marker", "start_failed", "started", "restarted", "stopped"]:
		assert_signal(emitter).is_signal_exists(signal_name)

func on_callback(_dict: Dictionary, _type: int):
	callback_called = true

func test_assert_set_callback():
	var fmod_event: FmodEvent = FmodServer.create_event_instance("event:/Music/Level 02")
	fmod_event.volume = 0
	fmod_event.start()
	fmod_event.set_callback(on_callback, FmodServer.FMOD_STUDIO_EVENT_CALLBACK_ALL)
	await await_millis(2000)
	assert_bool(callback_called).is_true()
	fmod_event.stop(FmodServer.FMOD_STUDIO_STOP_IMMEDIATE)
	fmod_event.release()
