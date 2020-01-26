extends Area2D

# Declare member variables here. Examples:
# var a = 2
var music: int = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	Fmod.load_file_as_music("res://assets/Music/jingles_SAX07.ogg")
# warning-ignore:return_value_discarded
	connect("body_entered", self, "enter")
# warning-ignore:return_value_discarded
	connect("body_exited", self, "leave")

# warning-ignore:unused_argument
func enter(area):
	print("enter")
	music = Fmod.create_sound_instance("res://assets/Music/jingles_SAX07.ogg")
	Fmod.play_sound(music)
	
# warning-ignore:unused_argument
func leave(area):
	print("leave")
	Fmod.release_sound(music)

func _exit_tree():
	Fmod.unload_file("res://assets/Music/jingles_SAX07.ogg")
