extends FmodEventEmitter3D



func _process(delta: float) -> void:
	if Input.is_action_just_pressed("kill"):
		self.queue_free()
