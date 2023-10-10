# High level nodes

We provide nodes to easily create your game using fmod.  
Those nodes are gdextension, so you can extends them using scripts like any other godot node.

## Fmod Bank loader

`FmodBankLoader` is in charge of loading banks when entering the scene. You should place it, in the scene hierarchy,
before all other fmod nodes using this bank. Banks are unloaded on exit tree.  
If you want to load your bank when starting game and keep them loaded, use this node and add it as autoload node.  
You can add banks with fmod project explorer, using the `+` button with bank icon, or manually add a bank using bottom
line edit. You can also remove and re-order banks:  
![fmod-bank-image]

!!! warning
    You should place master banks first.

## Event emitters

`FmodEventEmitter2D` and `FmodEventEmitter3D` are nodes to play Fmod events and their position will be updated
automatically.

![emitter-image]

### Properties

You first should set event that emitter will play. You can set name or guid manually or using event selection button.  
You can setup parameters using a `Dictionary` with keys representing parameter name as `String` and values as `Float`.
Then you have few toogle options:  
- *load_by_event_name*: if `true` will use event_name to load event, otherwise it uses GUID.  
- *attached*: if `true`, Fmod event position will be updated alongside node's position.  
- *autoplay*: if `true`, it will autoplay event, without need to call `play` method.  
- *one_shot*: if `true`, event will be played only once.  
- *allow_fadeout*: if `true`, event will be stop with fadeout mode.  
- *preload_event*: if `true`, event will be preloaded when node is ready.  

#### Fmod parameters

Event emitters have dynamics properties corresponding to fmod parameters associated with the current event.  
You can set their values like any other godot float property.  
By default those are loaded using their FMOD id. You can load them using their names by unchecking `Should Load by ID`.  
From scripts, you can change them using `get` and `set` object's operators.  
Example:
```gdscript
extends FmodEventEmitter2D

func _process(_delta):
	if Input.is_action_pressed("engine_power_up"):
		self["event_parameter/RPM/value"] = self["event_parameter/RPM/value"] + 10
	if Input.is_action_pressed("engine_power_down"):
		self["event_parameter/RPM/value"] = self["event_parameter/RPM/value"] - 10
```  
To easily retrieve fmod parameters properties path, you can use godot's `Copy Property Path` functionality.

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

#### stop
Stops the event.

## Fmod listeners

`FmodListener2D` and `FmodListener3D` are nodes to add a Fmod listener and bind it to them.
![listener-image]

### Properties:

#### listener_index
Index of the listener. You can have up to 8 listener concurrently.
#### is_locked
If `true`, listener will not update its position according to node's position.
#### weight
Used to compute the relative contribution to the final sound.


[fmod-bank-image]: ./assets/fmod-bank.png
[emitter-image]: ./assets/emitter.png
[listener-image]: ./assets/listeners.png