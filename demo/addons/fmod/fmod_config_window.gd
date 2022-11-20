tool
extends VBoxContainer
class_name FMOD_Config

# Called when the node enters the scene tree for the first time.
func _ready():
	get_node("%ApplyChanges").connect("pressed", self, "apply_changes")
	get_node("%UndoChanges").connect("pressed", self, "load_file")
	get_node("%LoadBanksButton").connect("pressed", self, "load_bank_names")
	get_node("%ClearBanksButton").connect("pressed", self, "clear_bank_names")
	get_node("%DoplerScaleSlider").connect("value_changed", self, "update_dop_val_label")
	get_node("%RolloffValue").connect("value_changed", self, "update_rolloff_scale_label")
	
	load_file()

func load_file():
	var config = ConfigFile.new()
	
	var err = config.load("res://addons/fmod/fmod_config.cfg")

	if err != OK:
		#Set Default Values
		config.set_value("Init", "SRIndex", 1)
		config.set_value("Init", "SampleRate", 44100)
		config.set_value("Init", "LiveUpdate", false)
		config.set_value("Init", "SpeakerMode", 3)
		config.set_value("Banks", "Location", "Hello There Friend")
		config.set_value("Init", "NumChannels", 64)
		return

	#Change input locations to show loaded values
	get_node("%SampleRateSelector").selected = config.get_value("Init", "SRIndex")
	get_node("%SpeakerModeSelector").selected = config.get_value("Init", "SpeakerMode")
	get_node("%LiveUpdateCheck").pressed = config.get_value("Init", "LiveUpdate")
	get_node("%NumChannelsBox").value = config.get_value("Init", "NumChannels")
	get_node("%NumListenersValue").value = config.get_value("Init", "NumListeners")
	get_node("%DSPBufferSize").value = config.get_value("Init", "DSPBufferSize")
	get_node("%DebugChecker").pressed = config.get_value("Init", "ShowDebug")
	get_node("%LoadFMODOnStartCheck").pressed = config.get_value("Init", "LoadFMODOnStart")
	
	get_node("%BankFilePath").text = config.get_value("Banks", "Location")
	get_node("%BanksToLoad").text = config.get_value("Banks", "Autoload")
	
	get_node("%DoplerScaleSlider").value = config.get_value("3D", "DoplerScale")
	get_node("%DistFactorValue").value = config.get_value("3D", "DistanceFactor")
	get_node("%RolloffValue").value = config.get_value("3D", "RolloffScale")
	
	
	config.save("res://addons/fmod/fmod_config.cfg")

func apply_changes():
	var config = ConfigFile.new()
	config.set_value("Init", "SRIndex", get_node("%SampleRateSelector").selected)
	config.set_value("Init", "SampleRate", int(get_node("%SampleRateSelector").text))
	config.set_value("Init", "SpeakerMode", get_node("%SpeakerModeSelector").selected)
	config.set_value("Init", "LiveUpdate", get_node("%LiveUpdateCheck").pressed)
	config.set_value("Init", "NumChannels", int(get_node("%NumChannelsBox").value))
	config.set_value("Init", "NumListeners", int(get_node("%NumListenersValue").value))
	config.set_value("Init", "DSPBufferSize", int(get_node("%DSPBufferSize").value))
	config.set_value("Init", "ShowDebug", get_node("%DebugChecker").pressed)
	config.set_value("Init", "LoadFMODOnStart", get_node("%LoadFMODOnStartCheck").pressed)
	
	config.set_value("Banks", "Location", get_node("%BankFilePath").text)
	config.set_value("Banks", "Autoload", get_node("%BanksToLoad").text)
	
	config.set_value("3D", "DoplerScale", get_node("%DoplerScaleSlider").value)
	config.set_value("3D", "DistanceFactor", get_node("%DistFactorValue").value)
	config.set_value("3D", "RolloffScale", get_node("%RolloffValue").value)
	
	config.save("res://addons/fmod/fmod_config.cfg")

func load_bank_names():
	var files = []
	var dir = Directory.new()
	dir.open(get_node("%BankFilePath").text)
	dir.list_dir_begin()
	
	while true:
		var file = dir.get_next()
		if file == "":
			break
		elif not file.begins_with("."):
			file = file.trim_suffix(".bank")
			file = file.trim_suffix(".strings")
			if not file in files:
				files.append(file)
	dir.list_dir_end()
	
	get_node("%BanksToLoad").text = ""
	for file in files:
		get_node("%BanksToLoad").text += (file + "\n")

func clear_bank_names():
	get_node("%BanksToLoad").text = "Master\n"

func update_dop_val_label(val):
	get_node("%DopScaleNum").text = str(val)

func update_rolloff_scale_label(val):
	get_node("%RolloffScaleNum").text = str(val)
