extends Node2D

func _ready():
	# set up FMOD 
	FMOD.system_set_software_format(0, Fmod.FMOD_SPEAKERMODE_STEREO, 0)
	FMOD.system_init(1024, Fmod.FMOD_STUDIO_INIT_LIVEUPDATE, Fmod.FMOD_INIT_NORMAL)
	
	# load banks
	FMOD.bank_load("./Banks/Desktop/Master Bank.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	FMOD.bank_load("./Banks/Desktop/Master Bank.strings.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	
	# register listener
	FMOD.system_add_listener($Listener)
	
	# play some events
	FMOD.play_one_shot("event:/Car engine", $SoundSource1)
	FMOD.play_one_shot("event:/Waterfall", $SoundSource2)
		