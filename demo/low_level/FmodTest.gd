extends Node

# Called when the node enters the scene tree for the first time.
func _enter_tree():
	
	# set up FMOD
	FmodServer.set_software_format(0, FmodServer.FMOD_SPEAKERMODE_STEREO, 0)
	FmodServer.init(1024, FmodServer.FMOD_STUDIO_INIT_LIVEUPDATE, FmodServer.FMOD_INIT_NORMAL)
	FmodServer.set_sound_3D_settings(1.0, 32.0, 1.0)
	FmodServer.set_listener_number(2)
	
	# load banks
# warning-ignore:return_value_discarded
	FmodServer.load_bank("res://assets/Banks/Master.strings.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
# warning-ignore:return_value_discarded
	FmodServer.load_bank("res://assets/Banks/Master.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
# warning-ignore:return_value_discarded
	FmodServer.load_bank("res://assets/Banks/Music.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
# warning-ignore:return_value_discarded
	FmodServer.load_bank("res://assets/Banks/Vehicles.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	print("Fmod initialised.")
