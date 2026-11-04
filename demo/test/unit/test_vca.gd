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

func test_assert_valid_paths():
	assert_bool(FmodServer.check_vca_path("vca:/Environment")).is_true()
	assert_bool(FmodServer.check_vca_path("vca:/Player")).is_true()
	assert_bool(FmodServer.check_vca_path("vca:/Equipment")).is_true()

func test_assert_invalid_path():
	assert_bool(FmodServer.check_vca_path("vca:/undefined")).is_false()

func test_assert_volume():
	_test_assert_volume(false)
	_test_assert_volume(true)

func _test_assert_volume(is_guid: bool):
	var environment_vca: FmodVCA = FmodServer.get_vca_from_guid("{3f0b7d64-e765-400e-ae74-c2d973ad4ca1}") if is_guid else FmodServer.get_vca("vca:/Environment")
	assert_float(environment_vca.volume).is_equal(1.0)
	environment_vca.volume = 0.5
	assert_float(environment_vca.volume).is_equal(0.5)
	environment_vca.volume = 1.0
