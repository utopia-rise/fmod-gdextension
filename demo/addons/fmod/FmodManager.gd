extends Node

func _process(delta):
	FmodServer.update()
	
func _notification(what):
	FmodServer.notification(what)
