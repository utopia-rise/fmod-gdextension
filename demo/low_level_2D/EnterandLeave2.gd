extends Area2D

# Declare member variables here. Examples:
# var a = 2
var music_file: FmodFile = null
var music: FmodSound = null

# Called when the node enters the scene tree for the first time.
func _ready():
	music_file = FmodServer.load_file_as_music("res://assets/Music/jingles_SAX07.ogg")
# warning-ignore:return_value_discarded
	body_entered.connect(enter)
# warning-ignore:return_value_discarded
	body_exited.connect(leave)

# warning-ignore:unused_argument
func enter(_area):
	print("enter")
	music = music_file.create_instance()
	music.play()
	
# warning-ignore:unused_argument
func leave(_area):
	print("leave")
	music.release()
