extends GdUnitTestSuite

var sprite: Sprite2D

var master_strings_bank: FmodBank
var masterBank: FmodBank
var musicBank: FmodBank
var vehicleBank: FmodBank

func before():
	master_strings_bank = FmodServer.load_bank("res://assets/Banks/Master.strings.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	masterBank = FmodServer.load_bank("res://assets/Banks/Master.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	musicBank = FmodServer.load_bank("res://assets/Banks/Music.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	vehicleBank = FmodServer.load_bank("res://assets/Banks/Vehicles.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	FmodServer.set_listener_number(1)
	sprite = Sprite2D.new()
	get_tree().get_root().add_child(sprite)
	FmodServer.add_listener(0, sprite)

func after():
	FmodServer.remove_listener(0, sprite)
	sprite.free()

func test_assert_bank_loaded():
	assert_int(vehicleBank.get_loading_state()).is_equal(FmodServer.FMOD_STUDIO_LOADING_STATE_LOADED)

func test_assert_bank_bus_count():
	assert_int(musicBank.get_bus_count()).is_equal(0)
	assert_int(masterBank.get_bus_count()).is_equal(12)
	assert_int(vehicleBank.get_bus_count()).is_equal(0)

func test_assert_bank_event_count():
	assert_int(musicBank.get_event_description_count()).is_equal(4)
	assert_int(masterBank.get_event_description_count()).is_equal(5)
	assert_int(vehicleBank.get_event_description_count()).is_equal(2)

func test_assert_bank_string_count():
	assert_int(musicBank.get_string_count()).is_equal(0)
	assert_int(masterBank.get_string_count()).is_equal(0)
	assert_int(vehicleBank.get_string_count()).is_equal(0)

func test_assert_bank_vca_count():
	assert_int(musicBank.get_vca_count()).is_equal(0)
	assert_int(masterBank.get_vca_count()).is_equal(3)
	assert_int(vehicleBank.get_vca_count()).is_equal(0)
