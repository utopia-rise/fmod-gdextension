@tool extends Window

var tree: Tree

static var _fmod_icon = load("res://addons/fmod/icons/fmod_icon.svg")
static var _vca_icon = load("res://addons/fmod/icons/vca_icon.svg")
static var _bank_icon = load("res://addons/fmod/icons/bank_icon.svg")
static var _event_icon = load("res://addons/fmod/icons/event_icon.svg")
static var _bus_icon = load("res://addons/fmod/icons/bus_icon.svg")
static var _snapshot_icon = load("res://addons/fmod/icons/snapshot_icon.svg")

func _ready():
	var main_window_size = get_parent().get_window().size
	size = main_window_size * 0.5
	
	tree = $VBoxContainer/ScrollContainer/Tree
	
	tree.columns = 3
	
	var root_item: TreeItem = tree.create_item()
	root_item.set_text(0, "Fmod objects")
	root_item.set_icon(0, _fmod_icon)
	
	for bank in FmodServer.get_all_banks():
		var fmod_bank: FmodBank = bank as FmodBank
		
		var bank_item: TreeItem = tree.create_item(root_item)
		bank_item.set_text(0, fmod_bank.get_godot_res_path())
		bank_item.set_icon(0, _bank_icon)
		
		var buses_item: TreeItem = tree.create_item(bank_item)
		buses_item.set_text(0, "Buses")
		buses_item.set_icon(0, _bus_icon)
		
		var buses: Array = fmod_bank.get_bus_list()
		buses.sort_custom(sort_by_path)
		_add_elements_as_tree(buses, buses_item)
		
		var vca_item: TreeItem = tree.create_item(bank_item)
		vca_item.set_text(0, "VCAs")
		vca_item.set_icon(0, _vca_icon)
		
		var vcas: Array = fmod_bank.get_vca_list()
		vcas.sort_custom(sort_by_path)
		_add_elements_as_tree(vcas, vca_item)
		
		var events_item: TreeItem = tree.create_item(bank_item)
		events_item.set_text(0, "Events")
		events_item.set_icon(0, _event_icon)
		
		var events: Array = fmod_bank.get_description_list()
		events.sort_custom(sort_by_path)
		_add_elements_as_tree(events, events_item)

func _add_elements_as_tree(elements: Array, parent: TreeItem):
	var stack = Array()
	for element in elements:
		_add_element_to_stack(stack, parent, element)

func _add_element_to_stack(stack: Array, parent_root: TreeItem, path_element):
	var fmod_path: String = path_element.get_path()
	var path_parts: PackedStringArray = fmod_path.split("/")
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
				tree_item.set_text(1, fmod_path)
				tree_item.set_text(2, path_element.get_guid())
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
				tree_item.set_text(1, fmod_path)
				tree_item.set_text(2, path_element.get_guid())
				tree_item.set_icon(0, _get_icon_for_fmod_path(fmod_path))
			stack.append(tree_item)

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
