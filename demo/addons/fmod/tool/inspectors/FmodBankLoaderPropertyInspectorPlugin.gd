class_name FmodBankLoaderPropertyInspectorPlugin extends EditorInspectorPlugin

static var bank_icon = load("res://addons/fmod/icons/bank_icon.svg")

var _open_project_explorer_callable: Callable

func _init(plugin: FmodPlugin):
	_open_project_explorer_callable = plugin.open_project_explorer_bank

func _can_handle(object: Object):
	return object is FmodBankLoader

func _parse_property(object: Object, type: Variant.Type, name: String, hint_type: PropertyHint, hint_string: String, usage_flags: int, wide: bool):
	return name == "bank_paths"

func _parse_category(object: Object, category: String):
	if category != "FmodBankLoader":
		return
	var editor_property := FmodBankPathEditorProperty.new(_open_project_explorer_callable)
	add_property_editor("bank_paths", editor_property, false, "Fmod banks")
