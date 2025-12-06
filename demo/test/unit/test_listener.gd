extends "res://addons/gut/test.gd"

class TestListener:
	extends "res://addons/gut/test.gd"
	
	var sprite: Sprite2D = Sprite2D.new()
	
	var banks := Array()
	
	func before_all():
		# load banks
		# warning-ignore:return_value_discarded
		banks.append(
			FmodServer.load_bank("res://assets/Banks/Master.strings.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		)
		# warning-ignore:return_value_discarded
		banks.append(
			FmodServer.load_bank("res://assets/Banks/Master.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		)
		# warning-ignore:return_value_discarded
		banks.append(
			FmodServer.load_bank("res://assets/Banks/Music.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		)
		# warning-ignore:return_value_discarded
		banks.append(
			FmodServer.load_bank("res://assets/Banks/Vehicles.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
		)
		FmodServer.set_listener_number(1)
		get_tree().get_root().add_child(sprite)
		FmodServer.add_listener(0, sprite)
	
	func after_all():
		FmodServer.remove_listener(0, sprite)
	
	func test_assert_should_set_listener_num():
		var desiredValue: int = 1
		assert_listener_num(desiredValue)
		FmodServer.set_listener_number(2)
		desiredValue = 2
		assert_listener_num(desiredValue)
		desiredValue = 1
		FmodServer.set_listener_number(1)
		assert_listener_num(desiredValue)
	
	func test_assert_should_have_proper_weight():
		var desiredValue: float = 1.0
		assert_listener_weight(0, desiredValue)
		desiredValue = 2.0
		FmodServer.set_listener_weight(0, desiredValue)
		assert_listener_weight(0, desiredValue)
		desiredValue = 1.0
		FmodServer.set_listener_weight(0, desiredValue)
		FmodServer.set_listener_number(2)
		desiredValue = 2.0
		FmodServer.add_listener(1, sprite)
		FmodServer.set_listener_weight(1, desiredValue)
		assert_listener_weight(1, desiredValue)
		FmodServer.remove_listener(1, sprite)
		FmodServer.set_listener_number(1)
	
	func test_assert_attach_object_to_listener():
		var desired_listener: int = 0;
		var node_instance: Object = FmodServer.get_object_attached_to_listener(desired_listener)
		assert_false(node_instance == null, "Listener " + str(desired_listener) + " should have an object attached")
		FmodServer.set_listener_number(2);
		desired_listener = 1;
		assert_no_object_attached_to_listener(desired_listener)
		FmodServer.add_listener(desired_listener, sprite)
		assert_true(FmodServer.get_object_attached_to_listener(desired_listener) == node_instance, "Both listeners should be attached to same object")
		FmodServer.remove_listener(1, sprite)
		assert_no_object_attached_to_listener(desired_listener)
		FmodServer.set_listener_number(1)
	
	func test_attach_two_object_to_listeners():
		var desired_listener := 1
		FmodServer.set_listener_number(2);
		FmodServer.add_listener(desired_listener, sprite)
		
		assert_eq(FmodServer.get_object_attached_to_listener(desired_listener), sprite)
		
		var node := Node2D.new()
		
		FmodServer.add_listener(desired_listener, node)
		assert_eq(FmodServer.get_object_attached_to_listener(desired_listener), node)
		FmodServer.remove_listener(desired_listener, sprite)
		assert_eq(FmodServer.get_object_attached_to_listener(desired_listener), node)
		FmodServer.remove_listener(desired_listener, node)
		assert_no_object_attached_to_listener(desired_listener)
		
		node.free()
		
		FmodServer.set_listener_number(1);
	
	func assert_listener_num(desiredValue: int):
		assert_eq(FmodServer.get_listener_number(), desiredValue, "There should be " + str(desiredValue) + " listeners.")
	
	func assert_listener_weight(listenerNum: int, desiredValue: float):
		assert_eq(FmodServer.get_listener_weight(listenerNum), desiredValue, str(listenerNum) + " should have a weight of " + str(desiredValue))
	
	func assert_no_object_attached_to_listener(desired_listener: int):
		assert_true(FmodServer.get_object_attached_to_listener(desired_listener) == null, "Listener " + str(desired_listener) + " should not have any object attached")
