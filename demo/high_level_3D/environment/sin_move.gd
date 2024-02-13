extends RigidBody3D

func _process(delta: float) -> void:
    var time = Time.get_ticks_msec()/1000.0
    self.position.x = 10 * sin(time)
