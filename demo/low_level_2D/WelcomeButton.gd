extends Button


@export var welcome_option_button_path: NodePath
var welcome_option_button: WelcomeOptionButton

func _enter_tree():
	connect("pressed", _on_pressed)

func _ready():
	welcome_option_button = get_node(welcome_option_button_path)

func _on_pressed():
	if welcome_option_button.current_index == -1:
		return
	var event_instance = FmodServer.create_event_instance("event:/Character/Dialogue")
	event_instance.set_programmer_callback("welcome")
	event_instance.start()
