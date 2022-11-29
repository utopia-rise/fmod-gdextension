tool
extends VBoxContainer
class_name FMOD_Config

###Init variables
onready var sample_rate_selector = get_node("%SampleRateSelector")
onready var speaker_mode_selector = get_node("%SpeakerModeSelector")
onready var LU_check = get_node("%LiveUpdateCheck")
onready var num_channels_box = get_node("%NumChannelsBox")
onready var num_listeners_value = get_node("%NumListenersValue")
onready var dsp_buffer_size = get_node("%DSPBufferSize")
onready var debug_checker = get_node("%DebugChecker")
onready var load_fmod_on_start_check = get_node("%LoadFMODOnStartCheck")

onready var banks_file_path = get_node("%BankFilePath")
onready var banks_to_load = get_node("%BanksToLoad")

onready var dopler_scale_slider = get_node("%DoplerScaleSlider")
onready var dist_factor_value = get_node("%DistFactorValue")
onready var rolloff_value = get_node("%RolloffValue")

var config_file_path = "res://fmod/fmod_config.cfg"

# Called when the node enters the scene tree for the first time.
func _ready():
	get_node("%ApplyChanges").connect("pressed", self, "apply_changes")
	get_node("%UndoChanges").connect("pressed", self, "load_file")
	get_node("%LoadBanksButton").connect("pressed", self, "load_bank_names")
	get_node("%ClearBanksButton").connect("pressed", self, "clear_bank_names")
	get_node("%DoplerScaleSlider").connect("value_changed", self, "update_dop_val_label")
	get_node("%RolloffValue").connect("value_changed", self, "update_rolloff_scale_label")
	get_node("%FolderSelect").connect("pressed", self, "show_file_dialogue")
	get_node("%FileDialog").connect("dir_selected", self, "update_bank_path")
	
	load_file()

func load_file():
	var config = ConfigFile.new()
	
	var err = config.load(config_file_path)

	if err != OK:
		#Set Default Values
		config.set_value("Init", "SRIndex", 1)
		config.set_value("Init", "SampleRate", 44100)
		config.set_value("Init", "LiveUpdate", false)
		config.set_value("Init", "SpeakerMode", 3)
		config.set_value("Init", "NumChannels", 16)
		config.set_value("Init", "NumListeners", 2)
		config.set_value("Init", "DSPBufferSize", 1048)
		config.set_value("Init", "ShowDebug", false)
		config.set_value("Init", "LoadFMODOnStart", true)
		
		config.set_value("Banks", "Location", "")
		config.set_value("Banks", "Autoload", "Master\n")
		
		config.set_value("3D", "DoplerScale", 1.0)
		config.set_value("3D", "DistanceFactor", 32.0)
		config.set_value("3D", "RolloffScale", 1.0)
		
		config.save(config_file_path)

	#Change input locations to show loaded values
	sample_rate_selector.selected = config.get_value("Init", "SRIndex")
	speaker_mode_selector.selected = config.get_value("Init", "SpeakerMode")
	LU_check.pressed = config.get_value("Init", "LiveUpdate")
	num_channels_box.value = config.get_value("Init", "NumChannels")
	num_listeners_value.value = config.get_value("Init", "NumListeners")
	dsp_buffer_size.value = config.get_value("Init", "DSPBufferSize")
	debug_checker.pressed = config.get_value("Init", "ShowDebug")
	load_fmod_on_start_check.pressed = config.get_value("Init", "LoadFMODOnStart")
	
	banks_file_path.text = config.get_value("Banks", "Location")
	banks_to_load.text = config.get_value("Banks", "Autoload")
	
	dopler_scale_slider.value = config.get_value("3D", "DoplerScale")
	dist_factor_value.value = config.get_value("3D", "DistanceFactor")
	rolloff_value.value = config.get_value("3D", "RolloffScale")

func apply_changes():
	var config = ConfigFile.new()
	config.set_value("Init", "SRIndex", sample_rate_selector.selected)
	config.set_value("Init", "SampleRate", int(sample_rate_selector.text))
	config.set_value("Init", "SpeakerMode", speaker_mode_selector.selected)
	config.set_value("Init", "LiveUpdate", LU_check.pressed)
	config.set_value("Init", "NumChannels", int(num_channels_box.value))
	config.set_value("Init", "NumListeners", int(num_listeners_value.value))
	config.set_value("Init", "DSPBufferSize", int(dsp_buffer_size.value))
	config.set_value("Init", "ShowDebug", debug_checker.pressed)
	config.set_value("Init", "LoadFMODOnStart", load_fmod_on_start_check.pressed)
	
	config.set_value("Banks", "Location", banks_file_path.text)
	if !banks_to_load.text == "Incorrect Bank Location":
		config.set_value("Banks", "Autoload", banks_to_load.text)
	else:
		config.set_value("Banks", "Autoload", "")
	
	config.set_value("3D", "DoplerScale", dopler_scale_slider.value)
	config.set_value("3D", "DistanceFactor", dist_factor_value.value)
	config.set_value("3D", "RolloffScale", rolloff_value.value)
	
	config.save(config_file_path)

func load_bank_names():
	var files = ["Master"]
	var dir = Directory.new()
	
	if !dir.file_exists(banks_file_path.text + "/Master.bank"):
		#banks_to_load.text = "Incorrect Bank Location"
		get_node("%BankErrorPopup").popup_centered(Vector2(500, 50))
		banks_to_load.text = ""
		return
	
	dir.open(banks_file_path.text)
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
	
	banks_to_load.text = ""
	for file in files:
		banks_to_load.text += (file + "\n")

func clear_bank_names():
	banks_to_load.text = ""

func update_dop_val_label(val):
	get_node("%DopScaleNum").text = str(val)

func update_rolloff_scale_label(val):
	get_node("%RolloffScaleNum").text = str(val)

func show_file_dialogue():
	get_node("%FileDialog").popup_centered(Vector2(500, 500))

func update_bank_path(path):
	banks_file_path.text = path + "/"
