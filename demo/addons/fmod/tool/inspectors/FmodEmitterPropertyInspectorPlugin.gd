class_name FmodEmitterPropertyInspectorPlugin extends EditorInspectorPlugin

var _open_project_explorer_callable: Callable
var _event_editor_property_scene: PackedScene = load("res://addons/fmod/tool/property_editors/FmodEventEditorProperty.tscn")

func _init(plugin: FmodPlugin):
	_open_project_explorer_callable = plugin.open_project_explorer_events

func _can_handle(object: Object):
	return object is FmodEventEmitter2D or \
	object is FmodEventEmitter3D

func _parse_property(object: Object, type: Variant.Type, name: String, hint_type: PropertyHint, hint_string: String, usage_flags: int, wide: bool):
	return name == "event_name" || name == "event_guid"

func _parse_category(object: Object, category: String):
	if category != "FmodEventEmitter2D" and category != "FmodEventEmitter3D":
		return
	var editor_property := _event_editor_property_scene.instantiate()
	editor_property.initialize(_open_project_explorer_callable, "event_name", "event_guid")
	add_property_editor_for_multiple_properties("Fmod event", PackedStringArray(["event_name", "event_guid"]), editor_property)
