@tool class_name FmodBankExplorer extends Window

enum ToDisplayFlags {
	BANKS = 1,
	BUSES = 2,
	VCA = 4,
	EVENTS = 8
}

static var _fmod_icon = load("res://addons/fmod/icons/fmod_icon.svg")
static var _vca_icon = load("res://addons/fmod/icons/vca_icon.svg")
static var _bank_icon = load("res://addons/fmod/icons/bank_icon.svg")
static var _event_icon = load("res://addons/fmod/icons/event_icon.svg")
static var _bus_icon = load("res://addons/fmod/icons/bus_icon.svg")
static var _snapshot_icon = load("res://addons/fmod/icons/snapshot_icon.svg")

signal emit_path_and_guid(path: String, guid: String)

var tree: Tree
var copy_path_button := Button.new()
var copy_guid_button := Button.new()

var should_display_copy_buttons = true
var should_display_select_button = false

var _current_select_callable: Callable

func _ready():
	var main_window_size = get_parent().get_window().size
	size = main_window_size * 0.5
	
	copy_path_button.text = "Copy"
	copy_guid_button.text = "Copy"
	copy_path_button.visible = false
	copy_guid_button.visible = false
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
	
	%ButtonsContainer.add_child(copy_path_button)
	%ButtonsContainer.add_child(copy_guid_button)
	
	tree = %Tree
	tree.item_selected.connect(_on_item_selected)
	
	tree.columns = 1
	regenerate_tree(ToDisplayFlags.BANKS | ToDisplayFlags.BUSES | ToDisplayFlags.VCA | ToDisplayFlags.EVENTS)

func regenerate_tree(to_display: int, callable: Callable = Callable()):
	%SelectButton.visible = should_display_select_button
	if _current_select_callable != Callable() && _current_select_callable.get_object() != null:
		emit_path_and_guid.disconnect(_current_select_callable)
	_current_select_callable = callable
	
	tree.clear()
	var root_item := tree.create_item()
	root_item.set_text(0, "Fmod objects")
	root_item.set_icon(0, _fmod_icon)
	
	for bank in FmodServer.get_all_banks():
		var fmod_bank := bank as FmodBank
		
		var bank_item := tree.create_item(root_item)
		bank_item.set_text(0, fmod_bank.get_godot_res_path())
		bank_item.set_icon(0, _bank_icon)
		bank_item.set_metadata(0, bank)
		
		if to_display & ToDisplayFlags.BUSES:
			var buses_item := tree.create_item(bank_item)
			buses_item.set_text(0, "Buses")
			buses_item.set_icon(0, _bus_icon)
			
			var buses := fmod_bank.get_bus_list()
			buses.sort_custom(sort_by_path)
			_add_elements_as_tree(buses, buses_item)
		
		if to_display & ToDisplayFlags.VCA:
			var vca_item := tree.create_item(bank_item)
			vca_item.set_text(0, "VCAs")
			vca_item.set_icon(0, _vca_icon)
			
			var vcas := fmod_bank.get_vca_list()
			vcas.sort_custom(sort_by_path)
			_add_elements_as_tree(vcas, vca_item)
		
		if to_display & ToDisplayFlags.EVENTS:
			var events_item := tree.create_item(bank_item)
			events_item.set_text(0, "Events")
			events_item.set_icon(0, _event_icon)
			
			var events := fmod_bank.get_description_list()
			events.sort_custom(sort_by_path)
			_add_elements_as_tree(events, events_item)
	
	if copy_path_button.visible:
		copy_path_button.visible = should_display_copy_buttons
	
	if copy_guid_button.visible:
		copy_guid_button.visible = should_display_copy_buttons
	
	if _current_select_callable != Callable():
		print(_current_select_callable)
		emit_path_and_guid.connect(_current_select_callable)
	
	%SelectButton.visible = should_display_select_button and %GuidLabel.text != ""

func _add_elements_as_tree(elements: Array, parent: TreeItem):
	var stack = Array()
	for element in elements:
		_add_element_to_stack(stack, parent, element)

func _add_element_to_stack(stack: Array, parent_root: TreeItem, path_element):
	var fmod_path: String = path_element.get_path()
	var path_parts := fmod_path.split("/")
	if path_parts[path_parts.size() - 1] == "":
		path_parts.remove_at(path_parts.size() - 1)
	
	for i in range(0, path_parts.size()):
		var path_part = path_parts[i]
		path_part = path_part if path_part != "bus:" else "Master"
		
		if i >= stack.size():
			var parent_item = parent_root if stack.is_empty() else stack[stack.size() - 1]
			var tree_item = tree.create_item(parent_item)
			tree_item.set_text(0, path_part)
			if i == path_parts.size() - 1:
				tree_item.set_metadata(0, path_element)
				tree_item.set_icon(0, _get_icon_for_fmod_path(fmod_path))
			stack.append(tree_item)
			continue
		
		if stack[i].get_text(0) != path_part:
			for j in range(stack.size() - 1, i - 1, -1):
				stack.remove_at(j)
			
			var parent_item = parent_root if stack.is_empty() else stack[stack.size() - 1]
			var tree_item = tree.create_item(parent_item)
			tree_item.set_text(0, path_part)
			if i == path_parts.size() - 1:
				tree_item.set_metadata(0, path_element)
				tree_item.set_icon(0, _get_icon_for_fmod_path(fmod_path))
			stack.append(tree_item)

func _on_item_selected():
	var metadata = tree.get_selected().get_metadata(0)
	if metadata == null:
		%GuidLabel.set_text("")
		%PathLabel.set_text("")
		copy_path_button.visible = false
		copy_guid_button.visible = false
		%SelectButton.visible = false
		%ParameterSectionSeparator.visible = false
		%ParametersLabel.visible = false
		%EventParametersDisplay.visible = false
		return
	%GuidLabel.set_text(metadata.get_guid())
	%PathLabel.set_text(metadata.get_path())
	if should_display_copy_buttons:
		copy_path_button.visible = true
		copy_guid_button.visible = true
	if should_display_select_button:
		%SelectButton.visible = true
	
	if metadata is FmodEventDescription:
		%EventParametersDisplay.set_fmod_event(metadata)
		%ParameterSectionSeparator.visible = true
		%ParametersLabel.visible = true
		%EventParametersDisplay.visible = true
		return
	%ParameterSectionSeparator.visible = false
	%ParametersLabel.visible = false
	%EventParametersDisplay.visible = false

func _on_copy_path_button():
	DisplayServer.clipboard_set(%PathLabel.text)

func _on_copy_guid_button():
	DisplayServer.clipboard_set(%GuidLabel.text)

func close_window():
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
