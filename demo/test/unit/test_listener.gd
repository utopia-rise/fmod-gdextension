extends GdUnitTestSuite

var sprite: Sprite2D

var banks := Array()

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

func test_assert_should_set_listener_num():
	assert_int(FmodServer.get_listener_number()).is_equal(1)
	FmodServer.set_listener_number(2)
	assert_int(FmodServer.get_listener_number()).is_equal(2)
	FmodServer.set_listener_number(1)
	assert_int(FmodServer.get_listener_number()).is_equal(1)

func test_assert_should_have_proper_weight():
	assert_float(FmodServer.get_listener_weight(0)).is_equal(1.0)
	FmodServer.set_listener_weight(0, 0.5)
	assert_float(FmodServer.get_listener_weight(0)).is_equal(0.5)
	FmodServer.set_listener_weight(0, 1.0)
	FmodServer.set_listener_number(2)
	FmodServer.add_listener(1, sprite)
	FmodServer.set_listener_weight(1, 0.5)
	assert_float(FmodServer.get_listener_weight(1)).is_equal(0.5)
	FmodServer.remove_listener(1, sprite)
	FmodServer.set_listener_number(1)

func test_assert_attach_object_to_listener():
	var node_instance: Object = FmodServer.get_object_attached_to_listener(0)
	assert_object(node_instance).is_not_null()
	FmodServer.set_listener_number(2)
	assert_object(FmodServer.get_object_attached_to_listener(1)).is_null()
	FmodServer.add_listener(1, sprite)
	assert_object(FmodServer.get_object_attached_to_listener(1)).is_same(node_instance)
	FmodServer.remove_listener(1, sprite)
	assert_object(FmodServer.get_object_attached_to_listener(1)).is_null()
	FmodServer.set_listener_number(1)

func test_attach_two_object_to_listeners():
	FmodServer.set_listener_number(2)
	FmodServer.add_listener(1, sprite)
	assert_object(FmodServer.get_object_attached_to_listener(1)).is_same(sprite)
	var node: Node2D = auto_free(Node2D.new())
	FmodServer.add_listener(1, node)
	assert_object(FmodServer.get_object_attached_to_listener(1)).is_same(node)
	FmodServer.remove_listener(1, sprite)
	assert_object(FmodServer.get_object_attached_to_listener(1)).is_same(node)
	FmodServer.remove_listener(1, node)
	assert_object(FmodServer.get_object_attached_to_listener(1)).is_null()
	FmodServer.set_listener_number(1)
