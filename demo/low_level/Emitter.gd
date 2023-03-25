extends Sprite2D

var isPlaying: bool = true
var id: int = 0

# Called when the node enters the scene tree for the first time.
func _ready():
	id = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
	FmodServer.attach_instance_to_node(id, self)
	FmodServer.set_event_parameter_by_name(id, "RPM", 600)
	FmodServer.set_event_volume(id, 2)
	FmodServer.start_event(id)
	
# warning-ignore:unused_argument
func _process(delta):
	if Input.is_action_just_pressed("space"):
		isPlaying = !isPlaying
		if(isPlaying):
			print("Mower playing")
			FmodServer.set_event_paused(id, false)
		else:
			print("Mower paused")
			FmodServer.set_event_paused(id, true)
	elif Input.is_action_just_pressed("kill_event"):
		self.queue_free()
