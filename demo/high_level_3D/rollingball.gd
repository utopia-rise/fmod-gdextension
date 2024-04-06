extends FmodEventEmitter3D


func _process(_delta: float):

    var parent: RigidBody3D = get_parent()
    var value:= parent.angular_velocity.length() / 2
    if value < 0.1:
        value = 0
    self["event_parameter/Speed/value"] = value
