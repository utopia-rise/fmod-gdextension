extends GdUnitTestSuite

const SOUND_FILE = "res://assets/Sounds/doorOpen_1.ogg"
const MUSIC_FILE = "res://assets/Music/jingles_SAX07.ogg"

var sound: FmodSound
var music: FmodSound

func before():
	FmodServer.load_file_as_sound(SOUND_FILE)
	FmodServer.load_file_as_music(MUSIC_FILE)
	sound = FmodServer.create_sound_instance(SOUND_FILE)
	music = FmodServer.create_sound_instance(MUSIC_FILE)

func after():
	sound.release()
	music.release()
	FmodServer.unload_file(SOUND_FILE)
	FmodServer.unload_file(MUSIC_FILE)

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

func test_assert_playing():
	sound.play()
	assert_bool(sound.is_playing()).is_true()
	music.play()
	assert_bool(music.is_playing()).is_true()
