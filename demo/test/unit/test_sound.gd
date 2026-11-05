extends GdUnitTestSuite

const SOUND_FILE = "res://assets/Sounds/doorOpen_1.ogg"
const MUSIC_FILE = "res://assets/Music/jingles_SAX07.ogg"

var sound: FmodSound
var music: FmodSound

func before():
	sound = FmodServer.load_file_as_sound(SOUND_FILE).create_instance()
	music = FmodServer.load_file_as_music(MUSIC_FILE).create_instance()

func after():
	sound.release()
	music.release()
	sound = null
	music = null

func test_assert_set_volume():
	sound.volume = 2.0
	assert_float(sound.volume).is_equal(2.0)
	music.volume = 2.0
	assert_float(music.volume).is_equal(2.0)

func test_assert_set_pitch():
	sound.pitch = 0.75
	assert_float(sound.pitch).is_equal(0.75)
	music.pitch = 0.75
	assert_float(music.pitch).is_equal(0.75)

func test_assert_set_position():
	sound.position_3d = Vector3(1, 2, 3)
	assert_vector(sound.position_3d).is_equal_approx(Vector3(1, 2, 3), Vector3(0.001, 0.001, 0.001))
	sound.position_2d = Vector2(4, 5)
	assert_vector(sound.position_2d).is_equal_approx(Vector2(4, 5), Vector2(0.001, 0.001))
	assert_vector(sound.position_3d).is_equal_approx(Vector3(-4, 0, 5) / 64.0, Vector3(0.001, 0.001, 0.001))

func test_assert_distance_factors_convert_separately():
	FmodServer.set_distance_factors(10.0, 2.0)
	sound.position_3d = Vector3(2, 0, 0)
	sound.position_2d = Vector2(10, 0)
	assert_vector(sound.position_2d).is_equal_approx(Vector2(10, 0), Vector2(0.001, 0.001))
	assert_vector(sound.position_3d).is_equal_approx(Vector3(-2, 0, 0), Vector3(0.001, 0.001, 0.001))
	sound.position_3d = Vector3(2, 0, 0)
	assert_vector(sound.position_2d).is_equal_approx(Vector2(-10, 0), Vector2(0.001, 0.001))
	FmodServer.set_distance_factors(64.0, 1.0)

func test_assert_set_velocity():
	sound.velocity_3d = Vector3(1, 2, 3)
	assert_vector(sound.velocity_3d).is_equal_approx(Vector3(1, 2, 3), Vector3(0.001, 0.001, 0.001))
	sound.velocity_2d = Vector2(4, 5)
	assert_vector(sound.velocity_2d).is_equal_approx(Vector2(4, 5), Vector2(0.001, 0.001))

func test_assert_playing():
	sound.play()
	assert_bool(sound.is_playing()).is_true()
	music.play()
	assert_bool(music.is_playing()).is_true()

func test_assert_instance_keeps_its_file_loaded():
	var file := FmodServer.load_file_as_sound(SOUND_FILE)
	var instance := file.create_instance()
	file = null
	instance.play()
	assert_bool(instance.is_playing()).is_true()
	instance.release()
