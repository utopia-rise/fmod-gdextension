# FMOD Studio integration for Godot using GDNative

A Godot C++ GDNative that provides an integration for the FMOD Studio API.

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

## Typical Project structure

```
└── Project root
    ├── libs
    |   └── fmod
    |       └── {platform}
    |           └── specific platform fmod api goes here
    ├── godot-cpp (gdnative bindings, consider using a git submodule)
    └── fmod-gdnative (this repo, consider using it as a submodule of you GDNatives repo)
        ├── CMakeLists.txt (Here for CLion)
        ├── LICENSE
        ├── README.md
        ├── SConstruct (here to build on Windows, Linux, OSX and IOS
        └── src
```

If you look at [Sconstruct](SConstruct) script, you'll see that it refers to libraries that are in path relatives to
parent folder. This is because Utopia-Rise team chose to make this repo integrated to a unique repository, storing all
the GDNative for our project.

So, you are supposed to put fmod libraries under `libs/fmod/{platform}`, according to the platforms you want to support.

Otherwise, you can use [our example project](https://github.com/utopia-rise/GDNative-example-repo), which already contains the appropriate structure.

Feel free to modify SConstruct according to your project structure.

`CMakeLists` is here for CLion ide, as we are used to JetBrains tools. Unfortunately, CLion does not currently support
Sconstruct.

## Building the GDNative

Current CI status : ![CI status](https://travis-ci.com/utopia-rise/fmod-gdnative.svg?branch=master)

### Continuous delivery

This project uses Travis-CI to continuously deploy released drivers. If you use those releases, you can skip building
api and driver. Installing is still necessary.  
This project uses [SEMVER](https://semver.org/).

#### OS Compatibility matrix :

| Driver Version | Windows | Linux | OSX | Android | iOS |
|----------------|---------|-------|-----|---------|-----|
|      0.0.0     |    X    |   X   |  X  |         |     |
|      1.0.0     |    X    |   X   |  X  |    X    |  X  |
|      2.0.0     |    X    |   X   |  X  |    X    |  X  |
|      2.0.1     |    X    |   X   |  X  |    X    |  X  |
|      2.0.2     |    X    |   X   |  X  |    X    |  X  |
|      3.0.0     |    X    |   X   |  X  |    X    |  X  |

#### Godot compatibility matrix

| Driver Version | Godot 3.0 | Godot 3.1 | Godot 3.2 |
|----------------|-----------|-----------|-----------|
|      0.0.0     |           |      X    |           |
|      1.0.0     |           |      X    |           |
|      2.0.0     |           |      X    |           |
|      2.0.1     |           |      X    |           |
|      2.0.2     |           |      X    |           |
|      3.0.0     |           |      X    |     X     |

#### Fmod compatibility matrix

| Driver Version | 1.10.13 | 2.00.XX |
|----------------|---------|---------|
|      0.0.0     |    X    |         |
|      1.0.0     |    X    |         |
|      2.0.0     |    X    |         |
|      2.0.1     |    X    |         |
|      2.0.2     |    X    |         |
|      3.0.0     |         |    X    |

### Building GDNative API bindings

To Build GDNative bindings you can follow [this tutorial from godot official documentation](https://docs.godotengine.org/en/3.1/tutorials/plugins/gdnative/gdnative-cpp-example.html#building-the-c-bindings).
If you want to regenerate api you can add the following argument to your building command :
```
godotbinpath="path to your godot binary"
```
When you're done with that part you should have `libgodot-cpp.<platform>.<target>.<bits|android_arch|ios_arch>.<a|lib>` in godot-cpp/bin` folder.

#### iOS

To build bindings for iOS, execute:
```
scons platform=ios generate_bindings=True ios_arch=armv7/arm64 target=release
```

#### Android

To build bindings for android, we provide our [godot-cpp](https://github.com/utopia-rise/godot-cpp) version. 
Checkout `utopia-3.2` branch. This is also provided with our [GDNative example project](https://github.com/utopia-rise/GDNative-example-repo).  
First, you should set `ANDROID_NDK_ROOT` environment variable by typing :
```
export ANDROID_NDK_ROOT="pathToYourAndroidNDK"
```  
To build on Android, you should type :
```
scons platform=android generate_bindings=True android_arch=armv7/arm64v8 target=release
```

### Building the GDNative driver

[Download the FMOD Studio API](https://www.fmod.com/download) (You need to create an account) and place it in the
appropriate platform folder into lib folder (see project structure).

For each platforms, if your project structure is different from the one proposed here, you can overload `cpp_bindings_dir` 
and `headers_dir` parameters.

To load fmod dynamic libraries on app or engine loading, fmod GDNative will look in subfolder `libs` by default, as in OSX
part. you can overload this relative path adding this parameter to the command `fmod_lib_dir="path to fmod dll"`. 

#### OSX

To build the GDNative for OSX, you should use this command in `fmod-gdnative` folder :

```
scons platform=osx target=release bits=64
```

This will generate `libGodotFmod.osx.release.64.dylib` in `fmod-gdnative/bin` folder. 


#### Linux

To build the GDNative for Linux, you should use this command in `fmod-gdnative` folder :
```
scons platform=linux use_llvm=yes target=release bits=64
```
This will generate a `libGodotFmod.linux.release.64.so` in `fmod-gdnative/bin` folder.

#### Windows

To build the GDNative for Windows, you should use this command in `fmod-gdnative` folder :
```
scons platform=windows target=release bits=64
```
This will generate `libGodotFmod.windows.release.64.dll` in `fmod-gdnative/bin` folder.

#### Android

To build the GDNative for Android, we currently use NDKBuild. So you should use this command in `fmod-gdnative` folder :
```
$ANDROID_NDK_ROOT/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk  APP_PLATFORM=android-21
```
This will generate `libGodotFmod.android.<target>.<android_arch>.so` for each supported architectures in `libs` folder.

#### iOS

To build the GDNative for iOS, you should use this command in `fmod-gdnative` folder :
```
scons platform=ios target=release ios-arch=armv7/arm64
```
This will generate `libGodotFmod.ios.release.arm64.a` in `fmod-gdnative/bin` folder.

Those libraries are statics, so you need to add fmod static librairies and godot-cpp static library for ios in your
project, with `libGodotFmod.ios.release.arm64.a`.

## Installing the GDNative in your project

### Example project

We provide a [demo project](demo/) in demo subfolder to help you to understand how to link the driver to your game project, and how to use it. 

It does not contains :
- built driver.

It contains : 
- gdnlib and gdns
- Fmod gdscript singleton
- test scripts.

### Copy addon folder

Copy `addons/fmod` folder from [demo project](demo/) into your addons folder.

### Copy built GDNative

You have to copy the built GDNative driver in the correspoinding folder:  
- windows: `res://addons/fmod/libs/windows/`
- linux: `res://addons/fmod/libs/linux/`
- osx: `res://addons/fmod/libs/osx/`
- ios: `res://addons/fmod/libs/iOS/libGodotFmod.ios.release.arm64.a`
- android: copy arm folders this way `res://addons/fmod/libs/android/arm64_v8a/`

### Add Fmod libraries to appropriate folder

[Download the FMOD Studio API](https://www.fmod.com/download) (You need to create an account), if you have not done it yet.
Then place fmod libraries in the appropriate folder, using `gdnlib` to know where:

```
[dependencies]

Android.arm64-v8a=[ "res://addons/fmod/libs/android/arm64_v8a/libfmod.so", "res://addons/fmod/libs/android/arm64_v8a/libfmodstudio.so" ]
OSX.64=[ "res://addons/fmod/libs/osx/libfmod.dylib", "res://addons/fmod/libs/osx/libfmodstudio.dylib" ]
Windows.64=[ "res://addons/fmod/libs/windows/fmod.dll", "res://addons/fmod/libs/windows/fmodstudio.dll" ]
X11.64=[ "res://addons/fmod/libs/linux/libfmod.so", "res://addons/fmod/libs/linux/libfmodstudio.so" ]
iOS.arm64=[ "res://addons/fmod/libs/iOS/libfmodstudio_iphoneos.a", "res://addons/fmod/libs/iOS/libfmod_iphoneos.a", "res://addons/fmod/libs/iOS/libgodot-cpp.ios.release.arm64.a" ]
```

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
	# set up FMOD
	Fmod.set_software_format(0, Fmod.FMOD_SPEAKERMODE_STEREO, 0)
	Fmod.init(1024, Fmod.FMOD_STUDIO_INIT_LIVEUPDATE, Fmod.FMOD_INIT_NORMAL)
	
	# load banks
	Fmod.load_bank("res://Master Bank.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.load_bank("res://Master Bank.strings.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.load_bank("res://Music.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)

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
	# set up FMOD
	Fmod.set_software_format(0, Fmod.FMOD_SPEAKERMODE_STEREO, 0)
	Fmod.init(1024, Fmod.FMOD_STUDIO_INIT_LIVEUPDATE, Fmod.FMOD_INIT_NORMAL)
	Fmod.set_sound_3D_settings(1.0, 64.0, 1.0)
	
	# load banks
	Fmod.load_bank("res://Master Bank.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.load_bank("res://Master Bank.strings.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.load_bank("res://Music.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	
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
	Fmod.set_software_format(0, Fmod.FMOD_SPEAKERMODE_STEREO, 0)
	Fmod.init(1024, Fmod.FMOD_STUDIO_INIT_LIVEUPDATE, Fmod.FMOD_INIT_NORMAL)
	Fmod.setSound3DSettings(1.0, 64.0, 1.0)
	
	# load banks
	Fmod.load_bank("res://Master Bank.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.load_bank("res://Master Bank.strings.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.load_bank("res://Music.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	
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
	Fmod.stop_event(my_music_event, Fmods.FMOD_STUDIO_STOP_IMMEDIATE)
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
	Fmod.set_software_format(0, Fmod.FMOD_SPEAKERMODE_STEREO, 0)
	Fmod.init(1024, Fmod.FMOD_STUDIO_INIT_LIVEUPDATE, Fmod.FMOD_INIT_NORMAL)
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

You can mute all event using `muteAllEvents`. This will mute the master bus.

```gdscript
func _ready():
	Fmod.set_software_format(0, Fmod.FMOD_SPEAKERMODE_STEREO, 0)
	Fmod.init(1024, Fmod.FMOD_STUDIO_INIT_LIVEUPDATE, Fmod.FMOD_INIT_NORMAL)
	Fmod.setSound3DSettings(1.0, 64.0, 1.0)
	
	# load banks
	Fmod.load_bank("res://Master Bank.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.load_bank("res://Master Bank.strings.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.load_bank("res://Music.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	
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
	# set up FMOD
	Fmod.set_software_format(0, Fmod.FMOD_SPEAKERMODE_STEREO, 0)
	Fmod.init(1024, Fmod.FMOD_STUDIO_INIT_LIVEUPDATE, Fmod.FMOD_INIT_NORMAL)
	Fmod.setSound3DSettings(1/0, 64.0, 1.0)
	
	# load banks
	Fmod.load_bank("res://Master Bank.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.load_bank("res://Master Bank.strings.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.load_bank("res://Music.bank", Fmod.FMOD_STUDIO_LOAD_BANK_NORMAL)
	
	# register listener
	Fmod.add_listener(0, self)
	
	# play some events
	Fmod.play_one_shot("event:/Music/Level 02", self)
	var my_music_event = Fmod.create_event_instance("event:/Music/Level 01")
	Fmod.startEvent(my_music_event)
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

Here, `getAvailableDrivers()` returns an Array which contains a Dictionary for every audio driver found. Each Dictionary contains fields such as the name, sample rate
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

### Profiling & querying performance data

`getPerformanceData` returns an object which contains current performance stats for CPU, Memory and File Streaming usage of both FMOD Studio and the Core System.

```gdscript
# called every frame
var perf_data = FMOD.get_performance_data()

print(perf_data.CPU)
print(perf_data.memory)
print(perf_data.file)
```

## Thanks

This project is a forked from [godot-fmod-integration](https://github.com/alexfonseka/godot-fmod-integration)
from [alexfonseka](https://github.com/alexfonseka). We'd like to thank him for the work he did, we simply adapted his
work to GDNative.  
Feel free to propose any modification using github's *pull request*. We hope you'll enjoy this driver.


[wowmeme]: .README/wowmeme.png
[fmodsingleton]: .README/fmodsingleton.png
[usecustombuild]: .README/usecustombuild.png
