extends KinematicBody2D

# Called when the node enters the scene tree for the first time.
func _ready():
	# register listener
	Fmod.add_listener(self)
	print("Listener set.")
	return
	
func _process(delta):
	var direction: Vector2 = Vector2(0,0)
	var rotation_dir = 0
	if Input.is_action_pressed("right"):
		direction.x += 1
	if Input.is_action_pressed("left"):
		direction.x -= 1
	if Input.is_action_pressed("up"):
		direction.y -= 1
	if Input.is_action_pressed("down"):
		direction.y += 1
	if Input.is_key_pressed(KEY_Q):
		rotation_dir = -0.1
	if Input.is_key_pressed(KEY_D):
		rotation_dir = 0.1
	direction = direction.normalized()
	direction.x = direction.x * delta * 200
	direction.y = direction.y * delta * 200
	self.position += direction
	self.rotate(rotation_dir)