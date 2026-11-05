extends SceneTree

## Monkey test of the editor addon, run inside a headless Godot editor through run.sh. It builds a scene in a work folder,
## opens it like a user and drives the addon's own controls: the toolbar's Fmod Explorer (its tree, search, refresh,
## selection, event audition and parameter display), the Inspector's event picker of FmodEventEmitter2D/3D (through the
## explorer, typing a path or a GUID) with the fmod_parameters properties it creates, the bank list of FmodBankLoader
## (adding from the explorer or by hand, moving, removing) and its sample data preloading, the FMOD nodes' properties,
## and the undo history. A run passes when it prints `[MONKEY] DONE with 0 failure(s)` and no engine or script error.
##
## The runner calls every `step_*` method in declaration order, each a scripted step checking what its actions do. The
## `step_random_user_actions` step then calls `action_*` methods picked at random from the seed, each a single user
## action, and checks after each one that it reported no error, leaked no event instance and changed the scene only
## through the undo history. Adding a step or an action is declaring its method; its name, underscores as spaces, is
## what the log shows.
##
## The script is compiled before the extension's classes are registered: it names none of them, nor the addon's classes
## built on them, as types, and reaches them through ClassDB, Engine.get_singleton(), is_class() and script names.

const HERE := "res://editor_monkey"
const WORK := HERE + "/work"
const SCENE_PATH := WORK + "/monkey_scene.tscn"
const SEED := 20261102
const RANDOM_ACTIONS := 600
const FMOD_NODES := ["FmodEventEmitter2D", "FmodEventEmitter3D", "FmodListener2D", "FmodListener3D", "FmodBankLoader"]
const BANKS := ["res://assets/Banks/Master.strings.bank", "res://assets/Banks/Master.bank", "res://assets/Banks/Music.bank"]
const PARAMETER_PREFIX := "fmod_parameters/"
const NO_GUID := "{00000000-0000-0000-0000-000000000000}"

## Errors and warnings printed by the engine or the extension while the monkey runs.
class ErrorWatcher extends Logger:
	var mutex := Mutex.new()
	var errors: PackedStringArray = []
	var missing_banks := 0

	func _log_error(function: String, file: String, line: int, code: String, rationale: String, _editor_notify: bool, error_type: int, _script_backtraces: Array[ScriptBacktrace]) -> void:
		# A headless editor saving a scene reads its thumbnail from a renderer that draws nothing.
		if function == "texture_2d_get":
			return
		# Auditioning an event with a programmer instrument in the explorer: it has no audio table key to play.
		if (rationale + code).contains("Programmer sound callback"):
			return
		# FMOD destroys an instance's programmer sounds when it releases it, and reports that it waits for them.
		if (rationale + code).contains("programmer sounds pending release"):
			return
		# Setting a bank list with a path matching no file warns about it, on purpose.
		if (rationale + code).contains("cannot be opened. Check that it exists"):
			missing_banks += 1
			return
		mutex.lock()
		var kind := "warning" if error_type == ERROR_TYPE_WARNING else "error"
		errors.append("%s %s (%s:%d %s) %s" % [kind, rationale if rationale != "" else code, file.get_file(), line, function, code])
		mutex.unlock()

	func _log_message(_message: String, _error: bool) -> void:
		pass

	func take() -> PackedStringArray:
		mutex.lock()
		var taken := errors
		errors = []
		mutex.unlock()
		return taken


var failures := 0
var checks := 0
var rng := RandomNumberGenerator.new()
var watcher := ErrorWatcher.new()
var events: Array = []
var parameter_event: Object
var final := []
var last_edit_msec := 0
## Set by an action that changes the scene outside the undo history on purpose.
var untracked_change_allowed := false

#region The runner

func _initialize() -> void:
	OS.add_logger(watcher)
	rng.seed = SEED
	run.call_deferred()


func run() -> void:
	await pause(120)
	var steps := methods_named("step_")
	for index in steps.size():
		var description := describe(steps[index], "step_")
		log_line("===== %d: %s" % [index, description])
		await call(steps[index])
		check_log(description)
		await pause(5)
	check_log("at the end")
	log_line("DONE with %d failure(s), %d checks" % [failures, checks])
	quit(1 if failures > 0 else 0)


## The script's methods starting with the prefix, in declaration order.
func methods_named(prefix: String) -> Array:
	var names := []
	for method in get_script().get_script_method_list():
		if method["name"].begins_with(prefix):
			names.append(method["name"])
	return names


func describe(method: String, prefix: String) -> String:
	return method.trim_prefix(prefix).replace("_", " ")


func log_line(text: String) -> void:
	print("[MONKEY] ", text)


func expect(condition: bool, description: String) -> void:
	checks += 1
	if not condition:
		failures += 1
		log_line("FAIL " + description)


func check_log(context: String) -> void:
	for error in watcher.take():
		expect(false, "%s: %s" % [context, error])


## An invalid call must report an error, rather than fail silently or crash, and give nothing back.
func expect_rejected(description: String, call: Callable, rejected := func(result: Variant) -> bool: return result == null) -> void:
	check_log("before " + description)
	var result: Variant = call.call()
	await pause(2)
	expect(rejected.call(result), "%s gives nothing back (got %s)" % [description, result])
	expect(not watcher.take().is_empty(), "%s reports an error" % description)


func expect_same_scene(actual: Array, expected: Array, description: String) -> void:
	expect(actual == expected, description)
	for i in mini(actual.size(), expected.size()):
		if actual[i] != expected[i]:
			log_line("first difference: %s instead of %s" % [actual[i], expected[i]])
			return


func pause(frames := 3) -> void:
	for i in frames:
		await process_frame


func pick(values: Array) -> Variant:
	return values[rng.randi() % values.size()] if not values.is_empty() else null

#endregion

#region The editor

func server() -> Object:
	return Engine.get_singleton("FmodServer")


func scene() -> Node:
	return EditorInterface.get_edited_scene_root()


func history() -> UndoRedo:
	var undo_redo := EditorInterface.get_editor_undo_redo()
	return undo_redo.get_history_undo_redo(undo_redo.get_object_history_id(scene()))


## The editor's Undo and Redo menu items, which undo in the history of the edited scene as a user's shortcut does.
func edit_menu_item(text: String) -> void:
	for menu in find_all(root, func(node: Node) -> bool: return node is PopupMenu):
		for i in menu.item_count:
			if menu.get_item_text(i) == text:
				menu.id_pressed.emit(menu.get_item_id(i))
				return
	expect(false, "the editor has a %s menu item" % text)


func undo() -> void:
	edit_menu_item("Undo")


func redo() -> void:
	edit_menu_item("Redo")


func find_all(root_node: Node, test: Callable) -> Array:
	var found := []
	for child in root_node.get_children(true):
		if test.call(child):
			found.append(child)
		found.append_array(find_all(child, test))
	return found


func script_named(node: Node, script_name: String) -> bool:
	var script: Script = node.get_script()
	return script != null and script.get_global_name() == script_name


func fmod_runtimes() -> Array:
	return root.get_children().filter(func(node: Node) -> bool: return node.is_class("FmodRuntime"))


func plugins() -> Array:
	return find_all(root, func(node: Node) -> bool: return script_named(node, "FmodPlugin"))


func plugin() -> Node:
	return plugins()[0]


func explorer() -> Window:
	return plugin().bank_explorer


## A control of the addon's scenes by name: unique names belong to the sub-scene that declares them, which the
## Inspector properties instance inside themselves.
func unique(owner_node: Node, unique_name: String) -> Node:
	var found := find_all(owner_node, func(node: Node) -> bool: return node.name == unique_name)
	return found[0] if not found.is_empty() else null


func explorer_tree() -> Tree:
	return unique(explorer(), "Tree")


func play_controls() -> Control:
	return unique(explorer(), "EventPlayControls")


func inspector_property(script_name: String) -> Node:
	var found := find_all(EditorInterface.get_inspector(), func(node: Node) -> bool: return script_named(node, script_name))
	return found[0] if not found.is_empty() else null


func event_property() -> Node:
	return inspector_property("FmodEventEditorProperty")


func bank_property() -> Node:
	return inspector_property("FmodBankPathEditorProperty")


func bank_list_texts() -> Array:
	var bank_list: ItemList = unique(bank_property(), "BankList")
	var texts := []
	for i in bank_list.item_count:
		texts.append(bank_list.get_item_text(i))
	return texts


func press(button: BaseButton) -> void:
	if button.is_visible_in_tree() and not button.disabled:
		button.pressed.emit()


func type_text(line_edit: LineEdit, text: String) -> void:
	line_edit.text = text
	line_edit.text_changed.emit(text)


func fmod_nodes() -> Array:
	return find_all(scene(), func(node: Node) -> bool: return node.get_class() in FMOD_NODES)


func nodes_of(class_prefix: String) -> Array:
	return fmod_nodes().filter(func(node: Node) -> bool: return node.get_class().begins_with(class_prefix))


func emitter() -> Node:
	return nodes_of("FmodEventEmitter2D")[0]


func bank_loader() -> Node:
	return nodes_of("FmodBankLoader")[0]


## The Inspector merges an edit into the previous action of the same name made less than 800 ms before, whatever
## its object, keeping only the last edit: a user is never that fast, so edits of another node wait that long.
func select(node: Node) -> void:
	var since_last_edit := Time.get_ticks_msec() - last_edit_msec
	if EditorInterface.get_inspector().get_edited_object() != node and since_last_edit < 1000:
		await create_timer((1000 - since_last_edit) / 1000.0).timeout
	EditorInterface.get_selection().clear()
	EditorInterface.get_selection().add_node(node)
	EditorInterface.edit_node(node)
	await pause(6)


## Every tree item holding an FMOD object, depth first.
func explorer_items(item: TreeItem = null) -> Array:
	if item == null:
		item = explorer_tree().get_root()
		if item == null:
			return []
	var found := []
	if item.get_metadata(0) != null:
		found.append(item)
	for child in item.get_children():
		found.append_array(explorer_items(child))
	return found


func same(a: Object, b: Object) -> bool:
	return a != null and b != null and a.get_guid() == b.get_guid()


func explorer_item(test: Callable) -> TreeItem:
	for item in explorer_items():
		if test.call(item.get_metadata(0)):
			return item
	return null


func explorer_item_of_class(class_name_: String) -> TreeItem:
	return explorer_item(func(element: Object) -> bool: return element.is_class(class_name_))


func select_item(item: TreeItem) -> void:
	item.select(0)
	explorer_tree().item_selected.emit()


## Selects the item and presses the explorer's Select button, as a picker opened from the Inspector expects.
func choose(item: TreeItem, frames: int) -> void:
	select_item(item)
	press(unique(explorer(), "SelectButton"))
	await pause(frames)


func open_explorer() -> void:
	if not explorer().visible:
		press(plugin().fmod_button)
	await pause()


func close_explorer() -> void:
	if explorer().visible:
		press(unique(explorer(), "CloseButton"))
	await pause()


func search(word: String) -> void:
	var search_field: LineEdit = unique(explorer(), "SearchField")
	search_field.text = word
	search_field.text_submitted.emit(word)
	await pause()


func refresh_explorer(frames: int) -> void:
	press(unique(explorer(), "RefreshBanksButton"))
	await pause(frames)


## Opens the explorer from the emitter's event picker in the Inspector; false when the Inspector shows none.
func open_event_picker(node: Node) -> bool:
	await select(node)
	if event_property() == null:
		return false
	await close_explorer()
	press(unique(event_property(), "EventExplorerButton"))
	await pause()
	return true


## Opens the explorer from the bank loader's Add button in the Inspector; false when the Inspector shows none.
func open_bank_picker(node: Node) -> bool:
	await select(node)
	if bank_property() == null:
		return false
	await close_explorer()
	press(unique(bank_property(), "AddButton"))
	await pause()
	return true


func pick_event_with_explorer(node: Node, event: Object) -> void:
	if not await open_event_picker(node):
		return
	var item := explorer_item(func(element: Object) -> bool: return same(element, event))
	expect(item != null, "the explorer lists %s" % event.get_path())
	if item != null:
		await choose(item, 6)


func last_word(event: Object) -> String:
	return event.get_path().rsplit("/")[-1]


func parameter_properties(node: Node) -> Dictionary:
	var result := {}
	for property in node.get_property_list():
		var name: String = property["name"]
		if name.begins_with(PARAMETER_PREFIX):
			result[name] = node.get(name)
	return result


## The parameters' value properties, without their sub-properties.
func parameter_values(node: Node) -> Array:
	return parameter_properties(node).keys().filter(func(name: String) -> bool: return name.count("/") == 1)


## Reloading the banks invalidates the event descriptions held before.
func load_events() -> void:
	events = server().get_all_event_descriptions()
	parameter_event = null
	for event in events:
		for parameter in event.get_parameters():
			if not (parameter.is_global() or parameter.is_automatic() or parameter.is_read_only()):
				parameter_event = event


## What a node holds that the addon edits.
func node_state(node: Node) -> Dictionary:
	var state := {"class": node.get_class(), "name": String(node.name), "parent": String(scene().get_path_to(node.get_parent()))}
	match node.get_class():
		"FmodEventEmitter2D", "FmodEventEmitter3D":
			state["event"] = [node.event_name, node.event_guid, node.autoplay, node.attached, node.volume]
			state["parameters"] = parameter_properties(node)
		"FmodListener2D", "FmodListener3D":
			state["listener"] = [node.listener_index, node.is_locked, node.weight]
		"FmodBankLoader":
			state["banks"] = [node.bank_paths.duplicate(), node.preload_sample_data]
	return state


func snapshot() -> Array:
	return fmod_nodes().map(node_state)


## Event instances alive in FMOD: only the explorer's audition may be one.
func instance_total() -> int:
	var total := 0
	for event in server().get_all_event_descriptions():
		total += event.get_instance_count()
	return total

#endregion

#region Undoable edits, as the scene dock and the Inspector make them

func add_to_scene(node: Node, action: String) -> void:
	var undo_redo := EditorInterface.get_editor_undo_redo()
	undo_redo.create_action(action, UndoRedo.MERGE_DISABLE, scene())
	undo_redo.add_do_method(scene(), "add_child", node, true)
	undo_redo.add_do_method(node, "set_owner", scene())
	undo_redo.add_do_reference(node)
	undo_redo.add_undo_method(scene(), "remove_child", node)
	undo_redo.commit_action()
	await pause()


func add_node(type: String) -> void:
	var node: Node = ClassDB.instantiate(type)
	node.name = type
	await add_to_scene(node, "Add " + type)


func delete_node(node: Node) -> void:
	var undo_redo := EditorInterface.get_editor_undo_redo()
	undo_redo.create_action("Delete " + node.name, UndoRedo.MERGE_DISABLE, scene())
	undo_redo.add_do_method(scene(), "remove_child", node)
	undo_redo.add_undo_method(scene(), "add_child", node, true)
	undo_redo.add_undo_method(scene(), "move_child", node, node.get_index())
	undo_redo.add_undo_method(node, "set_owner", scene())
	undo_redo.add_undo_reference(node)
	undo_redo.commit_action()
	await pause()


func set_property(node: Node, property: String, value: Variant) -> void:
	var undo_redo := EditorInterface.get_editor_undo_redo()
	undo_redo.create_action("Set " + property, UndoRedo.MERGE_DISABLE, node)
	undo_redo.add_do_property(node, property, value)
	undo_redo.add_undo_property(node, property, node.get(property))
	undo_redo.commit_action()
	await pause(1)

#endregion

#region Scripted steps

func step_create_and_open_the_work_scene() -> void:
	DirAccess.make_dir_recursive_absolute(ProjectSettings.globalize_path(WORK))
	var new_scene := Node.new()
	new_scene.name = "MonkeyScene"
	var packed := PackedScene.new()
	packed.pack(new_scene)
	ResourceSaver.save(packed, SCENE_PATH)
	new_scene.free()
	EditorInterface.get_resource_filesystem().scan()
	await pause(60)
	root.size = Vector2i(1920, 1080)
	EditorInterface.open_scene_from_path(SCENE_PATH)
	await pause(60)


func step_the_fmod_plugin_is_loaded() -> void:
	expect(scene() != null and scene().name == "MonkeyScene", "the work scene is edited")
	expect(plugin() != null, "the FMOD plugin is enabled")
	expect(plugin().fmod_button.text == "Fmod Explorer", "the toolbar has the Fmod Explorer button")
	expect(fmod_runtimes().size() == 1, "FMOD runs one FmodRuntime node at the root of the editor's scene tree")
	expect(not ProjectSettings.has_setting("autoload/FmodManager"), "no FmodManager autoload is registered")
	load_events()
	expect(not events.is_empty(), "the editor loaded the master banks' events")
	log_line("%d banks, %d events, event with parameters: %s" % [server().get_all_banks().size(), events.size(), parameter_event.get_path() if parameter_event != null else "none"])


func step_browse_the_explorer() -> void:
	await open_explorer()
	expect(explorer().visible, "the toolbar button opens the explorer")
	var banks := explorer_items().filter(func(item: TreeItem) -> bool: return item.get_metadata(0).is_class("FmodBank"))
	expect(banks.size() == server().get_all_banks().size(), "the explorer lists the %d loaded banks" % server().get_all_banks().size())
	expect(explorer_item_of_class("FmodBus") != null, "the explorer lists buses")
	expect(explorer_item_of_class("FmodVCA") != null, "the explorer lists VCAs")
	var event_item := explorer_item_of_class("FmodEventDescription")
	select_item(event_item)
	await pause()
	expect(unique(explorer(), "PathLabel").text == event_item.get_metadata(0).get_path(), "selecting an event shows its path")
	expect(unique(explorer(), "GuidLabel").text == event_item.get_metadata(0).get_guid(), "selecting an event shows its GUID")
	expect(play_controls().visible, "selecting an event shows the audition controls")
	var bus_item := explorer_item_of_class("FmodBus")
	select_item(bus_item)
	await pause()
	expect(not play_controls().visible, "selecting a bus hides the audition controls")
	expect(unique(explorer(), "PathLabel").text == bus_item.get_metadata(0).get_path(), "selecting a bus shows its path")


func step_audition_events_in_the_explorer() -> void:
	select_item(explorer_item_of_class("FmodEventDescription"))
	await pause()
	press(play_controls().play_button)
	await pause(10)
	var instance: Object = play_controls().event_instance
	expect(instance != null and instance.is_valid(), "the play button creates an instance of the event")
	press(play_controls().stop_button)
	play_controls().fade_out_toggle.button_pressed = true
	press(play_controls().play_button)
	await pause(5)
	press(play_controls().stop_button)
	play_controls().fade_out_toggle.button_pressed = false
	if parameter_event != null:
		select_item(explorer_item(func(element: Object) -> bool: return same(element, parameter_event)))
		await pause()
		expect(unique(explorer(), "ParametersContainer").visible, "an event with parameters shows them")


func step_search_and_refresh_the_explorer() -> void:
	var word := last_word(events[0])
	await search(word)
	var items := explorer_items()
	expect(not items.is_empty() and items.all(func(item: TreeItem) -> bool: return item.get_metadata(0).get_path().containsn(word)), "searching \"%s\" lists only matching objects" % word)
	await search("")
	await refresh_explorer(10)
	expect(not explorer_items().is_empty(), "refreshing reloads the banks into the tree")
	await close_explorer()
	expect(not explorer().visible, "the close button closes the explorer")


func step_add_the_fmod_nodes_undo_and_redo() -> void:
	for type in FMOD_NODES:
		await add_node(type)
	expect(fmod_nodes().size() == FMOD_NODES.size(), "the five FMOD nodes are added")
	undo()
	await pause()
	expect(fmod_nodes().size() == FMOD_NODES.size() - 1, "undo removes the last added node")
	redo()
	await pause()
	expect(fmod_nodes().size() == FMOD_NODES.size(), "redo adds it back")


func step_pick_an_emitters_event_in_the_explorer_undo_and_redo() -> void:
	await select(emitter())
	expect(event_property() != null, "the Inspector shows the emitter's event picker")
	var event: Object = parameter_event if parameter_event != null else events[0]
	await pick_event_with_explorer(emitter(), event)
	expect(not explorer().visible, "selecting closes the explorer")
	expect(emitter().event_name == event.get_path() and emitter().event_guid == event.get_guid(), "the explorer sets the emitter's event path and GUID")
	if parameter_event != null:
		await select(emitter())
		expect(not parameter_properties(emitter()).is_empty(), "picking an event with parameters adds them to the emitter's properties")
	undo()
	undo()
	await pause()
	expect(emitter().event_name == "" and emitter().event_guid == NO_GUID, "undo clears the picked event (path %s, GUID %s)" % [emitter().event_name, emitter().event_guid])
	redo()
	redo()
	await pause()
	expect(emitter().event_guid == event.get_guid(), "redo picks it again")


func step_type_an_emitters_event_path_and_guid() -> void:
	await select(emitter())
	var other: Object = events[-1]
	type_text(unique(event_property(), "PathLineEdit"), other.get_path())
	await pause()
	expect(emitter().event_name == other.get_path(), "typing a path sets the emitter's event path")
	var guid_before: String = emitter().event_guid
	var guid_edit: LineEdit = unique(event_property(), "GuidLineEdit")
	type_text(guid_edit, "not a guid")
	await pause()
	expect(emitter().event_guid == guid_before, "an invalid GUID is rejected")
	type_text(guid_edit, other.get_guid())
	await pause()
	expect(emitter().event_guid == other.get_guid(), "typing a valid GUID sets the emitter's event GUID")


func step_edit_an_emitters_parameters() -> void:
	if parameter_event == null:
		log_line("no loaded event has editable parameters: skipped")
		return
	await pick_event_with_explorer(emitter(), parameter_event)
	await select(emitter())
	var names := parameter_values(emitter())
	expect(not names.is_empty(), "the emitter has a value property per parameter")
	for name in names:
		var value: Variant = emitter().get(name)
		if value is float or value is int:
			await set_property(emitter(), name, value + 1)
			expect(emitter().get(name) == value + 1, "setting %s changes its value" % name)
			undo()
			await pause(1)
			expect(emitter().get(name) == value, "undo restores %s" % name)
	var numeric: Array = names.filter(func(name: String) -> bool: return emitter().get(name) is float or emitter().get(name) is int)
	if numeric.is_empty():
		return
	var edited: String = numeric[0]
	var edited_value: Variant = emitter().get(edited) + 1
	await set_property(emitter(), edited, edited_value)
	await open_event_picker(emitter())
	await refresh_explorer(10)
	await choose(explorer_item(func(element: Object) -> bool: return same(element, parameter_event)), 6)
	expect(emitter().get(edited) == edited_value, "picking the same event after a bank refresh keeps %s (got %s)" % [edited, emitter().get(edited)])


func step_edit_a_bank_loaders_banks() -> void:
	await select(bank_loader())
	var property := bank_property()
	expect(property != null, "the Inspector shows the bank loader's bank list")
	var line_edit: LineEdit = unique(property, "ManualAddLineEdit")
	for bank in BANKS:
		line_edit.text = bank
		press(unique(property, "ManualAddButton"))
		await pause(2)
	expect(bank_loader().bank_paths == BANKS, "typed banks are added in order")
	line_edit.text = "res://not_a_bank.txt"
	press(unique(property, "ManualAddButton"))
	await pause(2)
	expect(bank_loader().bank_paths == BANKS, "a path that is not a bank is rejected")
	line_edit.text = BANKS[0]
	press(unique(property, "ManualAddButton"))
	await pause(2)
	expect(bank_loader().bank_paths == BANKS, "a bank already listed is not added twice")
	var missing_before: int = watcher.missing_banks
	line_edit.text = "res://assets/Banks/Missing.bank"
	press(unique(property, "ManualAddButton"))
	await pause(2)
	expect(watcher.missing_banks == missing_before + 1, "a bank path matching no file is warned about")
	unique(bank_property(), "BankList").select(BANKS.size())
	press(unique(bank_property(), "RemoveButton"))
	await pause(2)
	expect(bank_loader().bank_paths == BANKS, "the missing bank is removed again")
	unique(bank_property(), "BankList").select(2)
	press(unique(bank_property(), "UpButton"))
	await pause(2)
	expect(bank_loader().bank_paths == [BANKS[0], BANKS[2], BANKS[1]], "the up button moves a bank up")
	# The editor merges edits of one property made within a second of each other into one undo action.
	await create_timer(1.0).timeout
	unique(bank_property(), "BankList").select(1)
	press(unique(bank_property(), "RemoveButton"))
	await pause(2)
	expect(bank_loader().bank_paths == [BANKS[0], BANKS[1]], "the remove button removes a bank")
	undo()
	await pause(2)
	expect(bank_loader().bank_paths == [BANKS[0], BANKS[2], BANKS[1]], "undo puts the removed bank back (got %s)" % [bank_loader().bank_paths])
	await open_bank_picker(bank_loader())
	expect(explorer().visible, "the add button opens the explorer on the banks")
	var bank_item := explorer_item_of_class("FmodBank")
	await choose(bank_item, 4)
	expect(bank_loader().bank_paths.has(bank_item.get_metadata(0).get_godot_res_path()), "a bank picked in the explorer is listed")
	await set_property(bank_loader(), "preload_sample_data", true)
	expect(bank_loader().preload_sample_data, "the bank loader's preload_sample_data changes")


func step_edit_the_listeners() -> void:
	for listener in nodes_of("FmodListener"):
		await select(listener)
		await set_property(listener, "listener_index", 1)
		await set_property(listener, "is_locked", true)
		await set_property(listener, "weight", 0.5)
		expect(listener.listener_index == 1 and listener.is_locked and is_equal_approx(listener.weight, 0.5), "%s's properties change" % listener.name)


func project_settings_dialog() -> Window:
	return find_all(root, func(node: Node) -> bool: return node.get_class() == "ProjectSettingsEditor")[0]


func project_settings_inspector() -> Node:
	return find_all(project_settings_dialog(), func(node: Node) -> bool: return node.get_class() == "SectionedInspector")[0]


func shown_settings(section: String) -> Array:
	project_settings_inspector().update_category_list()
	var sections: Tree = find_all(project_settings_inspector(), func(node: Node) -> bool: return node is Tree)[0]
	var item := sections.get_root()
	while item != null and item.get_metadata(0) != section:
		item = item.get_next_in_tree()
	if item == null:
		return []
	item.select(0)
	await pause()
	return find_all(project_settings_inspector(), func(node: Node) -> bool: return node is EditorProperty).map(func(property: EditorProperty) -> String: return str(property.get_edited_property()))


func output_settings(shown: Array) -> Array:
	return shown.filter(func(name: String) -> bool: return name.begins_with("output/"))


func step_switch_the_output_routing_in_project_settings() -> void:
	const ROUTING := "Fmod/Mixer/output/routing"
	project_settings_dialog().popup_centered()
	var advanced: CheckButton = find_all(project_settings_dialog(), func(node: Node) -> bool: return node is CheckButton and node.text.contains("Advanced"))[0]
	var was_advanced := advanced.button_pressed
	advanced.button_pressed = true
	var shown := await shown_settings("Fmod/Mixer")
	expect(output_settings(shown) == ["output/routing"], "with Direct to Device, Fmod/Mixer's Output group shows only Routing: %s" % [shown])
	expect(shown.has("format/raw_speaker_count"), "Advanced Settings show the advanced settings: %s" % [shown])
	ProjectSettings.set_setting(ROUTING, 1)
	await pause(5)
	shown = await shown_settings("Fmod/Mixer")
	expect(output_settings(shown).has("output/godot_audio_bus") and output_settings(shown).has("output/godot_bus_mixing"), "with Godot Bus, the Output group shows the bus settings: %s" % [shown])
	ProjectSettings.set_setting(ROUTING, 0)
	await pause(5)
	shown = await shown_settings("Fmod/Mixer")
	expect(output_settings(shown) == ["output/routing"], "back to Direct to Device, the bus settings are hidden again: %s" % [shown])
	shown = await shown_settings("Fmod/Spatialization")
	expect(shown == ["listener_count", "distance_factor", "rolloff_scale", "doppler_scale"], "Fmod/Spatialization shows its settings in order: %s" % [shown])
	advanced.button_pressed = was_advanced
	project_settings_dialog().hide()


func step_disable_and_re_enable_the_plugin() -> void:
	var project := FileAccess.get_file_as_string("res://project.godot")
	await select(emitter())
	await open_explorer()
	if play_controls().visible:
		press(play_controls().play_button)
	EditorInterface.set_plugin_enabled("res://addons/fmod/plugin.cfg", false)
	await pause(10)
	expect(plugins().is_empty(), "disabling the plugin removes it")
	expect(fmod_runtimes().size() == 1, "FMOD keeps running with the plugin disabled")
	EditorInterface.set_plugin_enabled("res://addons/fmod/plugin.cfg", true)
	await pause(10)
	expect(plugins().size() == 1, "enabling the plugin adds it back once")
	load_events()
	expect(fmod_runtimes().size() == 1, "re-enabling the plugin adds no second FmodRuntime")
	await open_explorer()
	expect(not explorer_items().is_empty(), "the explorer works again")
	await close_explorer()
	await select(emitter())
	expect(event_property() != null, "the Inspector shows the event picker again")
	if FileAccess.get_file_as_string("res://project.godot") != project:
		var file := FileAccess.open("res://project.godot", FileAccess.WRITE)
		file.store_string(project)
		file.close()


func step_call_fmodserver_with_invalid_input() -> void:
	await expect_rejected("creating an instance of an unknown event", func() -> Variant: return server().create_event_instance("event:/not/an/event"))
	await expect_rejected("creating an instance of an unknown GUID", func() -> Variant: return server().create_event_instance_with_guid("{12345678-1234-1234-1234-123456789abc}"))
	var valid_guid: String = events[0].get_guid()
	for malformed: String in [valid_guid + "xyz", valid_guid.replace("{", "{ "), valid_guid.replace("{", "{0x"), valid_guid.trim_prefix("{"), valid_guid.trim_suffix("}")]:
		expect(not server().check_event_guid(malformed), "the malformed GUID \"%s\" is not read as the event's" % malformed)
	expect(server().check_event_guid(valid_guid.to_upper()), "an upper case GUID is read as the event's")
	await expect_rejected("creating an instance from no description", func() -> Variant: return server().create_event_instance_from_description(null))
	await expect_rejected("playing a one-shot of an unknown event", func() -> Variant:
		server().play_one_shot("event:/not/an/event")
		return null)
	await expect_rejected("loading a missing file as a sound", func() -> Variant: return server().load_file_as_sound("res://not_a_sound.ogg"))
	await expect_rejected("loading a missing file as music", func() -> Variant: return server().load_file_as_music("res://not_a_sound.ogg"))
	await expect_rejected("creating an instance of a file never loaded", func() -> Variant: return server().create_sound_instance("res://not_a_sound.ogg"))
	await expect_rejected("loading a missing bank", func() -> Variant: return server().load_bank("res://not_a_bank.bank", 0))
	var event: Object = parameter_event if parameter_event != null else events[0]
	await expect_rejected("labels of an unknown parameter name", func() -> Variant: return event.get_parameter_labels_by_name("not a parameter"), func(result: Variant) -> bool: return result.is_empty())
	await expect_rejected("labels of an unknown parameter index", func() -> Variant: return event.get_parameter_labels_by_index(999), func(result: Variant) -> bool: return result.is_empty())
	await expect_rejected("a label of an unknown parameter", func() -> Variant: return event.get_parameter_label_by_name("not a parameter", 0), func(result: Variant) -> bool: return result == "")
	var handle: int = server().load_plugin("res://not_a_plugin.dll", 0)
	await pause(2)
	expect(not watcher.take().is_empty(), "loading a missing plugin reports an error")
	expect(not server().is_plugin_loaded(handle), "a missing plugin is not reported as loaded")


func step_round_trips_through_fmodserver_buses_vcas_and_sounds() -> void:
	var sound_path := "res://assets/Sounds/doorOpen_1.ogg"
	server().load_file_as_sound(sound_path)
	var sound: Object = server().create_sound_instance(sound_path)
	expect(sound != null and sound.is_valid(), "a loaded file gives a sound instance")
	if sound != null:
		sound.play()
		sound.set_volume(0.5)
		sound.set_pitch(1.5)
		expect(is_equal_approx(sound.get_volume(), 0.5) and is_equal_approx(sound.get_pitch(), 1.5), "a sound keeps its volume and pitch")
		sound.stop()
		server().unload_file(sound_path)
		await pause(5)
	for description in server().get_global_parameter_desc_list():
		var name: String = description["name"]
		server().set_global_parameter_by_name(name, description["maximum"])
		await pause(2)
		expect(is_equal_approx(server().get_global_parameter_by_name(name), description["maximum"]), "the global parameter %s keeps its value" % name)
		server().set_global_parameter_by_name(name, description["minimum"])
	var bus: Object = server().get_bus("bus:/")
	bus.volume = 0.5
	bus.mute = true
	expect(is_equal_approx(bus.volume, 0.5) and bus.mute, "the master bus keeps its volume and mute")
	bus.volume = 1.0
	bus.mute = false
	var vcas: Array = server().get_all_vca()
	if not vcas.is_empty():
		vcas[0].volume = 0.25
		expect(is_equal_approx(vcas[0].volume, 0.25), "a VCA keeps its volume")
		vcas[0].volume = 1.0
	if server().check_event_path("event:/Character/Dialogue"):
		var event: Object = server().create_event_instance("event:/Character/Dialogue")
		event.set_programmer_callback("welcome")
		event.start()
		await pause(30)
		event.stop(1)
		event.release()
		await pause(5)
		check_log("a programmer instrument plays a known audio table key")
	# A refresh unloads the banks and loads them again: wrappers held before it are invalid.
	await open_explorer()
	await refresh_explorer(10)
	await close_explorer()
	load_events()
	expect(not bus.is_valid(), "a bus held across a refresh is invalid")
	var reloaded_bus: Object = server().get_bus("bus:/")
	reloaded_bus.volume = 0.5
	expect(is_equal_approx(reloaded_bus.volume, 0.5), "the master bus found again after a refresh keeps its volume")
	reloaded_bus.volume = 1.0


func step_random_user_actions() -> void:
	var actions := methods_named("action_")
	for index in RANDOM_ACTIONS:
		var action: String = pick(actions)
		var context := "random action %d (%s)" % [index, describe(action, "action_")]
		log_line(context)
		var state_before := snapshot()
		var version_before := history().get_version()
		var may_merge := Time.get_ticks_msec() - last_edit_msec < 1000
		untracked_change_allowed = false
		await call(action)
		await pause(1)
		if history().get_version() != version_before:
			last_edit_msec = Time.get_ticks_msec()
		elif not may_merge and not untracked_change_allowed:
			expect(snapshot() == state_before, "%s changes the scene only through an undoable action" % context)
		# FMOD destroys a released instance once its sample data finishes loading: a leak is one still there later.
		if instance_total() > 1:
			await pause(10)
		expect(instance_total() <= 1, "%s: %d event instances alive" % [context, instance_total()])
		check_log(context)
	await close_explorer()
	log_line("random phase: %d FMOD nodes, %d undo steps" % [fmod_nodes().size(), history().get_history_count()])


# Reopening a scene clears its history: the history is checked first.
func step_undo_everything_redo_everything() -> void:
	final = snapshot()
	var undone := 0
	while history().has_undo() and undone < 5000:
		undo()
		undone += 1
	await pause(10)
	expect(fmod_nodes().is_empty(), "undoing everything empties the scene (%d actions)" % undone)
	for i in undone:
		redo()
	await pause(10)
	expect_same_scene(snapshot(), final, "redoing everything gives back the final scene")


func step_save_and_reopen_the_monkeyed_scene() -> void:
	EditorInterface.get_selection().clear()
	EditorInterface.save_scene()
	final = snapshot()
	await pause(10)
	EditorInterface.close_scene()
	await pause(10)
	EditorInterface.open_scene_from_path(SCENE_PATH)
	await pause(60)
	expect_same_scene(snapshot(), final, "the saved scene reopens unchanged")


func step_remove_the_work_folder() -> void:
	EditorInterface.close_scene()
	await pause(5)
	for file in DirAccess.get_files_at(WORK):
		DirAccess.remove_absolute(ProjectSettings.globalize_path(WORK + "/" + file))
	DirAccess.remove_absolute(ProjectSettings.globalize_path(WORK))
	EditorInterface.get_resource_filesystem().scan()

#endregion

#region Random actions

func action_open_or_close_the_explorer() -> void:
	if explorer().visible:
		await close_explorer()
	else:
		await open_explorer()


func action_select_an_explorer_item() -> void:
	if explorer().visible:
		var item: TreeItem = pick(explorer_items())
		if item != null:
			select_item(item)


func action_play_or_stop_the_audition() -> void:
	if explorer().visible and play_controls().visible:
		press(play_controls().play_button if rng.randf() < 0.6 else play_controls().stop_button)


func action_search_the_explorer() -> void:
	if explorer().visible:
		await search("" if rng.randf() < 0.4 else last_word(pick(events)).left(rng.randi_range(1, 4)))


func action_refresh_the_explorer() -> void:
	if explorer().visible:
		await refresh_explorer(1)


func action_add_a_node() -> void:
	if fmod_nodes().size() < 12:
		await add_node(pick(FMOD_NODES))


func action_delete_a_node() -> void:
	var node: Node = pick(fmod_nodes())
	if node != null and rng.randf() < 0.5:
		await delete_node(node)


func action_select_a_node() -> void:
	var node: Node = pick(fmod_nodes())
	if node != null:
		await select(node)


func action_pick_an_event_in_the_explorer() -> void:
	var node: Node = pick(nodes_of("FmodEventEmitter"))
	if node != null:
		await pick_event_with_explorer(node, pick(events))


func action_set_an_emitter_property() -> void:
	var node: Node = pick(nodes_of("FmodEventEmitter"))
	if node != null:
		var name: String = pick(["autoplay", "attached", "allow_fadeout", "preload_event", "volume"])
		await set_property(node, name, rng.randf_range(0, 2) if name == "volume" else not node.get(name))


func action_set_a_listener_property() -> void:
	var node: Node = pick(nodes_of("FmodListener"))
	if node != null:
		var name: String = pick(["listener_index", "is_locked", "weight"])
		var value: Variant = rng.randi_range(0, 7) if name == "listener_index" else (not node.is_locked if name == "is_locked" else rng.randf())
		await set_property(node, name, value)


func action_set_a_parameter() -> void:
	var node: Node = pick(nodes_of("FmodEventEmitter"))
	if node != null:
		var name: Variant = pick(parameter_values(node).filter(func(value_name: String) -> bool: return node.get(value_name) is float or node.get(value_name) is int))
		if name != null:
			await set_property(node, name, node.get(name) + rng.randi_range(-2, 2))


func action_revert_a_parameter() -> void:
	var node: Node = pick(nodes_of("FmodEventEmitter"))
	if node != null:
		var name: Variant = pick(parameter_values(node))
		if name != null:
			var before: Variant = node.get(name)
			await set_property(node, name, node.property_get_revert(name))
			expect(typeof(node.get(name)) == typeof(before), "reverting %s keeps its type (%s, was %s)" % [name, type_string(typeof(node.get(name))), type_string(typeof(before))])


func action_undo() -> void:
	undo()


func action_redo() -> void:
	redo()


func action_save_the_scene() -> void:
	untracked_change_allowed = true
	EditorInterface.save_scene()


func action_type_an_event_path_key_by_key() -> void:
	var node: Node = pick(nodes_of("FmodEventEmitter"))
	if node == null:
		return
	await select(node)
	if event_property() != null:
		var path: String = pick(events).get_path()
		var path_edit: LineEdit = unique(event_property(), "PathLineEdit")
		for length in range(1, path.length() + 1):
			type_text(path_edit, path.left(length))
		await pause()
		expect(node.event_name == path, "typing %s sets the emitter's path (got %s)" % [path, node.event_name])


func action_type_a_malformed_guid() -> void:
	var node: Node = pick(nodes_of("FmodEventEmitter"))
	if node == null:
		return
	await select(node)
	if event_property() != null:
		var guid_before: String = node.event_guid
		var valid_guid: String = pick(events).get_guid()
		var typed: String = pick(["{zzzzzzzz-zzzz-zzzz-zzzz-zzzzzzzzzzzz}", "x" + valid_guid, valid_guid.left(20)])
		type_text(unique(event_property(), "GuidLineEdit"), typed)
		await pause()
		expect(node.event_guid == guid_before or server().check_event_guid(node.event_guid), "typing the GUID %s leaves the emitter's GUID unchanged or valid (got %s)" % [typed, node.event_guid])


func action_select_a_bank_in_the_event_picker() -> void:
	var node: Node = pick(nodes_of("FmodEventEmitter"))
	if node == null:
		return
	var guid_before: String = node.event_guid
	if not await open_event_picker(node):
		return
	var bank_item := explorer_item_of_class("FmodBank")
	if bank_item != null:
		await choose(bank_item, 3)
	await close_explorer()
	expect(node.event_guid == guid_before or server().check_event_guid(node.event_guid), "a bank selected in the event picker is not set as the event")


func action_refresh_or_search_the_event_picker_then_pick() -> void:
	var node: Node = pick(nodes_of("FmodEventEmitter"))
	if node == null:
		return
	var event: Object = pick(events)
	if not await open_event_picker(node):
		return
	if rng.randf() < 0.5:
		await refresh_explorer(5)
	else:
		await search(last_word(event))
	var item := explorer_item(func(element: Object) -> bool: return same(element, event))
	if item != null:
		await choose(item, 4)
		expect(node.event_guid == event.get_guid(), "an event picked after a refresh or a search is set")
	await close_explorer()


func action_edit_a_bank_list() -> void:
	var node: Node = pick(nodes_of("FmodBankLoader"))
	if node == null:
		return
	await select(node)
	var property := bank_property()
	if property == null:
		return
	match rng.randi_range(0, 3):
		0:
			unique(property, "ManualAddLineEdit").text = pick(BANKS)
			press(unique(property, "ManualAddButton"))
		1, 2:
			var bank_list: ItemList = unique(property, "BankList")
			if bank_list.item_count > 0:
				bank_list.select(rng.randi() % bank_list.item_count)
				press(unique(property, pick(["UpButton", "DownButton", "RemoveButton"])))
		3:
			press(unique(property, "AddButton"))


func action_edit_a_bank_list_set_from_a_script() -> void:
	var node: Node = pick(nodes_of("FmodBankLoader"))
	if node == null:
		return
	await set_property(node, "bank_paths", [BANKS[0], BANKS[0], "", "res://assets/Banks/missing.bank"])
	await select(node)
	# The Inspector shows a value set from a script once its auto refresh interval passes.
	await create_timer(float(EditorInterface.get_editor_settings().get_setting("docks/property_editor/auto_refresh_interval")) + 0.1).timeout
	var property := bank_property()
	if property != null:
		var bank_list: ItemList = unique(property, "BankList")
		bank_list.select(rng.randi() % bank_list.item_count)
		press(unique(property, pick(["UpButton", "DownButton", "RemoveButton"])))
		await pause(2)
		expect(bank_list_texts() == Array(node.bank_paths), "the bank list shows %s (shows %s)" % [node.bank_paths, bank_list_texts()])


func action_search_the_bank_picker() -> void:
	var node: Node = pick(nodes_of("FmodBankLoader"))
	if node != null and await open_bank_picker(node):
		await search("Master")
		expect(not explorer_items().is_empty(), "searching the bank picker lists the matching banks")
		await close_explorer()


func action_select_a_bus_during_an_audition() -> void:
	if not (explorer().visible and play_controls().visible):
		return
	press(play_controls().play_button)
	await pause(3)
	var bus_item := explorer_item_of_class("FmodBus")
	if bus_item != null:
		select_item(bus_item)
		await pause(3)
		var audition: Object = play_controls().event_instance
		expect(audition == null or audition.get_playback_state() != 0, "selecting a bus stops the audition")


func action_duplicate_an_emitter() -> void:
	var node: Node = pick(nodes_of("FmodEventEmitter"))
	if node == null:
		return
	var copy: Node = node.duplicate()
	await add_to_scene(copy, "Duplicate " + node.name)
	var original_state := node_state(node)
	var copy_state := node_state(copy)
	original_state.erase("name")
	copy_state.erase("name")
	expect(copy_state == original_state, "a duplicated emitter keeps its event and parameters")

#endregion
