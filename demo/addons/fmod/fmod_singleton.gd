tool
extends EditorPlugin

var main_pan_inst

func _enter_tree():
	add_autoload_singleton("Fmod", "res://addons/fmod/Fmod.tscn")
	main_pan_inst = preload("res://addons/fmod/fmod_config_window.tscn").instance()
	get_editor_interface().get_editor_viewport().add_child(main_pan_inst)
	make_visible(false)

func _exit_tree():
	remove_autoload_singleton("Fmod")

func has_main_screen():
	return true

func make_visible(visible: bool):
	if main_pan_inst:
		main_pan_inst.visible = visible

func get_plugin_name():
	return "FMOD"

func get_plugin_icon():
	return load("res://addons/fmod/nodes/fmod.svg")
	#get_editor_interface().get_base_control().get_icon("Node", "EditorIcons")
