tool
extends EditorPlugin

func _enter_tree():
	add_autoload_singleton("Fmod", "res://addons/fmod/Fmod.gd")

func _exit_tree():
	remove_autoload_singleton("Fmod")