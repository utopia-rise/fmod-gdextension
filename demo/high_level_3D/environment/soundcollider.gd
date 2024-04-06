extends FmodEventEmitter3D

var moving := false

func _process(_delta: float):

    var parent: RigidBody3D = get_parent()
    if moving == false and parent.linear_velocity.length() > 1:
        moving = true
        self.play()
    elif parent.linear_velocity.length() < 1:
        moving = false
