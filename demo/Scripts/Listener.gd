extends Sprite

export var speed = 125

func _ready():
	pass
	
func _physics_process(delta):
	if Input.is_action_pressed("ui_right"):
		position.x = position.x + (speed * delta)
		
	if Input.is_action_pressed("ui_left"):
		position.x = position.x - (speed * delta)
		
	if Input.is_action_pressed("ui_up"):
		position.y = position.y - (speed * delta)
		
	if Input.is_action_pressed("ui_down"):
		position.y = position.y + (speed * delta)
