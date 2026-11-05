extends Area2D

# Declare member variables here. Examples:
# var a = 2
var open_file: FmodFile = null
var close_file: FmodFile = null
var open_sound: FmodSound = null
var close_sound: FmodSound = null

# Called when the node enters the scene tree for the first time.
func _ready():
	open_file = FmodServer.load_file_as_sound("res://assets/Sounds/doorOpen_1.ogg")
	close_file = FmodServer.load_file_as_sound("res://assets/Sounds/doorClose_1.ogg")
# warning-ignore:return_value_discarded
	body_entered.connect(enter)
# warning-ignore:return_value_discarded
	body_exited.connect(leave)

# warning-ignore:unused_argument
func enter(_area):
	print("enter")
	open_sound = open_file.create_instance()
	open_sound.set_pitch(randf_range(0.75,1.25))
	open_sound.play()
	
# warning-ignore:unused_argument
func leave(_area):
	print("leave")
	close_sound = close_file.create_instance()
	close_sound.set_pitch(randf_range(0.75,1.5))
	close_sound.play()
