extends GdUnitTestSuite

var sprite: Sprite2D

var banks := Array()

func before():
	for bank in ["Master.strings", "Master", "Music", "Vehicles"]:
		banks.append(FmodServer.load_bank("res://assets/Banks/%s.bank" % bank, FmodBank.StudioLoadBankFlags.NORMAL))
	FmodServer.set_listener_number(1)
	sprite = Sprite2D.new()
	get_tree().get_root().add_child(sprite)
	FmodServer.add_listener(0, sprite)

func after():
	FmodServer.remove_listener(0, sprite)
	sprite.free()

func test_assert_should_have_performance_data():
	assert_object(FmodServer.get_performance_data()).is_not_null()

func test_assert_should_have_dsp_buffer_length():
	assert_int(FmodServer.get_system_dsp_buffer_length()).is_equal(FmodSettings.new().dsp_buffer_size)

func test_assert_should_have_dsp_num_buffers():
	assert_int(FmodServer.get_system_dsp_num_buffers()).is_equal(FmodSettings.new().dsp_buffer_count)
