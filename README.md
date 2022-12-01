# FMOD Studio integration for Godot using GDNative

**A Godot C++ GDNative that provides an integration for the FMOD Studio API.**

![](https://img.shields.io/badge/Godot%20Compatible-3.1%2B-%234385B5) [![All builds](https://github.com/utopia-rise/fmod-gdnative/actions/workflows/all_builds.yml/badge.svg?branch=master)](https://github.com/utopia-rise/fmod-gdnative/actions/workflows/all_builds.yml) [![](https://img.shields.io/discord/1012326818365325352.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2)](https://discord.gg/u2NM2vTGMn)

---

FMOD is an audio engine and middleware solution for interactive audio in games. It has been the audio engine behind many
titles such as Transistor, Into the Breach and Celeste. [More on FMOD's website](https://www.fmod.com/).

This GDNative exposes most of the Studio API functions to Godot's GDScript and also provides helpers for performing
common functions like attaching Studio events to Godot nodes and playing 3D/positional audio. _It is still very much a
work in progress and some API functions are not yet exposed._ Feel free to tweak/extend it based on your project's needs.

**Note:** FMOD also provides a C# wrapper for their API which is used in the Unity integration and it is possible to use the
same wrapper to build an integration for Godot in C#. However do note that this would only work on a Mono build of Godot
as C# support is required and performance might not be on the same level as a C++ integration. 

**Note:** This project is a fork of [godot-fmod-integration](https://github.com/alexfonseka/godot-fmod-integration)
which uses godot module to integrate fmod in the engine. We thank [alexfonseka](https://github.com/alexfonseka) for the work he did.
This fork was designed to be able to use Fmod without building Godot Engine !

![wowmeme]

### Continuous delivery

This project uses github actions to continuously deploy released drivers. If you do not want to use those releases, you
can compile from sources by looking to [compile from sources section](./docs/COMPILING.md).  
This project uses [SEMVER](https://semver.org/).

#### OS Compatibility matrix :

| Driver Version | Windows | Linux | OSX | Android | iOS |
|----------------|---------|-------|-----|---------|-----|
|      0.0.0     |    X    |   X   |  X  |         |     |
|      1.0.0     |    X    |   X   |  X  |    X    |  X  |
|      2.0.0     |    X    |   X   |  X  |    X    |  X  |
|      2.0.1     |    X    |   X   |  X  |    X    |  X  |
|      2.0.2     |    X    |   X   |  X  |    X    |  X  |
|      3.x.x     |    X    |   X   |  X  |    X    |  X  |

#### Godot compatibility matrix

| Driver Version | Godot 3.0 | Godot 3.1 | Godot 3.2 | Godot 3.3 | Godot 3.4 |
|----------------|-----------|-----------|-----------|-----------|-----------|
|      0.0.0     |           |      X    |           |           |           |
|      1.0.0     |           |      X    |           |           |           |
|      2.0.0     |           |      X    |           |           |           |
|      2.0.1     |           |      X    |           |           |           |
|      2.0.2     |           |      X    |           |           |           |
|      3.x.x     |           |      X    |     X     |     X     |     X     |

#### Fmod compatibility matrix

| Driver Version | 1.10.13 | 2.00.XX | 2.0X.XX |
|----------------|---------|---------|---------|
|      0.0.0     |    X    |         |         |
|      1.0.0     |    X    |         |         |
|      2.0.0     |    X    |         |         |
|      2.0.1     |    X    |         |         |
|      2.0.2     |    X    |         |         |
|      3.0.0     |         |    X    |         |
|      3.x.x     |         |         |    X    |

## Installing the plugin in your project

### Install addon folder

We provide releases in github repository. You can download `fmod.zip` from release tabs, unzip it, and copy it to
`addons` folder of your project.  

It does not contains :
- Libraries from FMOD company.

It contains :
- built plugin
- gdnlib and gdns
- Fmod gdscript singleton
- test scripts.

### Add Fmod libraries to appropriate folder

[Download the FMOD Studio API](https://www.fmod.com/download) (You need to create an account), if you have not done it yet.
Then place fmod libraries (both `fmod` and `fmodstudio`) in the appropriate folder for each platform. A `CopyPast_Fmod_Libs_and_Gdnative_Here.txt` file
should be present where you need to copy libraries.  
Paths where you need to add shared libraries are:  
- `res://addons/fmod/libs/android/arm64_v8a/`
- `res://addons/fmod/libs/osx/`
- `res://addons/fmod/libs/windows/`
- `res://addons/fmod/libs/linux/`
- `res://addons/fmod/libs/iOS/`

You can also refer to `gdnlib` file in order to figure out what dependency is needed:

```
[dependencies]

Android.arm64-v8a=[ "res://addons/fmod/libs/android/arm64_v8a/libfmod.so", "res://addons/fmod/libs/android/arm64_v8a/libfmodstudio.so" ]
OSX.64=[ "res://addons/fmod/libs/osx/libfmod.dylib", "res://addons/fmod/libs/osx/libfmodstudio.dylib" ]
Windows.64=[ "res://addons/fmod/libs/windows/fmod.dll", "res://addons/fmod/libs/windows/fmodstudio.dll" ]
X11.64=[ "res://addons/fmod/libs/linux/libfmod.so", "res://addons/fmod/libs/linux/libfmodstudio.so" ]
iOS.arm64=[ "res://addons/fmod/libs/iOS/libfmodstudio_iphoneos.a", "res://addons/fmod/libs/iOS/libfmod_iphoneos.a", "res://addons/fmod/libs/iOS/libgodot-cpp.ios.release.arm64.a" ]
```

You should now be ready to go with Fmod and Godot !

NOTE: The Fmod singleton must be the first autoload otherwise issues can arise from calling the Fmod singleton from other autoloaded scripts. 

### Fmod on android with GDNative

Fmod require a specific .jar to run on Android + some additionnal setup lines in the godot java wrapper for Android.
Starting from Godot 3.2, a new custom android build system was introduced. You can use add simple files to your godot
project to add java code and libraries without recompiling the whole engine.  
All is explained in this tutorial this [tutorial](https://docs.godotengine.org/en/3.2/getting_started/workflow/export/android_custom_build.html#doc-android-custom-build).  

Here is how-to:  
To export android project, you need to copy `android` folder from [demo project](demo/).  
Also you will have to add `fmod.jar` from `core/lib` of downloaded android fmod library into `android/fmod/libs/`.
Then add the Fmod java singleton in `ProjectSettings ==> android`.  

![fmodsingleton]

And finally you have to setup the android export template.

![usecustombuild]

## Using the GDNative

### Basic usage

```gdscript
extends Node

func _ready():
	# register listener
	Fmod.add_listener(0, self)

	# play some events
	Fmod.play_one_shot("event:/Music/Level 02", self)
```

You can look at test scenes in POC folder of [example project](https://github.com/utopia-rise/fmod-gndative-godot-example-project) to find how to use the provided methods.

### Calling Studio events

Following is an example of an event instance called manually (ie. not directly managed by the integration). 
These instances are refered by an int id, returned when created. Remember to release the instance once you're done with it.

```gdscript
func _ready():	
	# register listener
	Fmod.add_listener(0, self)
	
	# play some events
	var my_music_event = Fmod.create_event_instance("event:/Music/Level 02")
	Fmod.start_event(my_music_event)
	var t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	Fmod.stop_event(my_music_event, Fmod.FMOD_STUDIO_STOP_ALLOWFADEOUT)
	t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	Fmod.release_event(my_music_event)
```

### Using the integration helpers

These are helper functions provided by the integration for attaching event instances to Godot Nodes for 3D/positional audio. The listener position and 3D attributes of any attached instances are automatically updated every time you call `system_update()`. Instances are also automatically cleaned up once finished so you don't have to manually call `event_release()`.

```gdscript
# play an event at this Node's position
# 3D attributes are only set ONCE
# parameters cannot be set
FMOD.play_one_shot("event:/Footstep", self)

# same as play_one_shot but lets you set initial parameters
# subsequent parameters cannot be set
FMOD.play_one_shot_with_params("event:/Footstep", self, { "Surface": 1.0, "Speed": 2.0 })

# play an event attached to this Node
# 3D attributes are automatically set every frame (when update is called)
# parameters cannot be set
FMOD.play_one_shot_attached("event:/Footstep", self)

# same as play_one_shot_attached but lets you set initial parameters
# subsequent parameters cannot be set
FMOD.play_one_shot_attached_with_params("event:/Footstep", self, { "Surface": 1.0, "Speed": 2.0 })

# attaches a manually called instance to a Node
# once attached 3D attributes are automatically set every frame (when update is called)
FMOD.attach_instance_to_node(instanceId, self)

# detaches the instance from its Node
FMOD.detach_instance_from_node(instanceId)

# blocks the calling thread until all sample loading is done
FMOD.wait_for_all_loads()
```

### Attach to existing event, 3D positioning

Here is an example where we attach event and listener to instances. In the [example project](https://github.com/utopia-rise/fmod-gndative-godot-example-project)
you have a scene `AttachToInstanceTest` where you can play with listener position, using mouse cursor.

```gdscript
func _ready():	
	# register listener
	Fmod.add_listener(0, $Listener)
	
	# Create event instance
	var my_music_event = Fmod.create_event_instance("event:/Weapons/Machine Gun")
	Fmod.start_event(my_music_event)
	
	# attach instance to node
	Fmod.attach_instance_to_node(my_music_event, $NodeToAttach)
	
	var t = Timer.new()
	t.set_wait_time(10)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	
	Fmod.detach_instance_from_node(my_music_event)
	Fmod.stop_event(my_music_event, Fmod.FMOD_STUDIO_STOP_IMMEDIATE)
```

### Timeline marker & music beat callbacks

You can have events subscribe to Studio callbacks to implement rhythm based game mechanics. Event callbacks leverage Godot's signal system and you can connect your callback functions through the integration.

```gdscript
# create a new event instance
var my_music_event = Fmod.create_event_instance("event:/schmid - 140 Part 2B")

# request callbacks from this instance
# in this case request both Marker and Beat callbacks
Fmod.event_set_callback(my_music_event,
	Fmod.FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER | Fmod.FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT)

# hook up our signals
Fmod.connect("timeline_beat", self, "_on_beat")
Fmod.connect("timeline_marker", self, "_on_marker")

# will be called on every musical beat
func _on_beat(params):
	print(params)

# will be called whenever a new marker is encountered
func _on_marker(params):
	print(params)
```

In the above example, `params` is a Dictionary which contains parameters passed in by FMOD. These vary from each callback. For beat callbacks it will contain fields such as the current beat, current bar, time signature etc. For marker callbacks it will contain the marker name etc. The event_id of the instance that triggered the callback will also be passed in. You can use this to filter out individual callbacks if multiple events are subscribed.

### Playing sounds using FMOD Core / Low Level API

You can load and play any sound file in your project directory by using the FMOD Low Level API bindings. Similar to Studio Events these instances are identified by a UUID generated in script. Any instances you create must be released manually. Refer to FMOD's documentation pages for a list of compatible sound formats. You can use Fmod.load_file_as_music(path) to stream the file and loop it or Fmod.load_file_as_sound(path) to load and play it at once.
Note that instances of file loaded as sound are automatically release by FMOD once played.

```gdscript
func _ready():
	# set up FMOD
	Fmod.add_listener(0, self)
	
	Fmod.load_file_as_music("res://assets/Music/jingles_SAX07.ogg")
	music = Fmod.create_sound_instance("res://assets/Music/jingles_SAX07.ogg")
	Fmod.play_sound(my_sound)
	
	var t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	
	Fmod.stop_sound(my_sound)
	Fmod.release_sound(music)
	Fmod.unload_file("res://assets/Music/jingles_SAX07.ogg")
```

### Muting all event

You can mute all event using `mute_all_events`. This will mute the master bus.

```gdscript
func _ready():
	# register listener
	Fmod.add_listener(0, self)
	
	# play some events
	Fmod.play_one_shot("event:/Music/Level 02", self)
	var my_music_event = Fmod.create_event_instance("event:/Music/Level 01")
	Fmod.start_event(my_music_event)
	var t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	Fmod.mute_all_events();
	t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	Fmod.unmute_all_events()
```

### Pausing all events

```gdscript
func _ready():
	# register listener
	Fmod.add_listener(0, self)
	
	# play some events
	Fmod.play_one_shot("event:/Music/Level 02", self)
	var my_music_event = Fmod.create_event_instance("event:/Music/Level 01")
	Fmod.start_event(my_music_event)
	var t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	Fmod.pause_all_events(true)
	t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	Fmod.pause_all_events(false)
```

### Changing the default audio output device

By default, FMOD will use the primary audio output device as determined by the operating system. This can be changed at runtime, ideally through your game's Options Menu.

Here, `get_available_drivers()` returns an Array which contains a Dictionary for every audio driver found. Each Dictionary contains fields such as the name, sample rate
and speaker config of the respective driver. Most importantly, it contains the id for that driver.

 ```gdscript
# retrieve all available audio drivers
var drivers = Fmod.get_available_drivers()
 # change the audio driver
# you must pass in the id of the respective driver
Fmod.set_driver(id)
 # retrieve the id of the currently set driver
var id = Fmod.get_driver()
```

### Reducing audio playback latency

You may encounter that the audio playback has some latency. This may be caused by the DSP buffer size. You can change the value **before** initialisation to adjust it:
```gdscript
Fmod.set_dsp_buffer_size(512, 4)
# retrieve the buffer length
Fmod.get_dsp_buffer_length()
# retrieve the number of buffers
Fmod.get_dsp_num_buffers()
```

### Profiling & querying performance data

`get_performance_data` returns an object which contains current performance stats for CPU, Memory and File Streaming usage of both FMOD Studio and the Core System.

```gdscript
# called every frame
var perf_data = FMOD.get_performance_data()

print(perf_data.CPU)
print(perf_data.memory)
print(perf_data.file)
```

## Contributing

In order to be able to PR this repo from a fork, you need to add `FMODUSER` and `FMODPASS` secrets to your fork repo.  
This enables CI to download FMOD api.

## Thanks

This project is a forked from [godot-fmod-integration](https://github.com/alexfonseka/godot-fmod-integration)
from [alexfonseka](https://github.com/alexfonseka). We'd like to thank him for the work he did, we simply adapted his
work to GDNative.  
Feel free to propose any modification using github's *pull request*. We hope you'll enjoy this driver.


[wowmeme]: .README/wowmeme.png
[fmodsingleton]: .README/fmodsingleton.png
[usecustombuild]: .README/usecustombuild.png
