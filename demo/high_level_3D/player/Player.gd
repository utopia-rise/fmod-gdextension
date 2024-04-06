extends CharacterBody3D

var direction := Vector3(0, 0, 0) # Used for animation
var will_jump := false

const JUMP := 4
const PLAYER_MOVE_SPEED := 4

@onready var Camera = $Camera3D
@onready var GRAVITY = ProjectSettings.get("physics/3d/default_gravity") / 1000

func do_jump() -> void:
	if not self.is_on_floor():
		return

	if self.will_jump:
		return
	will_jump = true

	await get_tree().create_timer(0.05).timeout

	self.velocity.y += JUMP

	await get_tree().create_timer(0.1).timeout
	will_jump = false


func _process(_delta: float) -> void:
	"""
	Allow the player to move the camera with WASD
	See Project settings -> Input map for keyboard bindings
	"""
	if Input.is_action_just_pressed("space"):
		self.do_jump()

	var amount: float = 1

	if not is_on_floor() or will_jump:
		amount = 0.2

	if Input.is_action_pressed("up"):
		self.direction.z -= amount

	elif Input.is_action_pressed("down"):
		self.direction.z += amount

	if Input.is_action_pressed("left"):
		self.direction.x -= amount

	elif Input.is_action_pressed("right"):
		self.direction.x += amount

	self.direction = self.direction.clamp(Vector3(-1, -1, -1), Vector3(1, 1, 1))


func _physics_process(delta: float) -> void:
	# Apply friction
	if self.is_on_floor():
		self.direction *= Vector3.ONE - Vector3(0.9, 1.0, 0.9) * (10 * delta)

	# Preserve the Y velocity from the previous frame
	self.velocity = Vector3(0, self.velocity.y, 0)

	# Always add velocity even when we're in the air
	self.velocity += get_transform().basis.x * direction.x * PLAYER_MOVE_SPEED
	self.velocity += get_transform().basis.z * direction.z * PLAYER_MOVE_SPEED

	# Apply less gravity if we were on the floor last frame
	# This helps our KinematicBody to avoid physics jitter
	if self.is_on_floor():
		self.velocity -= Vector3(0, GRAVITY / 100, 0)
	else:
		self.velocity -= Vector3(0, GRAVITY, 0)

	self.move_and_slide()

	for i in get_slide_collision_count():
		var collision = get_slide_collision(i)
		var collider = collision.get_collider()
		if not collider is RigidBody3D:
			continue

		collider.apply_central_impulse(-collision.get_normal() * 0.8)
		collider.apply_impulse(-collision.get_normal() * 0.01, collision.get_position())
