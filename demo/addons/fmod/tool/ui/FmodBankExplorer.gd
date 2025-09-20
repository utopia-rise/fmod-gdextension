@tool class_name FmodBankExplorer extends Window

enum ToDisplayFlags {
	BUSES = 1,
	VCA = 2,
	EVENTS = 4
}

static var _fmod_icon = load("res://addons/fmod/icons/fmod_icon.svg")
static var _vca_icon = load("res://addons/fmod/icons/vca_icon.svg")
static var _bank_icon = load("res://addons/fmod/icons/bank_icon.svg")
static var _event_icon = load("res://addons/fmod/icons/event_icon.svg")
static var _bus_icon = load("res://addons/fmod/icons/bus_icon.svg")
static var _snapshot_icon = load("res://addons/fmod/icons/snapshot_icon.svg")

signal emit_path_and_guid(path: String, guid: String)

var tree: Tree
@onready var copy_path_button := %PathLabel.get_child(0)
@onready var copy_guid_button := %GuidLabel.get_child(0)

var should_display_copy_buttons = true
var should_display_select_button = false

var _current_select_callable: Callable

var base_color: Color
var contrast: float
var background_color: Color

var banks: Array = Array()

var flags: int = 0
var search: String = ""

func reset_search():
	%SearchField.text = ""
	search = ""

func _ready():
	var main_window_size = get_parent().get_window().size
	size = main_window_size * 0.5

	var copy_texture : Texture = EditorInterface.get_editor_theme().get_icon("ActionCopy", "EditorIcons")
	copy_guid_button.icon = copy_texture
	copy_path_button.icon = copy_texture
	copy_guid_button.visible = false
	copy_path_button.visible = false
	copy_path_button.pressed.connect(_on_copy_path_button)
	copy_guid_button.pressed.connect(_on_copy_guid_button)

	var emit_path_and_guid_callable = func emit_path_and_guid_callable():
		var selected_item = tree.get_selected()
		if selected_item == null:
			return
		var selected_fmod_element = selected_item.get_metadata(0)
		if selected_fmod_element == null:
			return

		var path = selected_fmod_element.get_godot_res_path() if selected_fmod_element is FmodBank else selected_fmod_element.get_path()
		emit_path_and_guid.emit(path, selected_fmod_element.get_guid())
	%SelectButton.pressed.connect(emit_path_and_guid_callable)
	%SelectButton.pressed.connect(close_window)
	%CloseButton.pressed.connect(close_window)
	close_requested.connect(close_window)

	tree = %Tree
	tree.item_selected.connect(_on_item_selected)
	tree.columns = 1

	generate_tree()
	%RefreshBanksButton.pressed.connect(on_refresh_banks_button_pressed)


func regenerate_tree(callable: Callable = Callable()):
	tree.clear()
	generate_tree(callable)

func generate_tree(callable: Callable = Callable()):
	%SelectButton.visible = should_display_select_button
	if _current_select_callable != Callable() && _current_select_callable.get_object() != null:
		emit_path_and_guid.disconnect(_current_select_callable)
	_current_select_callable = callable

	var root_item := tree.create_item()
	root_item.set_text(0, "Fmod objects")
	root_item.set_icon(0, _fmod_icon)

	var has_many_flags = (flags & (flags - 1)) != 0

	for bank in FmodServer.get_all_banks():
		var fmod_bank := bank as FmodBank

		var buses := fmod_bank.get_bus_list()
		var vcas := fmod_bank.get_vca_list()
		var events := fmod_bank.get_description_list()

		if search.is_empty():
			var bank_item := tree.create_item(root_item)
			bank_item.set_text(0, fmod_bank.get_godot_res_path())
			bank_item.set_icon(0, _bank_icon)
			bank_item.set_metadata(0, bank)

			if flags & ToDisplayFlags.BUSES and buses.size() != 0:
				buses.sort_custom(sort_by_path)
				if has_many_flags:
					var buses_item := tree.create_item(bank_item)
					buses_item.set_text(0, "Buses")
					buses_item.set_icon(0, _bus_icon)
					_add_elements_as_tree(buses, buses_item)
				else:
					_add_elements_as_tree(buses, bank_item)

			if flags & ToDisplayFlags.VCA and vcas.size() != 0:
				vcas.sort_custom(sort_by_path)
				if has_many_flags:
					var vca_item := tree.create_item(bank_item)
					vca_item.set_text(0, "VCAs")
					vca_item.set_icon(0, _vca_icon)
					_add_elements_as_tree(vcas, vca_item)
				else:
					_add_elements_as_tree(vcas, bank_item)

			if flags & ToDisplayFlags.EVENTS and events.size() != 0:
				events.sort_custom(sort_by_path)
				if has_many_flags:
					var events_item := tree.create_item(bank_item)
					events_item.set_text(0, "Events")
					events_item.set_icon(0, _event_icon)
					_add_elements_as_tree(events, events_item)
				else:
					_add_elements_as_tree(events, bank_item)

		else:
			if flags & ToDisplayFlags.BUSES:
				_add_elements_as_items(buses, root_item)
			if flags & ToDisplayFlags.VCA:
				_add_elements_as_items(vcas, root_item)
			if flags & ToDisplayFlags.EVENTS:
				_add_elements_as_items(events, root_item)

	if copy_path_button.visible:
		copy_path_button.visible = should_display_copy_buttons

	if copy_guid_button.visible:
		copy_guid_button.visible = should_display_copy_buttons

	if _current_select_callable != Callable():
		print(_current_select_callable)
		emit_path_and_guid.connect(_current_select_callable)

	%SelectButton.visible = should_display_select_button and %GuidLabel.text != ""

func _add_elements_as_items(elements: Array, parent: TreeItem):
		for element in elements:
			var full_path: String = element.get_path()
			if not full_path.containsn(search):
				continue

			var child := tree.create_item(parent)
			var name := full_path.rsplit("/")[-1]
			child.set_text(0, name)
			child.set_metadata(0, element)
			child.set_icon(0, _get_icon_for_fmod_path(full_path))

func _add_elements_as_tree(elements: Array, parent: TreeItem):
	var nodes := { "": parent }

	for element in elements:
		var full_path: String = element.get_path()
		var parts := full_path.split("/")
		# Drop the “type:” prefix
		if parts.size() > 0:
			parts.remove_at(0)

		# Walk each segment in turn, building a running “key”
		var key := ""
		for i in range(parts.size()):
			var name = parts[i]
			if full_path == "bus:/":
				name = "Master"
			if key == "":
				key = name
			else:
				key = key + "/" + name

			# If we haven’t created this node yet, do so now
			if not nodes.has(key):
				var root_and_name := key.rsplit("/", false, 1)
				var parent_key: String = ""
				if root_and_name.size() == 2:
					parent_key = root_and_name[0]
				var parent_item = nodes[parent_key]
				var child := tree.create_item(parent_item)
				child.set_text(0, name)
				nodes[key] = child

			# If this is the final segment, attach the metadata & icon
			if i == parts.size() - 1:
				var leaf = nodes[key]
				leaf.set_metadata(0, element)
				leaf.set_icon(0, _get_icon_for_fmod_path(full_path))

func _on_item_selected():
	var metadata = tree.get_selected().get_metadata(0)
	if metadata == null or !metadata.get_guid():
		%PathsBG.hide()
		%EventPlayControls.hide()
		copy_path_button.visible = false
		copy_guid_button.visible = false
		%SelectButton.visible = false
		%ParametersLabel.visible = false
		%ParametersContainer.visible = false
		return
	%GuidLabel.set_text(metadata.get_guid())
	%PathLabel.set_text(metadata.get_path())
	%PathsBG.show()
	if should_display_copy_buttons:
		copy_path_button.visible = true
		copy_guid_button.visible = true
	if should_display_select_button:
		%SelectButton.visible = true

	if metadata is FmodEventDescription:
		%EventPlayControls.set_fmod_event(metadata)
		var _show_parameter_controls : bool = %EventParametersDisplay.set_fmod_event(metadata)
		%ParametersLabel.visible = _show_parameter_controls
		%ParametersContainer.visible = _show_parameter_controls
		return
	%EventPlayControls.hide()
	%EventParametersDisplay.hide()
	%ParametersLabel.visible = false
	%ParametersContainer.visible = false

func _on_copy_path_button():
	DisplayServer.clipboard_set(%PathLabel.text)

func _on_copy_guid_button():
	DisplayServer.clipboard_set(%GuidLabel.text)


func on_refresh_banks_button_pressed() -> void:
	# unload banks
	banks.clear()
	tree.clear()
	FmodBankDatabase.reload_all_banks()
	generate_tree()

func close_window():
	%EventPlayControls.stop_event()
	visible = false

static func _get_icon_for_fmod_path(fmod_path: String) -> Texture2D:
	var icon: Texture2D = null
	if fmod_path.begins_with("bus:/"):
		icon = _bus_icon
	elif fmod_path.begins_with("event:/"):
		icon = _event_icon
	elif fmod_path.begins_with("vca:/"):
		icon = _vca_icon
	elif fmod_path.begins_with("snapshot:/"):
		icon = _snapshot_icon
	return icon

static func sort_by_path(a, b):
	return a.get_path().casecmp_to(b.get_path()) < 0


func _on_text_edit_text_submitted(new_text: String) -> void:
	search = new_text
	regenerate_tree()
