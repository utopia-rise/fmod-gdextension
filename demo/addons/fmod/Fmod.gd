extends Node

var godot_fmod = preload("res://addons/fmod/Fmod.gdns").new()
var local_path_prefix = "./"

############
###UTILS###
############

const FMOD_INIT_3D_RIGHTHANDED = 4
const FMOD_INIT_CHANNEL_DISTANCEFILTER = 512
const FMOD_INIT_CHANNEL_LOWPASS = 256
const FMOD_INIT_DISABLE_SRS_HIGHPASSFILTER = 4194304
const FMOD_INIT_GEOMETRY_USECLOSEST = 262144
const FMOD_INIT_MIX_FROM_UPDATE = 2
const FMOD_INIT_NORMAL = 0
const FMOD_INIT_PREFER_DOLBY_DOWNMIX = 524288
const FMOD_INIT_PROFILE_ENABLE = 65536
const FMOD_INIT_PROFILE_METER_ALL = 2097152
const FMOD_INIT_STREAM_FROM_UPDATE = 1
const FMOD_INIT_THREAD_UNSAFE = 1048576
const FMOD_INIT_VOL0_BECOMES_VIRTUAL = 131072

const FMOD_STUDIO_INIT_ALLOW_MISSING_PLUGINS = 2
const FMOD_STUDIO_INIT_DEFERRED_CALLBACKS = 8
const FMOD_STUDIO_INIT_LIVEUPDATE = 1
const FMOD_STUDIO_INIT_LOAD_FROM_UPDATE = 16
const FMOD_STUDIO_INIT_NORMAL = 0

const FMOD_SPEAKERMODE_5POINT1 = 6
const FMOD_SPEAKERMODE_7POINT1 = 7
const FMOD_SPEAKERMODE_7POINT1POINT4 = 8
const FMOD_SPEAKERMODE_DEFAULT = 0
const FMOD_SPEAKERMODE_MAX = 9
const FMOD_SPEAKERMODE_MONO = 2
const FMOD_SPEAKERMODE_QUAD = 4
const FMOD_SPEAKERMODE_RAW = 1
const FMOD_SPEAKERMODE_STEREO = 3
const FMOD_SPEAKERMODE_SURROUND = 5

const FMOD_STUDIO_LOAD_BANK_NORMAL = 0
const FMOD_STUDIO_LOAD_BANK_NONBLOCKING = 1
const FMOD_STUDIO_LOAD_BANK_DECOMPRESS_SAMPLES = 2

const FMOD_2D = 8
const FMOD_3D = 16
const FMOD_3D_CUSTOMROLLOFF = 67108864
const FMOD_3D_HEADRELATIVE = 262144
const FMOD_3D_IGNOREGEOMETRY = 1073741824
const FMOD_3D_INVERSEROLLOFF = 1048576
const FMOD_3D_INVERSETAPEREDROLLOFF = 8388608
const FMOD_3D_LINEARROLLOFF = 2097152
const FMOD_3D_LINEARSQUAREROLLOFF = 4194304
const FMOD_3D_WORLDRELATIVE = 524288
const FMOD_ACCURATETIME = 16384
const FMOD_CREATECOMPRESSEDSAMPLE = 512
const FMOD_CREATESAMPLE = 256
const FMOD_CREATESTREAM = 128
const FMOD_DEFAULT = 0
const FMOD_IGNORETAGS = 33554432
const FMOD_LOOP_BIDI = 4
const FMOD_LOOP_NORMAL = 2
const FMOD_LOOP_OFF = 1
const FMOD_LOWMEM = 134217728
const FMOD_MPEGSEARCH = 32768
const FMOD_NONBLOCKING = 65536
const FMOD_OPENMEMORY = 2048
const FMOD_OPENMEMORY_POINT = 268435456
const FMOD_OPENONLY = 8192
const FMOD_OPENRAW = 4096
const FMOD_OPENUSER = 1024
const FMOD_UNIQUE = 131072
const FMOD_VIRTUAL_PLAYFROMSTART = -2147483648

const FMOD_STUDIO_STOP_ALLOWFADEOUT = 0
const FMOD_STUDIO_STOP_IMMEDIATE = 1
const FMOD_STUDIO_STOP_FORCEINT = 65536

func get_fmod_path(path: String) -> String:
	if path.left(2) == "./":
		return path.replace("./", local_path_prefix)
	return path

############
###SYSTEM###
############
func _init() -> void:
	if OS.get_name() == "Android":
		local_path_prefix = "file:///android_asset/"

func init(numOfChannels: int, studioFlag: int, fmodFlag: int) -> void:
	godot_fmod.init(1024, studioFlag, fmodFlag)
	
func _process(delta):
	godot_fmod.update()

func set_software_format(sampleRate: int, speakerMode: int, numRowSpeakers: int) -> void:
	godot_fmod.set_software_format(sampleRate, speakerMode, numRowSpeakers)

func add_listener(object) -> void:
	godot_fmod.add_listener(object)

##########
###BANK###
##########
func load_bank(pathToBank: String, loadBankFlag: int) -> String:
	return godot_fmod.load_bank(get_fmod_path(pathToBank), loadBankFlag)

func unload_bank(pathToBank: String) -> void:
	godot_fmod.unload_bank(get_fmod_path(pathToBank))


###########
###EVENT###
###########
func play_one_shot(eventPath: String, object) -> void:
	godot_fmod.play_one_shot(eventPath, object)

func play_one_shot_with_params(eventPath: String, object, params: Dictionary) -> void:
	godot_fmod.play_one_shot_with_params(eventPath, object, params)

func play_one_shot_attached(eventPath: String, object) -> void:
	godot_fmod.play_one_shot_attached(eventPath, object)

func play_one_shot_attached_with_params(eventPath: String, object: Object, params: Dictionary) -> void:
	godot_fmod.play_one_shot_attached_with_params(eventPath, object, params)

func create_event_instance(eventPath: String) -> int:
	return godot_fmod.create_event_instance(eventPath)

func start_event(instanceId: int) -> void:
	godot_fmod.start_event(instanceId)

func stop_event(instanceId: int, stopMode: int) -> void:
	godot_fmod.stop_event(instanceId, stopMode)

func release_event(instanceId: int) -> void:
	godot_fmod.release_event(instanceId)

func set_event_volume(instanceId: int, volume: float) -> void:
	godot_fmod.set_event_volume(instanceId, volume)

func set_event_paused(instanceId: int, paused: bool) -> void:
	godot_fmod.set_event_paused(instanceId, paused)

func set_event_pitch(instanceId: int, pitch: float) -> void:
	godot_fmod.set_event_pitch(instanceId, pitch)


func attach_instance_to_node(instanceId: int, object: Object) -> void:
	godot_fmod.attach_instance_to_node(instanceId, object)

func detach_instance_from_node(instanceId: int) -> void:
	godot_fmod.detach_instance_from_node(instanceId)

###########
###SOUND###
###########
func load_sound(path: String, mode: int) -> int:
	return godot_fmod.load_sound(get_fmod_path(path), mode)

func play_sound(instanceId: int) -> void:
	godot_fmod.play_sound(instanceId)

func stop_sound(instanceId: int) -> void:
	godot_fmod.stop_sound(instanceId)

func release_sound(instanceId: int) -> void:
	godot_fmod.release_sound(instanceId)

func set_sound_paused(instanceId: int, paused: bool) -> void:
	godot_fmod.set_sound_paused(instanceId, paused)

func set_sound_volume(instanceId: int, volume: float) -> void:
	godot_fmod.set_sound_volume(instanceId, volume)

func set_sound_pitch(instanceId: int, pitch: float) -> void:
	godot_fmod.set_sound_pitch(instanceId, pitch)

func set_sound_3D_settings(dopplerScale: float, distanceFactor: float, rollOffScale: float) -> void:
	godot_fmod.set_sound_3D_settings(dopplerScale, distanceFactor, rollOffScale)

