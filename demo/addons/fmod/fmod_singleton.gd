tool

func _enter_tree():
	add_autoload_singleton("Fmod", "res:/addons/fmod/Fmod.gdns")

func _exit_tree():
		remove_autoload_singleton("Fmod")