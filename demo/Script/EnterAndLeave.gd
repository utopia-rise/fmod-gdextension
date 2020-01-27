extends Area2D

# Declare member variables here. Examples:
# var a = 2
var open_sound: int = 0
var close_sound: int = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	Fmod.load_file_as_sound("res://assets/Sounds/doorOpen_1.ogg")
	Fmod.load_file_as_sound("res://assets/Sounds/doorClose_1.ogg")
# warning-ignore:return_value_discarded
	connect("body_entered", self, "enter")
# warning-ignore:return_value_discarded
	connect("body_exited", self, "leave")

# warning-ignore:unused_argument
func enter(area):
	print("enter")
	open_sound = Fmod.create_sound_instance("res://assets/Sounds/doorOpen_1.ogg")
	Fmod.set_sound_pitch(open_sound, rand_range(0.75,1.25))
	Fmod.play_sound(open_sound)
	
# warning-ignore:unused_argument
func leave(area):
	print("leave")
	close_sound = Fmod.create_sound_instance("res://assets/Sounds/doorClose_1.ogg")
	Fmod.set_sound_pitch(close_sound, rand_range(0.75,1.5))
	Fmod.play_sound(close_sound)
	

func _exit_tree():
	Fmod.unload_file("res://assets/Sounds/doorOpen_1.ogg")
	Fmod.unload_file("res://assets/Sounds/doorClose_1.ogg")
