extends Area2D

var id: int = 0

# Called when the node enters the scene tree for the first time.
func _ready():
# warning-ignore:return_value_discarded
	FmodServer.timeline_beat.connect(change_color)
# warning-ignore:return_value_discarded
	body_entered.connect(enter)
# warning-ignore:return_value_discarded
	body_exited.connect(leave)
	id = FmodServer.create_event_instance("event:/Music/Level 02")
	FmodServer.set_callback(id, Fmod.FMOD_STUDIO_EVENT_CALLBACK_ALL)
	FmodServer.start_event(id)
	FmodServer.set_event_paused(id, true)

# warning-ignore:unused_argument
func enter(area):
	print("enter")
	FmodServer.set_event_paused(id, false)
	
# warning-ignore:unused_argument
func leave(area):
	print("leave")
	FmodServer.set_event_paused(id, true)

# warning-ignore:unused_argument
func change_color(dict: Dictionary):
	$icon.self_modulate = Color(randf_range(0,1), randf_range(0,1), randf_range(0,1), 1)
