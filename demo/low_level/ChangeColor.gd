extends Area2D

var event: FmodEvent = null
var icon: Sprite2D

# Called when the node enters the scene tree for the first time.
func _ready():
	event = FmodServer.create_event_instance("event:/Music/Level 02")
	event.set_callback(Callable(self, "change_color"), FmodServer.FMOD_STUDIO_EVENT_CALLBACK_ALL)
	body_entered.connect(enter)
	body_exited.connect(leave)
	event.start()
	event.set_paused(true)

# warning-ignore:unused_argument
func enter(_area):
	print("enter")
	event.set_paused(false)
	
# warning-ignore:unused_argument
func leave(_area):
	print("leave")
	event.set_paused(true)

# warning-ignore:unused_argument
func change_color(_dict: Dictionary, type: int):
	if type == FmodServer.FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT:
		$icon.self_modulate = Color(randf_range(0,1), randf_range(0,1), randf_range(0,1), 1)
