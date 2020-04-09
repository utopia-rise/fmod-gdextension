extends "res://addons/gut/test.gd"

class TestListener:
	extends "res://addons/gut/test.gd"
	
	var sprite: Sprite = Sprite.new()
	
	func before_all():
		# load banks
		# warning-ignore:return_value_discarded
		Fmod.load_bank("res://assets/Banks/Master Bank.strings.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		Fmod.load_bank("res://assets/Banks/Master Bank.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		Fmod.load_bank("res://assets/Banks/Music.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		Fmod.load_bank("res://assets/Banks/Vehicles.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		Fmod.set_listener_number(1)
		get_tree().get_root().add_child(sprite)
		Fmod.add_listener(0, sprite)
	
	func after_all():
		Fmod.remove_listener(0)
		Fmod.unload_bank("res://assets/Banks/Master Bank.strings.bank")
		Fmod.unload_bank("res://assets/Banks/Master Bank.bank")
		Fmod.unload_bank("res://assets/Banks/Music.bank")
		Fmod.unload_bank("res://assets/Banks/Vehicles.bank")
	
	func test_assert_should_set_listener_num():
		var desiredValue: int = 1
		assert_listener_num(desiredValue)
		Fmod.set_listener_number(2)
		desiredValue = 2
		assert_listener_num(desiredValue)
		desiredValue = 1
		Fmod.set_listener_number(1)
		assert_listener_num(desiredValue)
	
	func test_assert_should_have_proper_weight():
		var desiredValue: float = 1.0
		assert_listener_weight(0, desiredValue)
		desiredValue = 2.0
		Fmod.set_listener_weight(0, desiredValue)
		assert_listener_weight(0, desiredValue)
		desiredValue = 1.0
		Fmod.set_listener_weight(0, desiredValue)
		Fmod.set_listener_number(2)
		desiredValue = 2.0
		Fmod.add_listener(1, sprite)
		Fmod.set_listener_weight(1, desiredValue)
		assert_listener_weight(1, desiredValue)
		Fmod.remove_listener(1)
		Fmod.set_listener_number(1)
	
	func test_assert_attach_object_to_listener():
		var desired_listener: int = 0;
		var node_instance: Object = Fmod.get_object_attached_to_listener(desired_listener)
		assert_false(node_instance == null, "Listener " + str(desired_listener) + " should have an object attached")
		Fmod.set_listener_number(2);
		desired_listener = 1;
		assert_no_object_attached_to_listener(desired_listener)
		Fmod.add_listener(desired_listener, sprite)
		assert_true(Fmod.get_object_attached_to_listener(desired_listener) == node_instance, "Both listeners should be attached to same object")
		Fmod.remove_listener(1)
		assert_no_object_attached_to_listener(desired_listener)
		Fmod.set_listener_number(1)
	
	func assert_listener_num(desiredValue: int):
		assert_eq(Fmod.get_listener_number(), desiredValue, "There should be " + str(desiredValue) + " listeners.")
	
	func assert_listener_weight(listenerNum: int, desiredValue: float):
		assert_eq(Fmod.get_listener_weight(listenerNum), desiredValue, str(listenerNum) + " should have a weight of " + str(desiredValue))
	
	func assert_no_object_attached_to_listener(desired_listener: int):
		assert_true(Fmod.get_object_attached_to_listener(desired_listener) == null, "Listener " + str(desired_listener) + " should not have any object attached")
