extends Node

class_name FmodBankDatabase


static var banks := Array()
const MASTER_STRINGS_BANK_NAME = "Master.strings.bank"
const MASTER_BANK_NAME = "Master.bank"

static func reload_all_banks(): 
	banks.clear()
	
	var banks_root = ProjectSettings.get_setting("Fmod/General/banks_path", "")
	var master_strings_bank_path = "%s/%s" % [banks_root, MASTER_STRINGS_BANK_NAME]
	var master_bank_path = "%s/%s" % [banks_root, MASTER_BANK_NAME]
	
	if not FileAccess.file_exists(master_strings_bank_path):
		push_warning("Cannot find master strings bank at %s" % master_strings_bank_path)
		return
	
	if not FileAccess.file_exists(master_bank_path):
		push_warning("Cannot find master bank at %s" % master_bank_path)
		return
	
	banks.append(
		FmodServer.load_bank(master_strings_bank_path, FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	)
	banks.append(
		FmodServer.load_bank(master_bank_path, FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	)

	var dir: DirAccess = DirAccess.open(banks_root)
	if dir:
		dir.list_dir_begin()
		var file_name : String = dir.get_next()
		while file_name != "":
			if dir.current_is_dir():
				pass # the found item is a directory
			elif file_name.ends_with(".bank") and file_name != MASTER_STRINGS_BANK_NAME and file_name != MASTER_BANK_NAME:
				banks.append(
					FmodServer.load_bank("%s/%s" % [banks_root, file_name], FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
				)
			file_name = dir.get_next()
	
