@tool class_name FmodPathEditorProperty extends EditorProperty

var ui: Control
var guid_property: String
var path_property: String
var regex := RegEx.new()

var default_line_edit_modulate: Color

func initialize(open_project_explorer_callable: Callable, path_prop: String, guid_prop: String):
    regex.compile("{\\w{8}-\\w{4}-\\w{4}-\\w{4}-\\w{12}}")
    guid_property = guid_prop
    path_property = path_prop
    var guid_and_path_ui: FmodGuidAndPathPropertyEditorUi = %FmodGuidAndPathPropertyEditorUi
    
    default_line_edit_modulate = guid_and_path_ui.get_node("%GuidLineEdit").modulate
    
    var open_project_explorer_event = func open_project_explorer_event():
        open_project_explorer_callable.call(self._set_path_and_guid)
    guid_and_path_ui.set_callables(open_project_explorer_event, _set_path, _set_guid)

func _update_property():
    var guid_and_path_ui = %FmodGuidAndPathPropertyEditorUi
    guid_and_path_ui.get_node("%PathLineEdit").text = get_edited_object()[path_property]
    guid_and_path_ui.get_node("%GuidLineEdit").text = get_edited_object()[guid_property]

func _set_path(path: String):
    emit_changed(path_property, path)

func _set_guid(guid: String):
    var line_edit := %FmodGuidAndPathPropertyEditorUi.get_node("%GuidLineEdit") as LineEdit
    if not regex.search(guid):
        line_edit.modulate = Color.RED
        return
    line_edit.modulate = default_line_edit_modulate
    emit_changed(guid_property, guid)

func _set_path_and_guid(path: String, guid: String):
    _set_path(path)
    _set_guid(guid)
