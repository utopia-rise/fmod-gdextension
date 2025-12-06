extends Label

func _ready():
	set_process(true)
	
func _process(_delta: float):
	self.text = 'FPS: %s' % Engine.get_frames_per_second()
