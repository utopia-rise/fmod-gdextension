@tool class_name FmodEventEditorProperty extends FmodPathEditorProperty


static var EVENT_PARAMETER_PREFIX_FOR_PROPERTIES = "event_parameter"

var former_event_description: FmodEventDescription

func _update_property():
	super()
	if get_edited_object().event_name == "":
		return
	_update_parameters()
	var event_description: FmodEventDescription = FmodServer.get_event_from_guid(get_edited_object().event_guid)
	former_event_description = event_description

func _set_path_and_guid(path: String, guid: String):
	super(path, guid)
	if get_edited_object().event_name == "":
		return
	_update_parameters()
	former_event_description = FmodServer.get_event_from_guid(get_edited_object().event_guid)

func _update_parameters():
	var event_description: FmodEventDescription = FmodServer.get_event_from_guid(get_edited_object().event_guid)
	
	if former_event_description != null and event_description != former_event_description:
		get_edited_object().tool_remove_all_parameters()
	
	var map_to_property_name = func map_to_property_name(dict: Dictionary):
			return dict["name"]
	var filter_fmod_parameter_property = func filter_fmod_parameter_property(parameter_name: String):
		return parameter_name.begins_with(EVENT_PARAMETER_PREFIX_FOR_PROPERTIES)
	
	var filter_property_id = func filter_property_id(property: String):
		return property.ends_with("/id")
	var existing_property_ids = get_edited_object().get_property_list().map(map_to_property_name).filter(filter_fmod_parameter_property).filter(filter_property_id)
	
	var map_property_name_to_parameter_name = func map_property_name_to_parameter_name(parameter: String):
		return parameter.split("/")[1]
	var existing_parameter_names = existing_property_ids.map(map_property_name_to_parameter_name)
	
	var map_property_id_to_parameter_id_value = func map_property_id_to_parameter_id_value(property: String):
		return get_edited_object()[property]
	var existing_parameter_ids = existing_property_ids.map(map_property_id_to_parameter_id_value)
	
	var property_matching = existing_parameter_ids.map(func(id): return false)
	
	for param in event_description.get_parameters():
		var parameter_name = param.get_name()
		var parameter_id_param = "%s/%s/id" % [EVENT_PARAMETER_PREFIX_FOR_PROPERTIES, parameter_name]
		var parameter_value_param = "%s/%s/value" % [EVENT_PARAMETER_PREFIX_FOR_PROPERTIES, parameter_name]
		var parameter_min_value_param = "%s/%s/min_value" % [EVENT_PARAMETER_PREFIX_FOR_PROPERTIES, parameter_name]
		var parameter_max_value_param = "%s/%s/max_value" % [EVENT_PARAMETER_PREFIX_FOR_PROPERTIES, parameter_name]
		var parameter_default_value_param = "%s/%s/default_value" % [EVENT_PARAMETER_PREFIX_FOR_PROPERTIES, parameter_name]
		
		var existing_property_name_index = existing_property_ids.find(parameter_id_param)
		var are_properties_already_in_node = existing_property_name_index != -1
		
		var parameter_id = param.get_id()
		
		if are_properties_already_in_node:
			property_matching[existing_property_name_index] = existing_parameter_ids[existing_property_name_index] == parameter_id
		
		if not are_properties_already_in_node or get_edited_object()[parameter_id_param] == null:
			get_edited_object()[parameter_id_param] = parameter_id
		if not are_properties_already_in_node or get_edited_object()[parameter_value_param] == null:
			get_edited_object()[parameter_value_param] = param.get_default_value()
		if not are_properties_already_in_node or get_edited_object()[parameter_min_value_param] == null:
			get_edited_object()[parameter_min_value_param] = param.get_minimum()
		if not are_properties_already_in_node or get_edited_object()[parameter_max_value_param] == null:
			get_edited_object()[parameter_max_value_param] = param.get_maximum()
		if not are_properties_already_in_node or get_edited_object()[parameter_default_value_param] == null:
			get_edited_object()[parameter_default_value_param] = param.get_default_value()
	
	for i in property_matching.size():
		if not property_matching[i]:
			get_edited_object().tool_remove_parameter(existing_parameter_ids[i])
		pass
	
	get_edited_object().notify_property_list_changed()
