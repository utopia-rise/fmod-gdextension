# Low level api

You can directly access `FmodServer` to access fmod api.  

## Attach to existing event, 3D positioning

For example you can directly create an instance of an event using `FmodServer`, set its position attribute, paramters,
etc.  
Here is an example:
```gdscript
extends Sprite2D

var isPlaying: bool = true
var event: FmodEvent = null

# Called when the node enters the scene tree for the first time.
func _ready():
	event = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
	event.set_2d_attributes(self.global_transform)
	event.set_parameter_by_name("RPM", 600)
	event.set_volume( 2)
	event.start()
	
# warning-ignore:unused_argument
func _process(_delta):
	if Input.is_action_just_pressed("space"):
		isPlaying = !isPlaying
		if(isPlaying):
			print("Mower playing")
			event.set_paused(false)
		else:
			print("Mower paused")
			event.set_paused(true)
	elif Input.is_action_just_pressed("kill_event"):
		self.queue_free()
```

## Timeline marker & music beat callbacks

Here is another with setting callback:
```gdscript
extends Area2D

var event: FmodEvent = null
var icon: Sprite2D

# Called when the node enters the scene tree for the first time.
func _ready():
	event = FmodServer.create_event_instance("event:/Music/Level 02")
	event.set_callback(Callable(self, "change_color"), FmodServer.FMOD_STUDIO_EVENT_CALLBACK_ALL)
	body_entered.connect(enter)
	body_exited.connect(leave)
	event.start()
	event.set_paused(true)

# warning-ignore:unused_argument
func enter(_area):
	print("enter")
	event.set_paused(false)
	
# warning-ignore:unused_argument
func leave(_area):
	print("leave")
	event.set_paused(true)

# warning-ignore:unused_argument
func change_color(_dict: Dictionary, type: int):
	if type == FmodServer.FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT:
		$icon.self_modulate = Color(randf_range(0,1), randf_range(0,1), randf_range(0,1), 1)
```

## Playing sounds using FMOD Core / Low Level API

You can also access Fmod low level api to play sound using `FmodServer`:
```gdscript
extends Area2D

# Declare member variables here. Examples:
# var a = 2
var music: FmodSound = null

# Called when the node enters the scene tree for the first time.
func _ready():
	FmodServer.load_file_as_music("res://assets/Music/jingles_SAX07.ogg")
# warning-ignore:return_value_discarded
	body_entered.connect(enter)
# warning-ignore:return_value_discarded
	body_exited.connect(leave)

# warning-ignore:unused_argument
func enter(_area):
	print("enter")
	music = FmodServer.create_sound_instance("res://assets/Music/jingles_SAX07.ogg")
	music.play()
	
# warning-ignore:unused_argument
func leave(_area):
	print("leave")
	music.release()

func _exit_tree():
	FmodServer.unload_file("res://assets/Music/jingles_SAX07.ogg")
```

## Muting all event

You can mute all event using `mute_all_events`. This will mute the master bus.

```gdscript
func _ready():
	FmodServer.set_software_format(0, FmodServer.FMOD_SPEAKERMODE_STEREO, 0)
	FmodServer.init(1024, FmodServer.FMOD_STUDIO_INIT_LIVEUPDATE, FmodServer.FMOD_INIT_NORMAL)
	FmodServer.set_sound_3d_settings(1.0, 64.0, 1.0)
	
	# load banks
	FmodServer.load_bank("res://Master.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	FmodServer.load_bank("res://Master.strings.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	FmodServer.load_bank("res://Music.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	
	# register listener
	FmodServer.add_listener(0, self)
	
	# play some events
	FmodServer.play_one_shot("event:/Music/Level 02", self)
	var my_music_event = FmodServer.create_event_instance("event:/Music/Level 01")
	FmodServer.start_event(my_music_event)
	var t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	FmodServer.mute_all_events();
	t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	FmodServer.unmute_all_events()
```

## Pausing all events

```gdscript
func _ready():
	# set up FMOD
	FmodServer.set_software_format(0, FmodServer.FMOD_SPEAKERMODE_STEREO, 0)
	FmodServer.init(1024, FmodServer.FMOD_STUDIO_INIT_LIVEUPDATE, FmodServer.FMOD_INIT_NORMAL)
	FmodServer.set_sound_3d_settings(1/0, 64.0, 1.0)
	
	# load banks
	FmodServer.load_bank("res://Master.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	FmodServer.load_bank("res://Master.strings.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	FmodServer.load_bank("res://Music.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
	
	# register listener
	FmodServer.add_listener(0, self)
	
	# play some events
	FmodServer.play_one_shot("event:/Music/Level 02", self)
	var my_music_event = FmodServer.create_event_instance("event:/Music/Level 01")
	FmodServer.start_event(my_music_event)
	var t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	FmodServer.pause_all_events(true)
	t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	FmodServer.pause_all_events(false)
```

## Changing the default audio output device

By default, FMOD will use the primary audio output device as determined by the operating system. This can be changed at runtime, ideally through your game's Options Menu.

Here, `get_available_drivers()` returns an Array which contains a Dictionary for every audio driver found. Each Dictionary contains fields such as the name, sample rate
and speaker config of the respective driver. Most importantly, it contains the id for that driver.

 ```gdscript
# retrieve all available audio drivers
var drivers = FmodServer.get_available_drivers()
 # change the audio driver
# you must pass in the id of the respective driver
FmodServer.set_driver(id)
 # retrieve the id of the currently set driver
var id = FmodServer.get_driver()
```

## Reducing audio playback latency

You may encounter that the audio playback has some latency. This may be caused by the DSP buffer size. You can change the value **before** initialisation to adjust it:
```gdscript
FmodServer.set_dsp_buffer_size(512, 4)
# retrieve the buffer length
FmodServer.get_dsp_buffer_length()
# retrieve the number of buffers
FmodServer.get_dsp_num_buffers()
```

## Profiling & querying performance data

`get_performance_data` returns an object which contains current performance stats for CPU, Memory and File Streaming usage of both FMOD Studio and the Core System.

```gdscript
# called every frame
var perf_data = FmodServer.get_performance_data()

print(perf_data.CPU)
print(perf_data.memory)
print(perf_data.file)
```