extends Area2D

# Declare member variables here. Examples:
# var a = 2
var music: int = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	FmodServer.load_file_as_music("res://assets/Music/jingles_SAX07.ogg")
# warning-ignore:return_value_discarded
	body_entered.connect(enter)
# warning-ignore:return_value_discarded
	body_exited.connect(leave)

# warning-ignore:unused_argument
func enter(area):
	print("enter")
	music = FmodServer.create_sound_instance("res://assets/Music/jingles_SAX07.ogg")
	FmodServer.play_sound(music)
	
# warning-ignore:unused_argument
func leave(area):
	print("leave")
	FmodServer.release_sound(music)

func _exit_tree():
	FmodServer.unload_file("res://assets/Music/jingles_SAX07.ogg")
