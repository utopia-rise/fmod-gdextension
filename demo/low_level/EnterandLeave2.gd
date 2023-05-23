extends Area2D

# Declare member variables here. Examples:
# var a = 2
var music: FmodSound = null

# Called when the node enters the scene tree for the first time.
func _ready():
	FmodServer.load_file_as_music("res://assets/Music/jingles_SAX07.ogg")
# warning-ignore:return_value_discarded
	body_entered.connect(enter)
# warning-ignore:return_value_discarded
	body_exited.connect(leave)

# warning-ignore:unused_argument
func enter(_area):
	print("enter")
	music = FmodServer.create_sound_instance("res://assets/Music/jingles_SAX07.ogg")
	music.play()
	
# warning-ignore:unused_argument
func leave(_area):
	print("leave")
	music.release()

func _exit_tree():
	FmodServer.unload_file("res://assets/Music/jingles_SAX07.ogg")
