extends Node

# Called when the node enters the scene tree for the first time.
func _enter_tree():
	
	# set up FMOD
	FmodServer.set_software_format(0, FmodServer.FMOD_SPEAKERMODE_STEREO, 0)
	FmodServer.init(1024, FmodServer.FMOD_STUDIO_INIT_LIVEUPDATE, FmodServer.FMOD_INIT_NORMAL)
	FmodServer.set_sound_3D_settings(1.0, 32.0, 1.0)
	
	print("Fmod initialised.")
