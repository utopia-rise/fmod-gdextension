extends "res://addons/gut/test.gd"

func before_all():
	# set up FMOD
	FmodServer.set_software_format(0, FmodServer.FMOD_SPEAKERMODE_STEREO, 0)
	FmodServer.set_system_dsp_buffer_size(512, 4)
	FmodServer.init(1024, FmodServer.FMOD_STUDIO_INIT_LIVEUPDATE, FmodServer.FMOD_INIT_NORMAL)
	FmodServer.set_sound_3D_settings(1, 32, 1)
