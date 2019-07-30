extends "res://addons/gut/test.gd"

func before_all():
	Fmod.set_listener_number(1)
	Fmod.add_listener(0, self)

func after_all():
	Fmod.remove_listener(0)

class TestVolume:
	extends "res://addons/gut/test.gd"
	
	var open_sound: int
	
	func before_all():
		open_sound = Fmod.load_sound("res://assets/Sounds/doorOpen_1.ogg", Fmod.FMOD_DEFAULT)
	
	func after_all():
		Fmod.release_sound(open_sound)
	
	func test_assert_set_volume():
		Fmod.set_sound_volume(open_sound, 2)
		assert_eq(Fmod.get_sound_volume(open_sound), 2.0, "Sound volume should be 2")

class TestPitch:
	extends "res://addons/gut/test.gd"
	
	var open_sound: int
	
	func before_all():
		open_sound = Fmod.load_sound("res://assets/Sounds/doorOpen_1.ogg", Fmod.FMOD_DEFAULT)
	
	func after_all():
		Fmod.release_sound(open_sound)
	
	func test_assert_set_pitch():
		Fmod.set_sound_pitch(open_sound, 0.75)
		assert_eq(Fmod.get_sound_pitch(open_sound), 0.75, "Sound pitch should be 0.75")

class TestPlay:
	extends "res://addons/gut/test.gd"
	
	var open_sound: int
	
	func before_all():
		open_sound = Fmod.load_sound("res://assets/Sounds/doorOpen_1.ogg", Fmod.FMOD_DEFAULT)
	
	func after_all():
		Fmod.release_sound(open_sound)
	
	func test_assert_playing():
		Fmod.play_sound(open_sound)
		assert_true(Fmod.is_sound_playing(open_sound), "Sound should be playing")