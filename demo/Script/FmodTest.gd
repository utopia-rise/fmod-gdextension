extends Node

## Called when the node enters the scene tree for the first time.
#func _enter_tree():
#	# set up FMOD
#	Fmod.set_software_format(0, Fmod.FMOD_SPEAKERMODE_STEREO, 0)
#	Fmod.init(1024, Fmod.FMOD_STUDIO_INIT_LIVEUPDATE, Fmod.FMOD_INIT_NORMAL)
#	Fmod.set_sound_3D_settings(1, 32, 1)
#	Fmod.set_listener_number(2)
#	#Fmod.set_default_callback(Fmod.FMOD_STUDIO_EVENT_CALLBACK_ALL)
#
#	# load banks
## warning-ignore:return_value_discarded
#	Fmod.load_bank("res://assets/Banks/Master.strings.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
## warning-ignore:return_value_discarded
#	Fmod.load_bank("res://assets/Banks/Master.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
## warning-ignore:return_value_discarded
#	Fmod.load_bank("res://assets/Banks/Music.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
## warning-ignore:return_value_discarded
#	Fmod.load_bank("res://assets/Banks/Vehicles.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
#	print("Fmod initialised.")

#func _ready():
#	Fmod.load_configuration("res://addons/fmod/fmod_config.cfg")
