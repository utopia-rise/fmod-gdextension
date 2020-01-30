extends Node2D

# Called when the node enters the scene tree for the first time.
func _ready():
	# register listener
	Fmod.add_listener(1, self)
	print("Listener set.")
	return
