extends Node

func _ready():
	process_mode = PROCESS_MODE_ALWAYS

func _process(delta):
	FmodServer.update()
	
func _notification(what):
	FmodServer.notification(what)
