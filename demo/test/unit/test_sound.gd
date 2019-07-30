extends "res://addons/gut/test.gd"

func before_all():
	# set up FMOD
	Fmod.set_software_format(0, Fmod.FMOD_SPEAKERMODE_STEREO, 0)
	Fmod.init(1024, Fmod.FMOD_STUDIO_INIT_LIVEUPDATE, Fmod.FMOD_INIT_NORMAL)
	Fmod.set_sound_3D_settings(1, 32, 1)
	Fmod.set_listener_number(1)
	Fmod.add_listener(0, self)

class TestVolume:
	extends "res://addons/gut/test.gd"
	
	var open_sound: int
	
	func before_all():
		open_sound = Fmod.load_sound("res://assets/Sounds/doorOpen_1.ogg", Fmod.FMOD_DEFAULT)
	
	func test_assert_set_sound():
		Fmod.set_sound_volume(open_sound, 2)
		assert_eq(Fmod.get_sound_volume(open_sound), 2.0, "Volume should be 2")
	
	func after_all():
		Fmod.release_sound(open_sound)

class TestPitch:
	extends "res://addons/gut/test.gd"
	
	var open_sound: int
	
	func before_all():
		open_sound = Fmod.load_sound("res://assets/Sounds/doorOpen_1.ogg", Fmod.FMOD_DEFAULT)
	
	func test_assert_set_pitch():
		Fmod.set_sound_pitch(open_sound, 0.75)
		assert_eq(Fmod.get_sound_pitch(open_sound), 0.75, "Pitch should be 0.75")
	
	func after_all():
		Fmod.release_sound(open_sound)