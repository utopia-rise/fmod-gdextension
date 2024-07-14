extends Node2D


func _process(delta: float) -> void:
	var time = Time.get_ticks_msec()/1000.0
	self.position.x = 700 + 300 * sin(time)
