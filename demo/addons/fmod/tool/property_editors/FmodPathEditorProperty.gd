class_name FmodPathEditorProperty extends EditorProperty

var ui: Control
var guid_property: String
var path_property: String

func _init(open_project_explorer_callable: Callable, path_prop: String, guid_prop: String):
	guid_property = guid_prop
	path_property = path_prop
	ui = load("res://addons/fmod/tool/property_editors/FmodGuidAndPathPropertyEditorUi.tscn").instantiate()
	add_child(ui)
	var event_explorer_button: Button = ui.get_node("%EventExplorerButton")
	
	var open_project_explorer_event = func open_project_explorer_event():
		open_project_explorer_callable.call(self._set_path_and_guid)
	event_explorer_button.pressed.connect(open_project_explorer_event)
	ui.get_node("%PathLineEdit").text_changed.connect(_set_path)
	ui.get_node("%GuidLineEdit").text_changed.connect(_set_guid)

func _update_property():
	ui.get_node("%PathLineEdit").text = get_edited_object()[path_property]
	ui.get_node("%GuidLineEdit").text = get_edited_object()[guid_property]

func _set_path(path: String):
	emit_changed(path_property, path)

func _set_guid(guid: String):
	emit_changed(guid_property, guid)

func _set_path_and_guid(path: String, guid: String):
	_set_path(path)
	_set_guid(guid)
