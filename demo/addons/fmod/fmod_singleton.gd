tool
extends EditorPlugin

func _enter_tree():
	print("Fmod interface added to Godot.")
	add_autoload_singleton("Fmod", "res://addons/fmod/Fmod.gd")

func _exit_tree():
	print("Fmod interface removed from Godot.")
	remove_autoload_singleton("Fmod")