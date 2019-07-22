extends Node
signal timeline_beat
signal timeline_marker

var godot_fmod = FmodNative.new()

############
###UTILS###
############
const FMOD_INIT_NORMAL = 0x00000000
const FMOD_INIT_STREAM_FROM_UPDATE = 0x00000001
const FMOD_INIT_MIX_FROM_UPDATE = 0x00000002
const FMOD_INIT_3D_RIGHTHANDED = 0x00000004
const FMOD_INIT_CHANNEL_LOWPASS = 0x00000100
const FMOD_INIT_CHANNEL_DISTANCEFILTER = 0x00000200
const FMOD_INIT_PROFILE_ENABLE = 0x00010000
const FMOD_INIT_VOL0_BECOMES_VIRTUAL = 0x00020000
const FMOD_INIT_GEOMETRY_USECLOSEST = 0x00040000
const FMOD_INIT_PREFER_DOLBY_DOWNMIX = 0x00080000
const FMOD_INIT_THREAD_UNSAFE = 0x00100000
const FMOD_INIT_PROFILE_METER_ALL = 0x00200000
const FMOD_INIT_DISABLE_SRS_HIGHPASSFILTER = 0x00400000

const FMOD_STUDIO_INIT_NORMAL = 0x00000000
const FMOD_STUDIO_INIT_LIVEUPDATE = 0x00000001
const FMOD_STUDIO_INIT_ALLOW_MISSING_PLUGINS = 0x00000002
const FMOD_STUDIO_INIT_SYNCHRONOUS_UPDATE = 0x00000004
const FMOD_STUDIO_INIT_DEFERRED_CALLBACKS = 0x00000008
const FMOD_STUDIO_INIT_LOAD_FROM_UPDATE = 0x00000010

const FMOD_SPEAKERMODE_DEFAULT = 0
const FMOD_SPEAKERMODE_RAW = 1
const FMOD_SPEAKERMODE_MONO = 2
const FMOD_SPEAKERMODE_STEREO = 3
const FMOD_SPEAKERMODE_QUAD = 4
const FMOD_SPEAKERMODE_SURROUND = 5
const FMOD_SPEAKERMODE_5POINT1 = 6
const FMOD_SPEAKERMODE_7POINT1 = 7
const FMOD_SPEAKERMODE_7POINT1POINT4 = 8

const FMOD_STUDIO_LOAD_BANK_NORMAL = 0x00000000
const FMOD_STUDIO_LOAD_BANK_NONBLOCKING = 0x00000001
const FMOD_STUDIO_LOAD_BANK_DECOMPRESS_SAMPLES = 0x00000002

const FMOD_DEFAULT = 0x00000000
const FMOD_LOOP_OFF = 0x00000001
const FMOD_LOOP_NORMAL = 0x00000002
const FMOD_LOOP_BIDI = 0x00000004
const FMOD_2D = 0x00000008
const FMOD_3D = 0x00000010
const FMOD_CREATESTREAM = 0x00000080
const FMOD_CREATESAMPLE = 0x00000100
const FMOD_CREATECOMPRESSEDSAMPLE = 0x00000200
const FMOD_OPENUSER = 0x00000400
const FMOD_OPENMEMORY = 0x00000800
const FMOD_OPENMEMORY_POINT = 0x10000000
const FMOD_OPENRAW = 0x00001000
const FMOD_OPENONLY = 0x00002000
const FMOD_ACCURATETIME = 0x00004000
const FMOD_MPEGSEARCH = 0x00008000
const FMOD_NONBLOCKING = 0x00010000
const FMOD_UNIQUE = 0x00020000
const FMOD_3D_HEADRELATIVE = 0x00040000
const FMOD_3D_WORLDRELATIVE = 0x00080000
const FMOD_3D_INVERSEROLLOFF = 0x00100000
const FMOD_3D_LINEARROLLOFF = 0x00200000
const FMOD_3D_LINEARSQUAREROLLOFF = 0x00400000
const FMOD_3D_INVERSETAPEREDROLLOFF = 0x00800000
const FMOD_3D_CUSTOMROLLOFF = 0x04000000
const FMOD_3D_IGNOREGEOMETRY = 0x40000000
const FMOD_IGNORETAGS = 0x02000000
const FMOD_LOWMEM = 0x08000000
const FMOD_LOADSECONDARYRAM = 0x20000000
const FMOD_VIRTUAL_PLAYFROMSTART = 0x80000000

const FMOD_STUDIO_EVENT_CALLBACK_CREATED = 0x00000001
const FMOD_STUDIO_EVENT_CALLBACK_DESTROYED = 0x00000002
const FMOD_STUDIO_EVENT_CALLBACK_STARTING = 0x00000004
const FMOD_STUDIO_EVENT_CALLBACK_STARTED = 0x00000008
const FMOD_STUDIO_EVENT_CALLBACK_RESTARTED = 0x00000010
const FMOD_STUDIO_EVENT_CALLBACK_STOPPED = 0x00000020
const FMOD_STUDIO_EVENT_CALLBACK_START_FAILED = 0x00000040
const FMOD_STUDIO_EVENT_CALLBACK_CREATE_PROGRAMMER_SOUND = 0x00000080
const FMOD_STUDIO_EVENT_CALLBACK_DESTROY_PROGRAMMER_SOUND = 0x00000100
const FMOD_STUDIO_EVENT_CALLBACK_PLUGIN_CREATED = 0x00000200
const FMOD_STUDIO_EVENT_CALLBACK_PLUGIN_DESTROYED = 0x00000400
const FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER = 0x00000800
const FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT = 0x00001000
const FMOD_STUDIO_EVENT_CALLBACK_SOUND_PLAYED = 0x00002000
const FMOD_STUDIO_EVENT_CALLBACK_SOUND_STOPPED = 0x00004000
const FMOD_STUDIO_EVENT_CALLBACK_REAL_TO_VIRTUAL = 0x00008000
const FMOD_STUDIO_EVENT_CALLBACK_VIRTUAL_TO_REAL = 0x00010000
const FMOD_STUDIO_EVENT_CALLBACK_ALL = 0xFFFFFFFF

const FMOD_STUDIO_STOP_ALLOWFADEOUT = 0
const FMOD_STUDIO_STOP_IMMEDIATE = 1
const FMOD_STUDIO_STOP_FORCEINT = 65536

############
###SYSTEM###
############
func _init() -> void:
	godot_fmod.connect("timeline_beat", self, "on_timeline_beat")
	godot_fmod.connect("timeline_marker", self, "on_timeline_marker")
	print("Fmod Gdnative interface managed by a GDScript wrapper")

func _process(delta):
	godot_fmod._process(delta)

func init(numOfChannels: int, studioFlag: int, fmodFlag: int) -> void:
	godot_fmod.init(numOfChannels, studioFlag, fmodFlag)
	
func shutdown() -> void:
	godot_fmod.shutdown()

func set_software_format(sampleRate: int, speakerMode: int, numRowSpeakers: int) -> void:
	godot_fmod.set_software_format(sampleRate, speakerMode, numRowSpeakers)

func add_listener(object) -> void:
	godot_fmod.add_listener(object)

func set_sound_3D_settings(dopplerScale: float, distanceFactor: float, rollOffScale: float) -> void:
	godot_fmod.set_sound_3D_settings(dopplerScale, distanceFactor, rollOffScale)

func wait_for_all_loads() -> void:
	godot_fmod.wait_for_all_loads()
	
func get_available_drivers() -> Array:
	return godot_fmod.get_available_drivers()
	
func get_driver() -> int:
	return godot_fmod.get_driver()
	
func set_driver(id: int) -> void:
	godot_fmod.set_driver(id)
	
func get_performance_data() -> Array:
	return godot_fmod.get_performance_data()
	
func set_global_parameter(parameterName: String, value: float) -> void:
	godot_fmod.set_global_parameter(parameterName, value)
	
func get_global_parameter(parameterName: String) -> float:
	return godot_fmod.get_global_parameter(parameterName)


##########
###BANK###
##########
func load_bank(pathTo_bank: String, load_bankFlag: int) -> String:
	return godot_fmod.load_bank(pathTo_bank, load_bankFlag)

func unload_bank(pathTo_bank: String) -> void:
	godot_fmod.unload_bank(pathTo_bank)
	
func get_bank_loading_state(pathTo_bank: String) -> int:
	return godot_fmod.get_bank_loading_state(pathTo_bank)
	
func get_bank_bus_count(pathTo_bank: String) -> int:
	return godot_fmod.get_bank_bus_count(pathTo_bank)
	
func get_bank_event_count(pathTo_bank: String) -> int:
	return godot_fmod.get_bank_event_count(pathTo_bank)
	
func get_bankString_count(pathTo_bank: String) -> int:
	return godot_fmod.get_bankString_count(pathTo_bank)
	
func get_bank_VCA_count(pathTo_bank: String) -> int:
	return godot_fmod.get_bank_VCA_count(pathTo_bank)
	
func banks_still_loading() -> bool:
	return godot_fmod.banks_still_loading()


####################
###EVENT_INSTANCE###
####################
func play_one_shot(event_path: String, object) -> void:
	godot_fmod.play_one_shot(event_path, object)

func play_one_shot_with_params(event_path: String, object, params: Dictionary) -> void:
	godot_fmod.play_one_shot_with_params(event_path, object, params)

func play_one_shot_attached(event_path: String, object) -> void:
	godot_fmod.play_one_shot_attached(event_path, object)

func play_one_shot_attached_with_params(event_path: String, object: Object, params: Dictionary) -> void:
	godot_fmod.play_one_shot_attached_with_params(event_path, object, params)

func create_event_instance(event_path: String) -> int:
	return godot_fmod.create_event_instance(event_path)

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

func get_event_parameter(instanceId: int, parameterName: String) -> float:
	return godot_fmod.get_event_parameter(instanceId, parameterName)
	
func set_event_parameter(instanceId: int, parameterName: String, value: float) -> void:
	godot_fmod.set_event_parameter(instanceId, parameterName, value)
	
func trigger_event_cue(instanceId: int) -> void:
	godot_fmod.trigger_event_cue(instanceId)
	
func get_event_playback_state(instanceId: int) -> int:
	return godot_fmod.get_event_playback_state(instanceId)
	
func get_event_timeline_position(instanceId: int) -> int:
	return godot_fmod.get_event_timeline_position(instanceId)
	
func set_event_timeline_position(instanceId: int, position: int) -> void:
	godot_fmod.set_event_timeline_position(instanceId, position)
	
func get_event_reverb_level(instanceId: int,index: int) -> float:
	return godot_fmod.get_event_reverb_level(instanceId, index)
	
func set_event_reverb_level(instanceId: int, index: int, level: float) -> void:
	godot_fmod.set_event_reverb_level(instanceId, index, level)
	
func is_event_virtual(instanceId: int) -> bool:
	return godot_fmod.is_event_virtual(instanceId)
	
func pause_all_events(pause: bool) -> void:
	godot_fmod.pause_all_events(pause)
	
func mute_all_events() -> void:
	godot_fmod.mute_all_events()
	
func unmute_all_events() -> void:
	godot_fmod.unmute_all_events()
	
func set_callback(instanceId: int, callbackMask: int) -> void:
	godot_fmod.set_callback(instanceId, callbackMask)

###########
###SOUND###
###########
func load_sound(path: String, mode: int) -> int:
	return godot_fmod.load_sound(path, mode)

func release_sound(instanceId: int) -> void:
	godot_fmod.release_sound(instanceId)
	
func play_sound(instanceId: int) -> void:
	godot_fmod.play_sound(instanceId)

func stop_sound(instanceId: int) -> void:
	godot_fmod.stop_sound(instanceId)

func set_sound_paused(instanceId: int, paused: bool) -> void:
	godot_fmod.set_sound_paused(instanceId, paused)

func set_sound_volume(instanceId: int, volume: float) -> void:
	godot_fmod.set_sound_volume(instanceId, volume)

func set_sound_pitch(instanceId: int, pitch: float) -> void:
	godot_fmod.set_sound_pitch(instanceId, pitch)
	
func get_sound_volume(instanceId: int) -> float:
	return godot_fmod.get_sound_volume(instanceId)
	
func get_sound_pitch(instanceId: int) -> float:
	return godot_fmod.get_sound_pitch(instanceId)
	
func is_sound_playing(instanceId: int) -> bool:
	return godot_fmod.is_sound_playing(instanceId)

#######################
###EVENT_DESCRIPTION###
#######################
func check_event_path(event_path: String) -> bool:
	return godot_fmod.check_event_path(event_path)


###########
###_VCA#####
###########
func check_VCA_path(_VCA_path: String) -> bool:
	return godot_fmod.check_VCA_path(_VCA_path)
	
func get_VCA_volume(_VCA_path: String) -> float:
	return godot_fmod.get_VCA_volume(_VCA_path)
	
func set_VCA_volume(_VCA_path: String, volume: float) -> void:
	godot_fmod.set_VCA_volume(_VCA_path, volume)


#########
###BUS###
#########
func check_bus_path(bus_path: String) -> bool:
	return godot_fmod.check_bus_path(bus_path)
	
func get_bus_mute(bus_path: String) -> bool:
	return godot_fmod.get_bus_mute(bus_path)
	
func get_bus_paused(bus_path: String) -> bool:
	return godot_fmod.get_bus_paused(bus_path)
	
func get_bus_volume(bus_path: String) -> float:
	return godot_fmod.get_bus_volume(bus_path)
	
func set_bus_mute(bus_path: String, mute: bool) -> void:
	godot_fmod.set_bus_mute(bus_path, mute)
	
func set_bus_paused(bus_path: String, paused: bool) -> void:
	godot_fmod.set_bus_paused(bus_path, paused)
	
func set_bus_volume(bus_path: String, volume: float) -> void:
	godot_fmod.set_bus_volume(bus_path, volume)
	
func stop_all_bus_events(bus_path: String, stopMode: int) -> void:
	godot_fmod.stop_all_bus_events(bus_path, stopMode)
	
#########
###Signal###
#########

func on_timeline_beat(dict : Dictionary) -> void:
	emit_signal("timeline_beat", dict)
	
func on_timeline_marker(dict : Dictionary) -> void:
	emit_signal("timeline_marker", dict)