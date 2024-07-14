extends Button


@export var welcome_option_button: ChooseLanguageButton

func _enter_tree():
	connect("pressed", _on_pressed)

func _on_pressed():
	if welcome_option_button.current_bank_loader == null:
		return
	var event_emitter = FmodEventEmitter2D.new()
	event_emitter.event_guid = "{9aa2ecc5-ea4b-4ebe-85c3-054b11b21dcd}"
	event_emitter.autoplay = true
	event_emitter.set_programmer_callback("welcome")
	welcome_option_button.current_bank_loader.add_child(event_emitter)
