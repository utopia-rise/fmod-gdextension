class_name FmodEmitterPropertyInspectorPlugin extends EditorInspectorPlugin

var _open_project_explorer_callable: Callable

func _init(plugin: FmodPlugin):
	_open_project_explorer_callable = plugin.open_project_explorer_events

func _can_handle(object: Object):
	return object is FmodEventEmitter2D or \
	object is FmodEventEmitter3D

func _parse_property(object: Object, type: Variant.Type, name: String, hint_type: PropertyHint, hint_string: String, usage_flags: int, wide: bool):
	if name == "event_name" || name == "event_guid":
		return true
	return false

func _parse_category(object: Object, category: String):
	if category != "FmodEventEmitter2D":
		return
	var editor_property := FmodPathEditorProperty.new(_open_project_explorer_callable, "event_name", "event_guid")
	add_property_editor_for_multiple_properties("fmod_path", PackedStringArray(["event_name", "event_guid"]), editor_property)
