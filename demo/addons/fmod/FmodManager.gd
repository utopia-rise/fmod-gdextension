@tool
extends Node

var performance_display: PerformancesDisplay

func _ready():
	process_mode = PROCESS_MODE_ALWAYS
	performance_display = PerformancesDisplay.new()
	add_child(performance_display)

func _exit_tree() -> void:
	remove_child(performance_display)
	performance_display.free()

func _process(delta):
	FmodServer.update()
	
func _notification(what):
	FmodServer.notification(what)

	if OS.has_feature("ios"):
		match what:
			NOTIFICATION_APPLICATION_FOCUS_OUT: FmodServer.mixer_suspend()
			NOTIFICATION_APPLICATION_FOCUS_IN: FmodServer.mixer_resume()
