# Playing events

In this guide we'll explore how to play Fmod events.  

## FmodEventEmitter nodes

`FmodEventEmitter2D` and `FmodEventEmitter3D` are nodes which play Fmod events.
Events and their parameters are loaded by id or name according to [Fmod General settings](./2-initialization.md#general).

![emitter-image]

### Properties

First, set the event that the emitter will play. You can set the name or guid of the event manually, or use the event selection button.  

Then, there are a few options you can toggle:
- *attached*: if `true`, the Fmod event's position will update alongside the node's position.  
- *autoplay*: if `true`, the event will autoplay (if `false`, it will play when the `play()` function is called on it).  
- *auto_release*: if `true`, the emitter node will be automatically freed when the event finishes playing.  
- *allow_fadeout*: if `true`, the event will fade out when stopped.  
- *preload_event*: if `true`, the event will be preloaded when the node is ready. 

#### Fmod parameters

Event emitters have dynamics properties corresponding to fmod parameters associated with the current event.  
You can set their values like any other godot float property.  
From scripts, you can change them using `get` and `set` object's operators.  
Example:
```gdscript
extends FmodEventEmitter2D

func _process(_delta):
	if Input.is_action_pressed("engine_power_up"):
		self["fmod_parameters/RPM"] = self["fmod_parameters/RPM"] + 10
	if Input.is_action_pressed("engine_power_down"):
		self["fmod_parameters/RPM"] = self["fmod_parameters/RPM"] - 10
```  
To easily retrieve fmod parameters properties path, you can use godot's `Copy Property Path` functionality.  

You can also use following methods:  
- `get_parameter`: Get the value of a parameter using its name.  
- `set_parameter`: Set the value of a parameter using its name.  
- `get_parameter_by_id`: Get the value of a parameter using its id.  
- `set_parameter_by_id`: Set the value of a parameter using its id.  

Example:  
```gdscript
emitter.set_parameter("RPM", 1000)
emitter.set_parameter_by_id(5864137074015534804, 1000)

emitter.get_parameter("RPM")
emitter.get_parameter_by_id(5864137074015534804)
```

### Signals

`FmodEventEmitter2D` and `FmodEventEmitter3D` emits signals:
#### timeline_beat
Emitted on fmod event's timeline beat callback.
Parameters (as dictionary):  
- `beat`: Beat number within bar (starting from 1).  
- `bar`: Bar number (starting from 1).  
- `tempo`: Current tempo in beats per minute.  
- `time_signature_upper`: Current time signature upper number (beats per bar).  
- `time_signature_lower`: Current time signature lower number (beat unit).  
- `position`: Position of the beat on the timeline in milliseconds.  

#### timeline_marker:
Emitted when fmod event timeline passes a named marker.
Parameters (as dictionary):  
- `name`: Marker name.  
- `position`: Position of the marker on the timeline in milliseconds.  

#### started
Emitted when event starts and was not playing.
No parameters.

#### restart
Emitted when event starts and was playing.
No parameters.

#### stopped
Emitted when event has stopped.
No parameters.

### Methods

#### play

Starts the event.

*parameters:*  
- `restart_if_playing`: If true, will restart event if it is already playing. Default value: `true`.  

#### play_one_shot

Starts a one shot instance of the event that will not be managed by the emitter. Useful for short SFX.

#### stop
Stops the event.

#### Set event methods

Both methods `set_event_name` and `set_event_guid` changes the event played by the emitter.  
Those methods stop and unload the current playing event. It also clear the current parameters values.  
If emitter option `preload_event` is true, it will load the new event.  
If emitter option `autoplay` is true, it will load and play the new event.

#### set_volume

This sets the emitter volume. If the emitter doe not have any loaded event yet, this will be applied to the future 
event.

### Programmers callbacks

To use a programmer callback, you must first load the bank with the
audio table used by your event containing the programmer callback.  
You can then use `set_programmer_callback` on the event to specify the
key from audio table to use.

```gdscript
var event_emitter = FmodEventEmitter2D.new()
event_emitter.event_guid = "{9aa2ecc5-ea4b-4ebe-85c3-054b11b21dcd}" # event:/Character/Dialogue from sfx bank.
event_emitter.autoplay = true
event_emitter.set_programmer_callback("welcome") # welcome key from audio table in Dialogue_EN.bank, Dialogue_JP.bank and Dialogue_CN.bank. One of those bank should be loaded.
add_child(event_emitter)
```

## FmodServer api

You can also use `FmodServer` api to play events. Here is an example script:  
```gdscript
extends Sprite2D

var isPlaying: bool = true
var event: FmodEvent = null

# Called when the node enters the scene tree for the first time.
func _ready():
	event = FmodServer.create_event_instance("event:/Vehicles/Car Engine")
	event.set_2d_attributes(self.global_transform)
	event.set_parameter_by_name("RPM", 600)
	event.volume = 2
	event.start()
	
# warning-ignore:unused_argument
func _process(_delta):
	if Input.is_action_just_pressed("space"):
		isPlaying = !isPlaying
		if(isPlaying):
			print("Mower playing")
			event.paused = false
		else:
			print("Mower paused")
			event.paused = true
	elif Input.is_action_just_pressed("kill_event"):
		self.queue_free()
```

In this script we create an instance of `FmodEvent` by calling `FmodServer`.  
We then set its position attribute to the node's transform, set its `RPM` parameter to `600` and its `volume` to `2`. 
We then start it.  
In the `_process` method, we pause the event if we press `space` action.  

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
	event.paused = true

# warning-ignore:unused_argument
func enter(_area):
	print("enter")
	event.paused = false
	
# warning-ignore:unused_argument
func leave(_area):
	print("leave")
	event.paused = true

# warning-ignore:unused_argument
func change_color(_dict: Dictionary, type: int):
	if type == FmodServer.FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT:
		$icon.self_modulate = Color(randf_range(0,1), randf_range(0,1), randf_range(0,1), 1)
```

In this script we create an `FmodEvent` and set a callback on it. Each time the event emits the callback we change the 
node's color.  

### Programmers callback

In order to play a programmer callback you must load the bank with audio table concerned by the programmer callback.  
then you can use `set_programmer_callback` to specify key from audio table to use.

```gdscript
FmodServer.load_bank("res://assets/Banks/Dialogue_EN.bank", FmodServer.FMOD_STUDIO_LOAD_BANK_NORMAL)
var event_instance = FmodServer.create_event_instance("event:/Character/Dialogue") # event from sfx bank.
event_instance.set_programmer_callback("welcome") # welcome key in audio table from Dialogue bank.
event_instance.start()
```

[emitter-image]: ./assets/emitter.png
