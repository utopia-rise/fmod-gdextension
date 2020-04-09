extends "res://addons/gut/test.gd"

class TestSound:
	extends "res://addons/gut/test.gd"
	
	var sound
	var music
	var sound_file = "res://assets/Sounds/doorOpen_1.ogg"
	var music_file = "res://assets/Music/jingles_SAX07.ogg"
	
	func before_all():
		Fmod.load_file_as_sound(sound_file)
		Fmod.load_file_as_music(music_file)
		sound = Fmod.create_sound_instance(sound_file)
		music = Fmod.create_sound_instance(music_file)
	
	func after_all():
		Fmod.release_sound(sound)
		Fmod.release_sound(music)
		Fmod.unload_file(sound_file)
		Fmod.unload_file(music_file)
	
	func test_assert_set_volume():
		var desired_value: float = 2.0
		Fmod.set_sound_volume(sound, desired_value)
		assert_eq(Fmod.get_sound_volume(sound), desired_value, "Sound volume should be 2")
		Fmod.set_sound_volume(music, desired_value)
		assert_eq(Fmod.get_sound_volume(music), desired_value, "Sound volume should be 2")
	
	func test_assert_set_pitch():
		var desired_value: float = 0.75
		Fmod.set_sound_pitch(sound, desired_value)
		assert_eq(Fmod.get_sound_pitch(sound), desired_value, "Sound pitch should be 0.75")
		Fmod.set_sound_pitch(music, desired_value)
		assert_eq(Fmod.get_sound_pitch(music), desired_value, "Sound pitch should be 0.75")	
	
	func test_assert_playing():
		Fmod.play_sound(sound)
		assert_true(Fmod.is_sound_playing(sound), "Sound should be playing")
		Fmod.play_sound(music)
		assert_true(Fmod.is_sound_playing(music), "Sound should be playing")
