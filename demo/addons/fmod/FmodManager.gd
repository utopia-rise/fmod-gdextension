extends Node

func _process(delta):
	Fmod.update()
	
func _notification(what):
	Fmod.notification(what)
