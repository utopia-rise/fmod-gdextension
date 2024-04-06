extends Sprite2D

var isPlaying: bool = true
var event: FmodEvent = null

# Called when the node enters the scene tree for the first time.
func _ready():
	event = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
	event.set_2d_attributes(self.global_transform)
	event.set_parameter_by_name("RPM", 600)
	event.set_volume( 2)
	event.start()

# warning-ignore:unused_argument
func _process(_delta):
	if Input.is_action_just_pressed("space"):
		isPlaying = !isPlaying
		if(isPlaying):
			print("Mower playing")
			event.set_paused(false)
		else:
			print("Mower paused")
			event.set_paused(true)
	elif Input.is_action_just_pressed("kill_event"):
		self.queue_free()
	var time = Time.get_ticks_msec()/1000.0
	self.position.x = 300 * sin(time)
	event.set_2d_attributes(self.global_transform)

