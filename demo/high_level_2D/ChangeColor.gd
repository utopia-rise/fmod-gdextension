extends Area2D

var event: FmodEvent = null
var icon: Sprite2D

# Called when the node enters the scene tree for the first time.
func _ready():
	body_entered.connect(enter)
	body_exited.connect(leave)
	$FmodEventEmitter2D.paused = true

# warning-ignore:unused_argument
func enter(_area):
	print("enter")
	
	$FmodEventEmitter2D.paused = false
	
# warning-ignore:unused_argument
func leave(_area):
	print("leave")
	$FmodEventEmitter2D.paused = true

# warning-ignore:unused_argument
func change_color(_dict: Dictionary):
	$icon.self_modulate = Color(randf_range(0,1), randf_range(0,1), randf_range(0,1), 1)
