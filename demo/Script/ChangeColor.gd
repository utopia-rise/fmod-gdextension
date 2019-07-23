extends Area2D

var id: int = 0

# Called when the node enters the scene tree for the first time.
func _ready():
# warning-ignore:return_value_discarded
	Fmod.connect("timeline_beat", self, "change_color")
# warning-ignore:return_value_discarded
	self.connect("body_entered", self, "enter")
# warning-ignore:return_value_discarded
	self.connect("body_exited", self, "leave")
	id = Fmod.create_event_instance("event:/Music/Level 02")
	Fmod.set_callback(id, Fmod.FMOD_STUDIO_EVENT_CALLBACK_ALL)
	Fmod.start_event(id)
	Fmod.set_event_paused(id, true)

# warning-ignore:unused_argument
func enter(area):
	print("enter")
	Fmod.set_event_paused(id, false)
	
# warning-ignore:unused_argument
func leave(area):
	print("leave")
	Fmod.set_event_paused(id, true)

# warning-ignore:unused_argument
func change_color(dict: Dictionary):
	$icon.self_modulate = Color(rand_range(0,1), rand_range(0,1), rand_range(0,1), 1)
