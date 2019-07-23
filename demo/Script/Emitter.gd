extends Sprite

var isPlaying: bool = true
var id: int = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	id = Fmod.create_event_instance("event:/Vehicles/Car Engine")
	Fmod.attach_instance_to_node(id, self)
	Fmod.set_event_parameter(id, "RPM", 600)
	Fmod.set_event_volume(id, 2)
	Fmod.start_event(id)
	print("Event Started.")
	
# warning-ignore:unused_argument
func _process(delta):
	if Input.is_action_just_pressed("space"):
		isPlaying = !isPlaying
		if(isPlaying):
			print("Mower playing")
			Fmod.set_event_paused(id, false)
		else:
			print("Mower paused")
			Fmod.set_event_paused(id, true)
