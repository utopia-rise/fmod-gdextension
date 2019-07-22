extends Area2D

# Declare member variables here. Examples:
# var a = 2
var open_sound: int
var close_sound: int

# Called when the node enters the scene tree for the first time.
func _ready():
	open_sound = Fmod.load_sound("./assets/Sounds/doorOpen_1.ogg", Fmod.FMOD_DEFAULT)
	close_sound = Fmod.load_sound("./assets/Sounds/doorClose_1.ogg", Fmod.FMOD_DEFAULT)
# warning-ignore:return_value_discarded
	connect("body_entered", self, "enter")
# warning-ignore:return_value_discarded
	connect("body_exited", self, "leave")

# warning-ignore:unused_argument
func enter(area):
	print("enter")
	Fmod.set_sound_pitch(open_sound, rand_range(0.75,1.25))
	Fmod.play_sound(open_sound)
	
# warning-ignore:unused_argument
func leave(area):
	print("leave")
	Fmod.set_sound_pitch(close_sound, rand_range(0.75,1.5))
	Fmod.play_sound(close_sound)
	

func _exit_tree():
	Fmod.release_sound(open_sound)
	Fmod.release_sound(close_sound)
