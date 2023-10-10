@tool class_name EventParametersDisplay extends ScrollContainer

static var parameter_display_scene: PackedScene = load("res://addons/fmod/tool/ui/ParameterDisplay.tscn")

func set_fmod_event(event: FmodEventDescription):
	for child in %ParameterDisplaysContainer.get_children():
		%ParameterDisplaysContainer.remove_child(child)
		child.queue_free()
	
	var event_parameters: Array = event.get_parameters()
	for parameter in event_parameters:
		var event_parameter := parameter as FmodParameterDescription
		print(event_parameter.get_name())
		var parameter_display: ParameterDisplay = parameter_display_scene.instantiate()
		parameter_display.set_parameter(event_parameter)
		%ParameterDisplaysContainer.add_child(parameter_display)
		%ParameterDisplaysContainer.add_child(HSeparator.new())
