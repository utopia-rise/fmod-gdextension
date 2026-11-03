extends Node

var master_string_bank: FmodBank
var master_bank: FmodBank
var music_bank: FmodBank
var vehicles_bank: FmodBank
var sfx_bank: FmodBank

# Called when the node enters the scene tree for the first time.
func _enter_tree():
	# load banks
# warning-ignore:return_value_discarded
	master_string_bank = FmodServer.load_bank("res://assets/Banks/Master.strings.bank", FmodBank.StudioLoadBankFlags.NORMAL)
# warning-ignore:return_value_discarded
	master_bank = FmodServer.load_bank("res://assets/Banks/Master.bank", FmodBank.StudioLoadBankFlags.NORMAL)
# warning-ignore:return_value_discarded
	music_bank = FmodServer.load_bank("res://assets/Banks/Music.bank", FmodBank.StudioLoadBankFlags.NORMAL)
# warning-ignore:return_value_discarded
	vehicles_bank = FmodServer.load_bank("res://assets/Banks/Vehicles.bank", FmodBank.StudioLoadBankFlags.NORMAL)
# warning-ignore:return_value_discarded
	sfx_bank = FmodServer.load_bank("res://assets/Banks/SFX.bank", FmodBank.StudioLoadBankFlags.NORMAL)
	print("Fmod initialised.")
