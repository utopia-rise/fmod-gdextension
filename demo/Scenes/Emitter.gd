extends Node

# Called when the node enters the scene tree for the first time.
func _ready():
	var id = Fmod.create_event_instance("event:/Music/Level 02")
	Fmod.start_event(id)
	print("Event Started.")
	return