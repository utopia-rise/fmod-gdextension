extends Node

func _ready() -> void:
	process = PROCESS_MODE_ALWAYS

func _process(delta):
	FmodServer.update()
	
func _notification(what):
	FmodServer.notification(what)
