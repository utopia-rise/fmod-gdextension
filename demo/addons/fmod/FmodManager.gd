extends Node

func _ready() -> void:
	process = PROCESS_MODE_PAUSABLE

func _process(delta):
	FmodServer.update()
	
func _notification(what):
	FmodServer.notification(what)
