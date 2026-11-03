extends GdUnitTestSuite

var banks := Array()

func before():
	for bank in ["Master", "SFX"]:
		banks.append(FmodServer.load_bank("res://assets/Banks/%s.bank" % bank, FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL))

func test_assert_event_paths_are_empty_without_strings_bank():
	var descriptions: Array = banks[1].get_description_list()
	assert_array(descriptions).is_not_empty()
	for description: FmodEventDescription in descriptions:
		assert_str(description.get_path()).is_empty()
		assert_bool(FmodServer.check_event_guid(description.get_guid())).is_true()

func test_assert_empty_path_finds_nothing_without_strings_bank():
	assert_bool(FmodServer.check_event_path("")).is_false()
	assert_bool(FmodServer.check_bus_path("")).is_false()
	assert_bool(FmodServer.check_vca_path("")).is_false()
