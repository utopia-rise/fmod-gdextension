extends FmodEventEmitter3D

func _process(_delta: float):
	var parent: RigidBody3D = get_parent()
	
	# Set velocity for Doppler effects and spatial audio
	set_velocity(parent.linear_velocity)
	
	# Set parameter based on angular velocity for engine sound
	var value := parent.angular_velocity.length() / 2
	if value < 0.1:
		value = 0
	self["fmod_parameters/Speed"] = value
