extends CharacterBody2D

var distance_traveled := 0
@export var footstep_scene: PackedScene

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
    if Input.is_action_pressed("rotate_right"):
        rotation_dir = 1
    if Input.is_action_pressed("rotate_left"):
        rotation_dir = -1
    if direction != Vector2(0,0):
        distance_traveled += delta * 200
        if distance_traveled >= 35:
            distance_traveled = 0
            add_child(footstep_scene.instantiate())
    direction = direction.normalized()
    direction.x = direction.x * delta * 200
    direction.y = direction.y * delta * 200
    self.position += direction
    self.rotate(rotation_dir * delta * 5)
    if Input.is_action_pressed("lock_listener"):
        $FmodListener2D.is_locked = !$FmodListener2D.is_locked
    elif Input.is_action_pressed("kill"):
        self.queue_free()

