@tool class_name FmodEventEditorProperty extends FmodPathEditorProperty


static var fmod_parameter_ui: PackedScene = load("res://addons/fmod/tool/property_editors/FmodParameterUi.tscn")

func initialize(open_project_explorer_callable: Callable, path_prop: String, guid_prop: String):
	super(open_project_explorer_callable, path_prop, guid_prop)

func _update_property():
	super()
	var event_description: FmodEventDescription = FmodServer.get_event_from_guid(get_edited_object().event_guid)
	
	for child in %ParametersContainer.get_children():
		%ParametersContainer.remove_child(child)
		child.queue_free()
	
	for param in event_description.get_parameters():
		var parameter_ui: FmodParameterUi = fmod_parameter_ui.instantiate()
		parameter_ui.set_parameter_name(param.get_name())
		parameter_ui.set_range(param.get_minimum(), param.get_maximum())
		%ParametersContainer.add_child(parameter_ui)
