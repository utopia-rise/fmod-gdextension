@tool
class_name FmodPlugin extends EditorPlugin


const ADDON_PATH: StringName = &"res://addons/fmod"
const FmodManager_Autoload_Name: StringName = &"FmodManager"
@onready var theme: Theme = get_editor_interface().get_base_control().get_theme()

var fmod_bank_explorer_window: PackedScene = load("res://addons/fmod/tool/ui/FmodBankExplorer.tscn")
var bank_explorer: FmodBankExplorer
var fmod_button: Button
var export_plugin: FmodEditorExportPlugin = FmodEditorExportPlugin.new()
var android_export_plugin: FmodAndroidExportPlugin = FmodAndroidExportPlugin.new()
var emitter_inspector_plugin: FmodEmitterPropertyInspectorPlugin = FmodEmitterPropertyInspectorPlugin.new(self)
var bank_loader_inspector_plugin: FmodBankLoaderPropertyInspectorPlugin = FmodBankLoaderPropertyInspectorPlugin.new(self)

func _init() -> void:
	FmodBankDatabase.reload_all_banks()

func _enable_plugin() -> void:
	add_autoload_singleton(FmodManager_Autoload_Name, "res://addons/fmod/FmodManager.gd")

func _disable_plugin() -> void:
	remove_autoload_singleton(FmodManager_Autoload_Name)

func _enter_tree() -> void:
	_add_explorer_button()
	
	_add_bank_explorer_window()

	add_inspector_plugin(bank_loader_inspector_plugin)
	add_inspector_plugin(emitter_inspector_plugin)
	
	add_export_plugin(export_plugin)
	add_export_plugin(android_export_plugin)

func _exit_tree() -> void:
	remove_control_from_container(EditorPlugin.CONTAINER_TOOLBAR, fmod_button)
	fmod_button.queue_free()
	
	bank_explorer.queue_free()
	
	remove_inspector_plugin(emitter_inspector_plugin)
	remove_inspector_plugin(bank_loader_inspector_plugin)

	remove_export_plugin(android_export_plugin)
	remove_export_plugin(export_plugin)

func _add_explorer_button() -> void:
	fmod_button = Button.new()
	fmod_button.icon = load("res://addons/fmod/icons/fmod_icon.svg")
	fmod_button.text = "Fmod Explorer"
	
	fmod_button.pressed.connect(_on_project_explorer_button_clicked)
	
	add_control_to_container(EditorPlugin.CONTAINER_TOOLBAR, fmod_button)

func _add_bank_explorer_window() -> void:
	bank_explorer = fmod_bank_explorer_window.instantiate()
	bank_explorer.theme = get_editor_interface().get_base_control().get_theme()
	bank_explorer.visible = false
	
	add_child(bank_explorer)

func _on_project_explorer_button_clicked() -> void:
	bank_explorer.should_display_copy_buttons = true
	bank_explorer.should_display_select_button = false
	_popup_project_explorer(FmodBankExplorer.ToDisplayFlags.BUSES | FmodBankExplorer.ToDisplayFlags.VCA | FmodBankExplorer.ToDisplayFlags.EVENTS)

func open_project_explorer_events(on_select_callable: Callable) -> void:
	_open_project_explorer(FmodBankExplorer.ToDisplayFlags.EVENTS, on_select_callable)

func open_project_explorer_bank(on_select_callable: Callable) -> void:
	_open_project_explorer(0, on_select_callable)

func _open_project_explorer(display_flag: int, on_select_callable: Callable) -> void:
	bank_explorer.should_display_copy_buttons = false
	bank_explorer.should_display_select_button = true
	_popup_project_explorer(display_flag, on_select_callable)

func _popup_project_explorer(to_display: int, callable: Callable = Callable()) -> void:
	if bank_explorer.visible == true:
		bank_explorer.close_window()
		return
	
	bank_explorer.flags = to_display
	bank_explorer.reset_search()
	bank_explorer.regenerate_tree(callable)
	bank_explorer.popup_centered()
	
