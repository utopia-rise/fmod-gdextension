extends "res://addons/gut/test.gd"

class TestEventDescription:
	extends "res://addons/gut/test.gd"
	
	var id: int
	
	func before_all():
		# load banks
		# warning-ignore:return_value_discarded
		Fmod.load_bank("./assets/Banks/Master Bank.strings.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		Fmod.load_bank("./assets/Banks/Master Bank.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		Fmod.load_bank("./assets/Banks/Music.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		# warning-ignore:return_value_discarded
		Fmod.load_bank("./assets/Banks/Vehicles.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
		Fmod.set_listener_number(1)
		Fmod.add_listener(0, self)
		id = Fmod.create_event_instance("event:/Vehicles/Car Engine")
	
	func after_all():
		Fmod.release_event(id)
		Fmod.remove_listener(0)
		Fmod.unload_bank("./assets/Banks/Master Bank.strings.bank")
		Fmod.unload_bank("./assets/Banks/Master Bank.bank")
		Fmod.unload_bank("./assets/Banks/Music.bank")
		Fmod.unload_bank("./assets/Banks/Vehicles.bank")
	
	func test_assert_desc_instance_list():
		var desired_value: int = 2
		var id2: int = Fmod.create_event_instance("event:/Vehicles/Car Engine")
		var instance_list: Array = Fmod.desc_get_instance_list("event:/Vehicles/Car Engine")
		assert_eq(instance_list.size(), desired_value, "Event description list size should be " + str(desired_value))
		Fmod.release_event(id2)
		desired_value = 1
		var t = Timer.new()
		t.set_wait_time(3)
		t.set_one_shot(true)
		self.add_child(t)
		t.start()
		yield(t, "timeout")
		assert_eq(Fmod.desc_get_instance_list("event:/Vehicles/Car Engine").size(), desired_value, "Event description list size should be " + str(desired_value))