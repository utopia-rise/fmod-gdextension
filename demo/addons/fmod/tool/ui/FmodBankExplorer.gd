@tool class_name FmodBankExplorer extends Window

static var _fmod_icon = load("res://addons/fmod/icons/fmod_icon.svg")
static var _vca_icon = load("res://addons/fmod/icons/vca_icon.svg")
static var _bank_icon = load("res://addons/fmod/icons/bank_icon.svg")
static var _event_icon = load("res://addons/fmod/icons/event_icon.svg")
static var _bus_icon = load("res://addons/fmod/icons/bus_icon.svg")
static var _snapshot_icon = load("res://addons/fmod/icons/snapshot_icon.svg")

var tree: Tree
var copy_path_button := Button.new()
var copy_guid_button := Button.new()

func _ready():
	var main_window_size = get_parent().get_window().size
	size = main_window_size * 0.5
	
	copy_path_button.text = "Copy"
	copy_guid_button.text = "Copy"
	copy_path_button.visible = false
	copy_guid_button.visible = false
	copy_path_button.pressed.connect(_on_copy_path_button)
	copy_guid_button.pressed.connect(_on_copy_guid_button)
	
	%ButtonsContainer.add_child(copy_path_button)
	%ButtonsContainer.add_child(copy_guid_button)
	
	tree = %Tree
	tree.item_selected.connect(_on_item_selected)
	
	tree.columns = 1
	
	var root_item := tree.create_item()
	root_item.set_text(0, "Fmod objects")
	root_item.set_icon(0, _fmod_icon)
	
	for bank in FmodServer.get_all_banks():
		var fmod_bank := bank as FmodBank
		
		var bank_item := tree.create_item(root_item)
		bank_item.set_text(0, fmod_bank.get_godot_res_path())
		bank_item.set_icon(0, _bank_icon)
		
		var buses_item := tree.create_item(bank_item)
		buses_item.set_text(0, "Buses")
		buses_item.set_icon(0, _bus_icon)
		
		var buses := fmod_bank.get_bus_list()
		buses.sort_custom(sort_by_path)
		_add_elements_as_tree(buses, buses_item)
		
		var vca_item := tree.create_item(bank_item)
		vca_item.set_text(0, "VCAs")
		vca_item.set_icon(0, _vca_icon)
		
		var vcas := fmod_bank.get_vca_list()
		vcas.sort_custom(sort_by_path)
		_add_elements_as_tree(vcas, vca_item)
		
		var events_item := tree.create_item(bank_item)
		events_item.set_text(0, "Events")
		events_item.set_icon(0, _event_icon)
		
		var events := fmod_bank.get_description_list()
		events.sort_custom(sort_by_path)
		_add_elements_as_tree(events, events_item)

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
		return
	%GuidLabel.set_text(metadata.get_guid())
	%PathLabel.set_text(metadata.get_path())
	copy_path_button.visible = true
	copy_guid_button.visible = true

func _on_copy_path_button():
	DisplayServer.clipboard_set(%PathLabel.text)

func _on_copy_guid_button():
	DisplayServer.clipboard_set(%GuidLabel.text)

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
