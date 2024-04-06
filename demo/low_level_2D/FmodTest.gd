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
