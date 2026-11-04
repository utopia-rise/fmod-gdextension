extends GdUnitTestSuite

const SOUND_FILE = "res://assets/Sounds/doorOpen_1.ogg"

func after():
	FmodServer.shutdown()
	FmodServer.init()

func _restart_fmod(settings: FmodSettings):
	FmodServer.shutdown()
	FmodServer.init(settings)

func _start_fmod_on_godot_bus(mixing: FmodSettings.GodotBusMixing):
	var settings := FmodSettings.new()
	settings.output_routing = FmodSettings.OutputRouting.GODOT_BUS
	settings.godot_bus_mixing = mixing
	_restart_fmod(settings)

func _peak(frames: PackedVector2Array) -> float:
	var peak := 0.0
	for frame in frames:
		peak = maxf(peak, maxf(absf(frame.x), absf(frame.y)))
	return peak

func _fmod_mix_reaches_godot(mixing: FmodSettings.GodotBusMixing) -> bool:
	_start_fmod_on_godot_bus(mixing)
	await await_idle_frame()
	var player: AudioStreamPlayer = _fmod_output_players()[0]
	player.stop()
	var playback := player.stream.instantiate_playback()
	playback.start()
	FmodServer.load_file_as_sound(SOUND_FILE)
	var sound := FmodServer.create_sound_instance(SOUND_FILE)
	sound.play()
	var heard := false
	for i in 100:
		FmodServer.update()
		if _peak(playback.mix_audio(1.0, 1024)) > 0.0:
			heard = true
			break
		await get_tree().create_timer(0.02).timeout
	sound.release()
	FmodServer.unload_file(SOUND_FILE)
	return heard

func test_assert_godot_audio_bus_is_empty_on_the_device_output():
	assert_str(FmodServer.get_godot_audio_bus()).is_empty()

func test_assert_godot_audio_bus_is_the_configured_one():
	_start_fmod_on_godot_bus(FmodSettings.GodotBusMixing.MIX_AHEAD)
	assert_str(FmodServer.get_godot_audio_bus()).is_equal("Master")

func _fmod_output_players() -> Array:
	var runtimes := get_tree().root.get_children().filter(func(node: Node) -> bool: return node.get_class() == "FmodRuntime")
	if runtimes.is_empty():
		return []
	return runtimes[0].get_children().filter(func(node: Node) -> bool: return node is AudioStreamPlayer and node.stream.get_class() == "FmodAudioStream")

func test_assert_scripts_cannot_create_the_audio_stream():
	assert_bool(ClassDB.can_instantiate("FmodAudioStream")).is_false()
	assert_bool(ClassDB.can_instantiate("FmodAudioStreamPlayback")).is_false()

func test_assert_init_plays_fmod_output_on_the_godot_bus_until_shutdown():
	_start_fmod_on_godot_bus(FmodSettings.GodotBusMixing.MIX_AHEAD)
	await await_idle_frame()
	var players := _fmod_output_players()
	assert_int(players.size()).is_equal(1)
	assert_str(players[0].bus).is_equal("Master")
	assert_bool(players[0].playing).is_true()
	FmodServer.shutdown()
	await await_idle_frame()
	assert_array(_fmod_output_players()).is_empty()

func test_assert_on_demand_stream_plays_fmod_mix():
	assert_bool(await _fmod_mix_reaches_godot(FmodSettings.GodotBusMixing.ON_DEMAND)).is_true()

func test_assert_mix_ahead_stream_plays_fmod_mix():
	assert_bool(await _fmod_mix_reaches_godot(FmodSettings.GodotBusMixing.MIX_AHEAD)).is_true()

func _event_state_without_update(processing: FmodSettings.StudioProcessing) -> FmodEvent.StudioPlaybackState:
	var settings := FmodSettings.new()
	settings.studio_processing = processing
	_restart_fmod(settings)
	var banks := []
	for bank in ["Master.strings", "Master", "Vehicles"]:
		banks.append(FmodServer.load_bank("res://assets/Banks/%s.bank" % bank, FmodBank.StudioLoadBankFlags.NORMAL))
	var event := FmodServer.create_event_instance("event:/Vehicles/Car Engine")
	event.start()
	FmodServer.update()
	OS.delay_msec(300)
	var state := event.get_playback_state()
	event.stop(FmodEvent.StudioStopMode.IMMEDIATE)
	event.release()
	return state

func test_assert_background_thread_processing_works_between_updates():
	assert_int(_event_state_without_update(FmodSettings.StudioProcessing.BACKGROUND_THREAD)).is_equal(FmodEvent.StudioPlaybackState.PLAYING)

func test_assert_main_thread_processing_waits_for_update():
	assert_int(_event_state_without_update(FmodSettings.StudioProcessing.MAIN_THREAD)).is_equal(FmodEvent.StudioPlaybackState.PLAYBACK_STARTING)
