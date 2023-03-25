@tool
extends EditorPlugin

const ADDON_PATH = "res://addons/fmod"

@onready var iconFmodEventEmitter:Texture = preload(ADDON_PATH + "/icons/FmodEventEmitter2D.svg")
@onready var theme = get_editor_interface().get_base_control().get_theme()

#func _ready() -> void:
	# configure icons
	#theme.set_icon("FmodEventEmitter2D", "EditorIcons", iconFmodEventEmitter)

func _init():
	add_autoload_singleton("FmodManager", "res://addons/fmod/FmodManager.gd")
