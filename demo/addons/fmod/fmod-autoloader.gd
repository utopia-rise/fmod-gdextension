extends Node

func _enter_tree():
	get_tree().root.call_deferred("add_child", Fmod.get_singleton())
