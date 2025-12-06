class_name FmodBankPathEditorProperty extends EditorProperty

var path_property_name := "bank_paths"

var ui: Control
var last_selected_index := -1

func _init(open_project_explorer_callable: Callable):
	ui = load("res://addons/fmod/tool/property_editors/FmodBankPathsPropertyEditorUi.tscn").instantiate()
	add_child(ui)
	var add_button: Button = ui.get_node("%AddButton")
	
	var open_project_explorer_event = func open_project_explorer_event():
		open_project_explorer_callable.call(self._set_path_and_guid)
	add_button.pressed.connect(open_project_explorer_event)
	
	var remove_button: Button = ui.get_node("%RemoveButton")
	remove_button.pressed.connect(_on_remove_button)
	
	var manual_add_button: Button = ui.get_node("%ManualAddButton")
	manual_add_button.pressed.connect(_on_manual_add_button)
	
	var up_button: Button = ui.get_node("%UpButton")
	up_button.pressed.connect(_on_move_button.bind(false))
	
	var down_button: Button = ui.get_node("%DownButton")
	down_button.pressed.connect(_on_move_button.bind(true))

func _update_property():
	var bank_list: ItemList = ui.get_node("%BankList")
	bank_list.clear()
	var bank_paths: Array = get_edited_object()[path_property_name]
	for path in bank_paths:
		bank_list.add_item(path)
	
	if last_selected_index == -1:
		return
	bank_list.select(last_selected_index)
	last_selected_index = -1

func _set_path_and_guid(path: String, _cancel: String):
	var current_bank_paths: Array = get_edited_object()[path_property_name]
	
	if current_bank_paths.has(path):
		return
	
	var bank_paths := Array(current_bank_paths)
	bank_paths.append(path)
	emit_changed(path_property_name, bank_paths)

func _on_remove_button():
	var bank_list: ItemList = ui.get_node("%BankList")
	var current_bank_paths: Array = get_edited_object()[path_property_name]
	var bank_paths := Array(current_bank_paths)
	var selected_items_indexes: PackedInt32Array = bank_list.get_selected_items()
	if selected_items_indexes.is_empty():
		return
	var item = bank_list.get_item_text(selected_items_indexes[0])
	var in_list_index = bank_paths.find(item)
	bank_paths.remove_at(in_list_index)
	last_selected_index = in_list_index if in_list_index < bank_paths.size() else in_list_index - 1
	emit_changed(path_property_name, bank_paths)

func _on_manual_add_button():
	var manual_add_line_edit: LineEdit = ui.get_node("%ManualAddLineEdit")
	var to_add: String = manual_add_line_edit.text
	if not to_add.begins_with("res://") || not to_add.ends_with(".bank"):
		return
	_set_path_and_guid(to_add, "")
	manual_add_line_edit.text = ""

func _on_move_button(is_down: bool):
	var bank_list: ItemList = ui.get_node("%BankList")
	var current_bank_paths: Array = get_edited_object()[path_property_name]
	var bank_paths := Array(current_bank_paths)
	var selected_items_indexes: PackedInt32Array = bank_list.get_selected_items()
	if selected_items_indexes.is_empty():
		return
	var item = bank_list.get_item_text(selected_items_indexes[0])
	var in_list_index = bank_paths.find(item)
	var boundary = current_bank_paths.size() - 1 if is_down else 0
	if in_list_index == boundary:
		return
	var new_index = in_list_index + 1 if is_down else in_list_index - 1
	bank_paths.remove_at(in_list_index)
	bank_paths.insert(new_index, item)
	last_selected_index = new_index
	emit_changed(path_property_name, bank_paths)
