extends GdUnitTestSuite

var sprite: Sprite2D

var master_strings_bank: FmodBank
var masterBank: FmodBank
var musicBank: FmodBank
var vehicleBank: FmodBank

func before():
	master_strings_bank = FmodServer.load_bank("res://assets/Banks/Master.strings.bank", FmodBank.StudioLoadBankFlags.NORMAL)
	masterBank = FmodServer.load_bank("res://assets/Banks/Master.bank", FmodBank.StudioLoadBankFlags.NORMAL)
	musicBank = FmodServer.load_bank("res://assets/Banks/Music.bank", FmodBank.StudioLoadBankFlags.NORMAL)
	vehicleBank = FmodServer.load_bank("res://assets/Banks/Vehicles.bank", FmodBank.StudioLoadBankFlags.NORMAL)
	FmodServer.set_listener_number(1)
	sprite = Sprite2D.new()
	get_tree().get_root().add_child(sprite)
	FmodServer.add_listener(0, sprite)

func after():
	FmodServer.remove_listener(0, sprite)
	sprite.free()

func test_assert_bank_loaded():
	assert_int(vehicleBank.get_loading_state()).is_equal(FmodBank.StudioLoadingState.LOADED)

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

func test_bank_sample_data():
	vehicleBank.load_sample_data()
	FmodServer.wait_for_all_loads()
	assert_int(vehicleBank.get_sample_loading_state()).is_equal(FmodBank.StudioLoadingState.LOADED)
	vehicleBank.unload_sample_data()
	FmodServer.wait_for_all_loads()
	assert_int(vehicleBank.get_sample_loading_state()).is_equal(FmodBank.StudioLoadingState.UNLOADED)

func test_bank_loader_preloads_sample_data():
	var loader := FmodBankLoader.new()
	loader.bank_paths = [vehicleBank.get_godot_res_path()]
	loader.preload_sample_data = true
	get_tree().get_root().add_child(loader)
	FmodServer.wait_for_all_loads()
	assert_int(vehicleBank.get_sample_loading_state()).is_equal(FmodBank.StudioLoadingState.LOADED)

	loader.bank_paths = []
	FmodServer.wait_for_all_loads()
	assert_int(vehicleBank.get_sample_loading_state()).is_equal(FmodBank.StudioLoadingState.UNLOADED)

	loader.bank_paths = [vehicleBank.get_godot_res_path()]
	FmodServer.wait_for_all_loads()
	assert_int(vehicleBank.get_sample_loading_state()).is_equal(FmodBank.StudioLoadingState.LOADED)

	loader.preload_sample_data = false
	FmodServer.wait_for_all_loads()
	assert_int(vehicleBank.get_sample_loading_state()).is_equal(FmodBank.StudioLoadingState.UNLOADED)

	loader.preload_sample_data = true
	get_tree().get_root().remove_child(loader)
	FmodServer.wait_for_all_loads()
	assert_int(vehicleBank.get_sample_loading_state()).is_equal(FmodBank.StudioLoadingState.UNLOADED)
	loader.free()
