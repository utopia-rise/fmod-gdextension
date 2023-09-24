@tool
class_name FmodPlugin extends EditorPlugin

const ADDON_PATH = "res://addons/fmod"

@onready var iconFmodEventEmitter:Texture = preload(ADDON_PATH + "/icons/FmodEventEmitter2D.svg")
@onready var theme = get_editor_interface().get_base_control().get_theme()
var fmod_bank_explorer_window: PackedScene = load("res://addons/fmod/tool/ui/FmodBankExplorer.tscn")
var bank_explorer: FmodBankExplorer
var fmod_button: Button

var export_plugin = FmodEditorExportPluginProxy.new()

var emitter_inspector_plugin = FmodEmitterPropertyInspectorPlugin.new(self)
var bank_loader_inspector_plugin = FmodBankLoaderPropertyInspectorPlugin.new(self)

func _init():
	add_autoload_singleton("FmodManager", "res://addons/fmod/FmodManager.gd")
	fmod_button = Button.new()
	fmod_button.icon = load("res://addons/fmod/icons/fmod_icon.svg")
	fmod_button.text = "Fmod Explorer"
	
	fmod_button.pressed.connect(_on_project_explorer_button_clicked)
	
	add_control_to_container(EditorPlugin.CONTAINER_TOOLBAR, fmod_button)
	
	bank_explorer = fmod_bank_explorer_window.instantiate()
	bank_explorer.theme = get_editor_interface().get_base_control().get_theme()
	bank_explorer.visible = false
	add_child(bank_explorer)

	add_inspector_plugin(bank_loader_inspector_plugin)
	add_inspector_plugin(emitter_inspector_plugin)

func _on_project_explorer_button_clicked():
	bank_explorer.should_display_copy_buttons = true
	bank_explorer.should_display_select_button = false
	_popup_project_explorer(FmodBankExplorer.ToDisplayFlags.BANKS | FmodBankExplorer.ToDisplayFlags.BUSES | FmodBankExplorer.ToDisplayFlags.VCA | FmodBankExplorer.ToDisplayFlags.EVENTS)

func open_project_explorer_events(on_select_callable: Callable):
	_open_project_explorer(FmodBankExplorer.ToDisplayFlags.BANKS | FmodBankExplorer.ToDisplayFlags.EVENTS, on_select_callable)

func open_project_explorer_bank(on_select_callable: Callable):
	_open_project_explorer(FmodBankExplorer.ToDisplayFlags.BANKS, on_select_callable)

func _open_project_explorer(display_flag: int, on_select_callable: Callable):
	bank_explorer.should_display_copy_buttons = false
	bank_explorer.should_display_select_button = true
	_popup_project_explorer(display_flag, on_select_callable)

func _popup_project_explorer(to_display: int, callable: Callable = Callable()):
	if bank_explorer.visible == true:
		bank_explorer.close_window()
		return
	bank_explorer.regenerate_tree(to_display, callable)
	bank_explorer.popup_centered()

func _enter_tree():
	add_export_plugin(export_plugin)

func _exit_tree():
	remove_inspector_plugin(emitter_inspector_plugin)
	remove_inspector_plugin(bank_loader_inspector_plugin)
	
	remove_control_from_container(EditorPlugin.CONTAINER_TOOLBAR, fmod_button)
	fmod_button.queue_free()
	remove_export_plugin(export_plugin)
