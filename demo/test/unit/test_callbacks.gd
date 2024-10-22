extends "res://addons/gut/test.gd"

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

func after_all():
	FmodServer.remove_listener(0)

func test_assert_has_signals():
	var emitter: FmodEventEmitter2D = FmodEventEmitter2D.new()
	assert_has_signal(emitter, "timeline_beat")
	assert_has_signal(emitter, "timeline_marker")
	assert_has_signal(emitter, "start_failed")
	assert_has_signal(emitter, "started")
	assert_has_signal(emitter, "restarted")
	assert_has_signal(emitter, "stopped")
	emitter.free()

var callback_called = false

func on_callback(_dict: Dictionary, type: int):
	callback_called = true

func test_assert_set_callback():
	watch_signals(FmodServer)
	var fmod_event: FmodEvent = FmodServer.create_event_instance("event:/Music/Level 02")
	fmod_event.volume = 0
	fmod_event.start()
	fmod_event.set_callback(on_callback, FmodServer.FMOD_STUDIO_EVENT_CALLBACK_ALL)
	await wait_seconds(2)
	assert_true(callback_called)
	fmod_event.stop(FmodServer.FMOD_STUDIO_STOP_IMMEDIATE)
	fmod_event.release()
