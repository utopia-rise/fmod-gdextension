@tool
extends PanelContainer

@export var play_button : Button
@export var stop_button : Button
@export var fade_out_toggle : CheckButton
var event_instance : FmodEvent

func _ready() -> void:
	hide()
	play_button.icon = EditorInterface.get_editor_theme().get_icon("Play", "EditorIcons")
	stop_button.icon = EditorInterface.get_editor_theme().get_icon("Stop", "EditorIcons")
	play_button.pressed.connect(on_play_button_pressed)
	stop_button.pressed.connect(on_stop_button_pressed)


func set_fmod_event(_event_description : FmodEventDescription) -> void:
	stop_and_release_instance() # always stop and release if a previous one is active
	event_instance = FmodServer.create_event_instance_with_guid(_event_description.get_guid())
	show()
	
	
func play_event() -> void:
	if event_instance:
		event_instance.start()
	
	
func stop_event() -> void:
	if event_instance:
		var stop_mode : int = FmodServer.FMOD_STUDIO_STOP_IMMEDIATE
		if fade_out_toggle.button_pressed:
			stop_mode = FmodServer.FMOD_STUDIO_STOP_ALLOWFADEOUT
			
		event_instance.stop(stop_mode)
	
	
func _exit_tree() -> void:
	stop_and_release_instance()
	
	
func stop_and_release_instance() -> void:
	if event_instance:
		event_instance.stop(0)
		event_instance.release()


func on_play_button_pressed() -> void:
	play_event()
	
	
func on_stop_button_pressed() -> void:
	stop_event()
