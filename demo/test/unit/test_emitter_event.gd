extends GdUnitTestSuite

var banks := Array()

func before():
	for bank in ["Master.strings", "Master", "Music", "Vehicles"]:
		banks.append(FmodServer.load_bank("res://assets/Banks/%s.bank" % bank, FmodBank.StudioLoadBankFlags.NORMAL))

func test_assert_has_no_event_before_playing():
	var emitter: FmodEventEmitter2D = auto_free(FmodEventEmitter2D.new())
	emitter.preload_event = false
	emitter.event_name = "event:/Music/Level 02"
	assert_object(emitter.get_event()).is_null()
	assert_int(emitter.get_timeline_position()).is_equal(0)
	emitter.set_timeline_position(1000)

func test_assert_gives_the_playing_event():
	var emitter: FmodEventEmitter3D = auto_free(FmodEventEmitter3D.new())
	emitter.event_name = "event:/Music/Level 02"
	add_child(emitter)
	emitter.volume = 0
	emitter.play()
	var event: FmodEvent = emitter.get_event()
	assert_object(event).is_not_null()
	assert_bool(event.is_valid()).is_true()
	emitter.set_timeline_position(1500)
	await await_millis(500)
	assert_int(event.position).is_greater_equal(1000)
	assert_int(emitter.get_timeline_position()).is_greater_equal(1000)
	emitter.stop()

func test_assert_one_shot_is_not_the_emitter_event():
	var emitter: FmodEventEmitter2D = auto_free(FmodEventEmitter2D.new())
	emitter.preload_event = false
	emitter.event_name = "event:/Music/Level 02"
	emitter.play_one_shot()
	assert_object(emitter.get_event()).is_null()

func test_assert_zero_scale_keeps_a_valid_orientation():
	var emitter: FmodEventEmitter3D = auto_free(FmodEventEmitter3D.new())
	emitter.event_name = "event:/Music/Level 02"
	add_child(emitter)
	emitter.volume = 0
	emitter.play()
	var event: FmodEvent = emitter.get_event()
	event.set_3d_attributes(Transform3D.IDENTITY)
	var default_basis: Basis = event.transform_3d.basis
	event.set_3d_attributes(Transform3D(Basis(Vector3.RIGHT, 1.0), Vector3.ZERO))
	event.set_3d_attributes(Transform3D(Basis.from_scale(Vector3.ZERO), Vector3(1, 2, 3)))
	assert_vector(event.transform_3d.basis.y).is_equal_approx(default_basis.y, Vector3(0.001, 0.001, 0.001))
	assert_vector(event.transform_3d.basis.z).is_equal_approx(default_basis.z, Vector3(0.001, 0.001, 0.001))
	emitter.stop()
