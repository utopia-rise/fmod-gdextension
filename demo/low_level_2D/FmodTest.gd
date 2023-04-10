extends Node

# Called when the node enters the scene tree for the first time.
func _enter_tree():
	# load banks
# warning-ignore:return_value_discarded
	FmodServer.load_bank("res://assets/Banks/Master.strings.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
# warning-ignore:return_value_discarded
	FmodServer.load_bank("res://assets/Banks/Master.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
# warning-ignore:return_value_discarded
	FmodServer.load_bank("res://assets/Banks/Music.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
# warning-ignore:return_value_discarded
	FmodServer.load_bank("res://assets/Banks/Vehicles.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
# warning-ignore:return_value_discarded
	FmodServer.load_bank("res://assets/Banks/SFX.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	print("Fmod initialised.")


func _on_reload_scene_button_pressed() -> void:
	get_tree().change_scene_to_file("res://low_level_2D/FmodScriptTest.tscn")


func _on_change_to_other_scene_pressed() -> void:
	get_tree().change_scene_to_file("res://low_level_2D/OtherScene.tscn")
