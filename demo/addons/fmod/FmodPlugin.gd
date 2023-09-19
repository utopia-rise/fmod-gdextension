@tool
extends EditorPlugin

const ADDON_PATH = "res://addons/fmod"

@onready var iconFmodEventEmitter:Texture = preload(ADDON_PATH + "/icons/FmodEventEmitter2D.svg")
@onready var theme = get_editor_interface().get_base_control().get_theme()
var fmod_bank_explorer_window: PackedScene = load("res://addons/fmod/tool/ui/FmodBankExplorer.tscn")
var bank_explorer: FmodBankExplorer
var fmod_button: Button

var export_plugin = FmodEditorExportPluginProxy.new()

#func _ready() -> void:
	# configure icons
	#theme.set_icon("FmodEventEmitter2D", "EditorIcons", iconFmodEventEmitter)

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

func _on_project_explorer_button_clicked():
	if bank_explorer.visible == true:
		bank_explorer.visible = false
		return
	bank_explorer.popup_centered()

func _enter_tree():
	add_export_plugin(export_plugin)

func _exit_tree():
	remove_control_from_container(EditorPlugin.CONTAINER_TOOLBAR, fmod_button)
	fmod_button.queue_free()
	remove_export_plugin(export_plugin)
