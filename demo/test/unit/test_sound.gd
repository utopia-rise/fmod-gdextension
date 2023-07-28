extends "res://addons/gut/test.gd"

class TestSound:
	extends "res://addons/gut/test.gd"
	
	var sound: FmodSound
	var music: FmodSound
	var sound_file = "res://assets/Sounds/doorOpen_1.ogg"
	var music_file = "res://assets/Music/jingles_SAX07.ogg"
	
	func before_all():
		FmodServer.load_file_as_sound(sound_file)
		FmodServer.load_file_as_music(music_file)
		sound = FmodServer.create_sound_instance(sound_file)
		music = FmodServer.create_sound_instance(music_file)
	
	func after_all():
		sound.release()
		music.release()
		FmodServer.unload_file(sound_file)
		FmodServer.unload_file(music_file)
	
	func test_assert_set_volume():
		var desired_value: float = 2.0
		sound.volume = desired_value
		assert_eq(sound.volume, desired_value, "Sound volume should be 2")
		music.volume = desired_value
		assert_eq(music.volume, desired_value, "Sound volume should be 2")
	
	func test_assert_set_pitch():
		var desired_value: float = 0.75
		sound.pitch = desired_value
		assert_eq(sound.pitch, desired_value, "Sound pitch should be 0.75")
		music.pitch = desired_value
		assert_eq(music.pitch, desired_value, "Sound pitch should be 0.75")	
	
	func test_assert_playing():
		sound.play()
		assert_true(sound.is_playing(), "Sound should be playing")
		music.play()
		assert_true(music.is_playing(), "Sound should be playing")
