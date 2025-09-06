@tool
extends Node

## ------------------
## Autoload:  FmodManager
## ------------------


## Used to store the Master Bank for an Fmod Project
## (Can store other banks as well if they need to be consistantly present)
@onready var master_Bank_Loader: FmodBankLoader = %MasterBankLoader

var performance_display: PerformancesDisplay

func _ready() -> void:
	if self != FmodManager: return
	
	process_mode = PROCESS_MODE_ALWAYS
	performance_display = PerformancesDisplay.new()
	add_child(performance_display)
	
	_check_Master_Bank_Exists()
	

func _check_Master_Bank_Exists() -> void:
	if master_Bank_Loader.bank_paths.size() < 1:
		push_warning("No Master Bank found in MasterBankLoader in FmodManager")
	


func _exit_tree() -> void:
	if self != FmodManager: return
	
	remove_child(performance_display)
	performance_display.free()
	

func _process(delta: float) -> void:
	if self != FmodManager: return
	
	FmodServer.update()
	

func _notification(what: int) -> void:
	if self != FmodManager: return
	
	FmodServer.notification(what)
	
