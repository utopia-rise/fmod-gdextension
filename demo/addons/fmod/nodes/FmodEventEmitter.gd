tool
class_name FmodEventEmitter, "res://addons/fmod/nodes/fmod.svg"
extends Node2D

const UNDEFINED = -1
const EVENT_PREFIX = "event:/"

export(String) var fmod_event_name = EVENT_PREFIX setget _set_event_name
export(bool) var attached = true
export(bool) var autoplay = false
export(bool) var looped = false
export(bool) var allow_fadeout = true
export(bool) var preload_event = true

var params = {}
var event_id = UNDEFINED

func _ready():
	for key in params:
		_set_fmod_param(key, params[key])
	if preload_event:
		Fmod.desc_load_sample_data(fmod_event_name)
	if autoplay:
		play()
		
func _exit_tree():
	if event_id != UNDEFINED:
		if attached:
			Fmod.detach_instance_from_node(event_id)
		if allow_fadeout:
			Fmod.stop_event(event_id, Fmod.FMOD_STUDIO_STOP_ALLOWFADEOUT)
		else:
			Fmod.stop_event(event_id, Fmod.FMOD_STUDIO_STOP_IMMEDIATE)
			
func set_param(key:String, value:float) -> void:
	params[key] = value
	_set_fmod_param(key, value)
	
func is_paused() -> bool:
	if event_id == UNDEFINED:
		return false
	return Fmod.get_event_paused(event_id)

func play() -> void:
	if is_paused():
		_unpause()
	elif looped:
		_play_looped()
	else:
		_play_one_shot()
		
func pause() -> void:
	if event_id != UNDEFINED:
		Fmod.set_event_paused(event_id, true)
	
func _unpause() -> void:
	if event_id != UNDEFINED:
		Fmod.set_event_paused(event_id, false)
		
func _play_one_shot() -> void:
	if !attached:
		if params.size() > 0:
			Fmod.play_one_shot_with_params(fmod_event_name, self, params)
		else:
			Fmod.play_one_shot(fmod_event_name, self)
	else:
		if params.size() > 0:
			Fmod.play_one_shot_attached_with_params(fmod_event_name, self, params)
		else:
			Fmod.play_one_shot_attached(fmod_event_name, self)

func _play_looped() -> void:
	if event_id != UNDEFINED:
		return
	event_id = Fmod.create_event_instance(fmod_event_name)
	Fmod.start_event(event_id)
	if attached:
		Fmod.attach_instance_to_node(event_id, self)
	for param in params:
		Fmod.set_event_parameter_by_name(event_id, param, params[param])

func _set_event_name(e:String) -> void:
	if e.begins_with(EVENT_PREFIX):
		fmod_event_name = e
	else:
		fmod_event_name = EVENT_PREFIX + e

func _set_fmod_param(key:String, value: float) -> void:
	if event_id != UNDEFINED:
		Fmod.set_event_parameter_by_name(event_id, key, value)
