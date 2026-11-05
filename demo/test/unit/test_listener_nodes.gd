extends GdUnitTestSuite

var parent_a: Node
var parent_b: Node

func before_test():
	FmodServer.set_listener_number(2)
	parent_a = auto_free(Node.new())
	parent_b = auto_free(Node.new())
	get_tree().get_root().add_child(parent_a)
	get_tree().get_root().add_child(parent_b)

func after_test():
	FmodServer.set_listener_number(1)

func test_listener_follows_node_after_reparent_3d():
	var listener := FmodListener3D.new()
	listener.listener_index = 1
	parent_a.add_child(listener)
	assert_object(FmodServer.get_object_attached_to_listener(1)).is_same(listener)
	listener.reparent(parent_b)
	assert_object(FmodServer.get_object_attached_to_listener(1)).is_same(listener)
	listener.global_position = Vector3(1, 2, 3)
	await await_idle_frame()
	await await_idle_frame()
	assert_vector(FmodServer.get_listener_transform_3d(1).origin).is_equal_approx(Vector3(1, 2, 3), Vector3(0.01, 0.01, 0.01))
	listener.free()
	assert_object(FmodServer.get_object_attached_to_listener(1)).is_null()

func test_listener_follows_node_after_reparent_2d():
	var listener := FmodListener2D.new()
	listener.listener_index = 1
	parent_a.add_child(listener)
	listener.reparent(parent_b)
	assert_object(FmodServer.get_object_attached_to_listener(1)).is_same(listener)
	listener.free()
	assert_object(FmodServer.get_object_attached_to_listener(1)).is_null()

func test_listener_stays_released_while_out_of_tree():
	var listener := FmodListener3D.new()
	listener.listener_index = 1
	parent_a.add_child(listener)
	parent_a.remove_child(listener)
	assert_object(FmodServer.get_object_attached_to_listener(1)).is_null()
	parent_b.add_child(listener)
	assert_object(FmodServer.get_object_attached_to_listener(1)).is_same(listener)
	listener.free()
