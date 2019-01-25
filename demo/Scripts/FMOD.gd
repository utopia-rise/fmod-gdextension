# this script holds a reference to the FMOD module instance
# and allows FMOD functions to be called from any other script
extends Node

var FMOD # FMOD singleton

func _ready():
	FMOD = Fmod.new()
	
#warning-ignore:unused_argument
func _process(delta):
	FMOD.system_update()

##### system functions  #####
func system_init(num_of_channels, studio_flags, flags):
	return FMOD.system_init(num_of_channels, studio_flags, flags)
	
func system_update():
	return FMOD.system_update()
	
func system_shutdown():
	return FMOD.system_shutdown()
	
func system_add_listener(listener):
	return FMOD.system_add_listener(listener)
	
func system_set_software_format(sample_rate, speaker_mode, num_raw_speakers):
	return FMOD.system_set_software_format(sample_rate, speaker_mode, num_raw_speakers)


##### helper functions #####
func play_one_shot(event_name, node):
	return FMOD.play_one_shot(event_name, node)
	
func play_one_shot_attached(event_name, node):
	return FMOD.play_one_shot_attached(event_name, node)

func play_one_shot_with_params(event_name, node, params):
	return FMOD.play_one_shot_with_params(event_name, node, params)
	
func play_one_shot_attached_with_params(event_name, node, params):
	return FMOD.play_one_shot_attached_with_params(event_name, node, params)
	
func attach_instance_to_node(uuid, node):
	return FMOD.attach_instance_to_node(uuid, node)

func detach_instance_from_node(uuid):
	return FMOD.detach_instance_from_node(uuid)
	
	
##### bank functions #####
func bank_load(path_to_bank, flags):
	return FMOD.bank_load(path_to_bank, flags)
	
func bank_unload(path_to_bank):
	return FMOD.bank_unload(path_to_bank)
	
func bank_get_loading_state(path_to_bank):
	return FMOD.bank_get_loading_state(path_to_bank)
	
func bank_get_bus_count(path_to_bank):
	return FMOD.bank_get_bus_count(path_to_bank)
	
func bank_get_event_count(path_to_bank):
	return FMOD.bank_get_event_count(path_to_bank)
	
func bank_get_string_count(path_to_bank):
	return FMOD.bank_get_string_count(path_to_bank)
	
func bank_get_vca_count(path_to_bank):
	return FMOD.bank_get_vca_count(path_to_bank)
	

##### event functions #####
func event_create_instance(uuid, event_path):
	return FMOD.event_create_instance(uuid, event_path)
	
func event_get_parameter(uuid, parameter_name):
	return FMOD.event_get_parameter(uuid, parameter_name)
	
func event_set_parameter(uuid, parameter_name, value):
	return FMOD.event_set_parameter(uuid, parameter_name, value)
	
func event_release(uuid):
	return FMOD.event_release(uuid)
	
func event_start(uuid):
	return FMOD.event_start(uuid)
	
func event_stop(uuid, stop_mode):
	return FMOD.event_stop(uuid, stop_mode)
	
func event_trigger_cue(uuid):
	return FMOD.event_trigger_cue(uuid)
	
func event_get_playback_state(uuid):
	return FMOD.event_get_playback_state(uuid)

func event_get_paused(uuid):
	return FMOD.event_get_paused(uuid)

func event_set_paused(uuid, paused):
	return FMOD.event_set_paused(uuid, paused)

func event_set_pitch(uuid, pitch):
	return FMOD.event_set_pitch(uuid, pitch)

func event_get_pitch(uuid):
	return FMOD.event_get_pitch(uuid)
	
func event_get_volume(uuid):
	return FMOD.event_get_volume(uuid)

func event_set_volume(uuid, volume):
	return FMOD.event_set_volume(uuid, volume)

func event_get_timeline_position(uuid):
	return FMOD.event_get_timeline_position(uuid)
	
func event_set_timeline_position(uuid, position):
	return FMOD.event_set_timeline_position(uuid, position)
	
func event_get_reverb_level(uuid):
	return FMOD.event_get_reverb_level(uuid)
	
func event_set_reverb_level(uuid, reverb_level):
	return FMOD.event_set_reverb_level(uuid, reverb_level)
	
func event_is_virtual(uuid):
	return FMOD.event_is_virtual(uuid)
	

##### bus functions #####
func bus_get_mute(path_to_bus):
	return FMOD.bus_get_mute(path_to_bus)
	
func bus_get_paused(path_to_bus):
	return FMOD.bus_get_paused(path_to_bus)
	
func bus_get_volume(path_to_bus):
	return FMOD.bus_get_volume(path_to_bus)
	
func bus_set_mute(path_to_bus, mute):
	return FMOD.bus_set_mute(path_to_bus, mute)
	
func bus_set_paused(path_to_bus, paused):
	return FMOD.bus_set_paused(path_to_bus, paused)
	
func bus_set_volume(path_to_bus, volume):
	return FMOD.bus_set_volume(path_to_bus, volume)
	
func bus_stop_all_events(path_to_bus, stop_mode):
	return FMOD.bus_stop_all_events(path_to_bus, stop_mode)
	

##### VCA functions #####
func vca_get_volume(path_to_vca):
	return FMOD.vca_get_volume(path_to_vca)
	
func vca_set_volume(path_to_vca, volume):
	return FMOD.vca_set_volume(path_to_vca, volume)