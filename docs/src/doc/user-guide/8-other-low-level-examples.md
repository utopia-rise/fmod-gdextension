# Other FmodServer api examples

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