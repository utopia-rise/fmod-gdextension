extends GdUnitTestSuite

var banks := Array()

func before():
	for bank in ["Master.strings", "Master", "Vehicles"]:
		banks.append(FmodServer.load_bank("res://assets/Banks/%s.bank" % bank, FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL))

func _extension_classes() -> Array[String]:
	var classes: Array[String] = []
	for name in ClassDB.get_class_list():
		if name.begins_with("Fmod") and ClassDB.class_exists(name):
			classes.append(name)
	return classes

func _declares_enum(enum_name: String) -> bool:
	var parts := enum_name.split(".")
	return parts.size() == 2 and ClassDB.class_has_enum(parts[0], parts[1])

func test_assert_every_referenced_enum_is_declared():
	for name in _extension_classes():
		for method in ClassDB.class_get_method_list(name, true):
			for info in [method["return"]] + method["args"]:
				if info["usage"] & (PROPERTY_USAGE_CLASS_IS_ENUM | PROPERTY_USAGE_CLASS_IS_BITFIELD):
					assert_bool(_declares_enum(info["class_name"])).override_failure_message("%s.%s references undeclared enum %s" % [name, method["name"], info["class_name"]]).is_true()
		for property in ClassDB.class_get_property_list(name, true):
			if property["usage"] & PROPERTY_USAGE_CLASS_IS_ENUM:
				assert_bool(_declares_enum(property["class_name"])).override_failure_message("%s.%s references undeclared enum %s" % [name, property["name"], property["class_name"]]).is_true()

func test_assert_fmod_enums_belong_to_fmod_server():
	assert_bool(ClassDB.class_has_enum("FmodServer", "FMOD_STUDIO_PLAYBACK_STATE")).is_true()
	assert_bool(ClassDB.class_has_enum("FmodServer", "FMOD_STUDIO_STOP_MODE")).is_true()
	assert_bool(ClassDB.is_class_enum_bitfield("FmodServer", "FMOD_STUDIO_LOAD_BANK_FLAGS")).is_true()
	assert_str(ClassDB.class_get_integer_constant_enum("FmodServer", "FMOD_STUDIO_PLAYBACK_PLAYING")).is_equal("FMOD_STUDIO_PLAYBACK_STATE")
	for constant in ClassDB.class_get_integer_constant_list("FmodServer", true):
		assert_str(ClassDB.class_get_integer_constant_enum("FmodServer", constant)).override_failure_message("%s belongs to no enum" % constant).is_not_empty()

func test_assert_every_bound_argument_is_named():
	for name in _extension_classes():
		for method in ClassDB.class_get_method_list(name, true):
			for argument in method["args"]:
				var argument_name: String = argument["name"]
				assert_bool(argument_name.is_empty() or argument_name.begins_with("_unnamed") or argument_name != argument_name.to_snake_case()).override_failure_message("%s.%s has argument '%s'" % [name, method["name"], argument_name]).is_false()

func test_assert_event_transform_3d_property_sets_attributes():
	var event: FmodEvent = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
	var transform := Transform3D(Basis.IDENTITY, Vector3(1, 2, 3))
	event.transform_3d = transform
	assert_vector(event.transform_3d.origin).is_equal_approx(transform.origin, Vector3(0.001, 0.001, 0.001))
	event.release()

func test_assert_emitter_paused_property_is_bool():
	for property in ClassDB.class_get_property_list("FmodEventEmitter3D", true):
		if property["name"] == "paused":
			assert_int(property["type"]).is_equal(TYPE_BOOL)
