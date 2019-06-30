# FMOD Studio integration for Godot using GDNative (WIP)

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
        ├── godot_fmod.h
        ├── godot_fmod.cpp
        └── gdlibrary.cpp
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

| Driver Version | Godot 3.0 | Godot 3.1-stable | Godot 3.1.1-stable |
|----------------|-----------|------------------|--------------------|
|      0.0.0     |           |         X        |                    |
|      1.0.0     |           |         X        |                    |
|      2.0.0     |           |         X        |          X         |
|      2.0.1     |           |         X        |          X         |
|      2.0.2     |           |         X        |          X         |
|      3.0.0     |           |         X        |          X         |

#### Fmod compatibility matrix

| Driver Version | 1.10.13 | 2.00.00 | 2.00.01 | 2.00.02 |
|----------------|---------|---------|---------|---------|
|      0.0.0     |    X    |         |         |         |
|      1.0.0     |    X    |         |         |         |
|      2.0.0     |    X    |         |         |         |
|      2.0.1     |    X    |         |         |         |
|      2.0.2     |    X    |         |         |         |
|      3.0.0     |         |    X    |    X    |    X    |

### Building GDNative API bindings

To Build GDNative bindings you can follow [this tutorial from godot official documentation](https://docs.godotengine.org/en/3.1/tutorials/plugins/gdnative/gdnative-cpp-example.html#building-the-c-bindings).
If you want to regenerate bindings you can add the following argument to your building command :
```
godotbinpath="path to your godot binary"
```
For the moment we are focusing on release target. So you may need to add :
```
target=release
```
When you're done with that part you should have `libgodot-cpp.<platform>.<target>.<bits>.<a|lib>` in godot-cpp/bin` folder.

#### iOS

To build bindings for iOS, we provide our [godot-cpp](https://github.com/utopia-rise/godot-cpp) version, with Android
and iOS build export. Checkout `3.1-utopia` branch. This is also provided with our [GDNative example project](https://github.com/utopia-rise/GDNative-example-repo).  
To build on iOS, you should type :
```
scons platform=ios generate_bindings=True bits=64 target=release
```

#### Android

To build bindings for iOS, we provide our [godot-cpp](https://github.com/utopia-rise/godot-cpp) version, with Android
and iOS build export. Checkout `3.1-utopia` branch. This is also provided with our [GDNative example project](https://github.com/utopia-rise/GDNative-example-repo).  
First, you should set `ANDROID_NDK_ROOT` environment variable by typing :
```
export ANDROID_NDK_ROOT="pathToYourAndroidNDK"
```  
To build on Android, you should type :
```
scons platform=android generate_bindings=True bits=64 target=release android-abi=arm/arm64
```

### Building the GDNative driver

[Download the FMOD Studio API](https://www.fmod.com/download) (You need to create an account) and place it in the
appropriate platform folder into lib folder (see project structure).

For each platforms, if your project structure is different from the one proposed here, you can overload `cpp_bindings` 
and `headers` parameters.

#### OSX

To build the GDNative for OSX, you should use this command in `fmod-gdnative` folder :

```
scons p=osx target=release
```

This will generate `libGodotFmod.osx.dylib` in `fmod-gdnative/bin` folder. 

Be aware that osx is a bit tricky with dynamic library loading. [This link](https://blogs.oracle.com/dipol/dynamic-libraries,-rpath,-and-mac-os) 
explains how to load dynamic dependencies on OSX. By default the GDNative will look for fmod libraries in a `libs` folder
relative to its path when loading the game or the engine. Otherwise, if you want to change that loading path, you can add this parameter `fmod-lib-install-path="path to fmod dll"`.

#### Linux

To build the GDNative for Linux, you should use this command in `fmod-gdnative` folder :
```
scons p=linux use_llvm=yes target=release bits=64
```
This will generate a `libGodotFmod.linux.so` in `fmod-gdnative/bin` folder.

To load fmod dynamic libraries on app or engine loading, fmod GDNative will look in subfolder `libs` by default, as in OSX
part. you can overload this relative path adding this parameter to the command `fmod-lib-install-path="path to fmod dll"`. 

#### Windows

To build the GDNative for Windows, you should use this command in `fmod-gdnative` folder :
```
scons p=windows target=release bits=64
```
This will generate `libGodotFmod.windows.dll` in `fmod-gdnative/bin` folder.

To load fmod dynamic libraries on app or engine loading, the simplest way is to put fmod dynamic dependencies in the 
same folder as GDNative dll. 

#### Android

To build the GDNative for Android, we currently use NDKBuild. So you should use this command in `fmod-gdnative` folder :
```
$ANDROID_NDK_ROOT/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk  APP_PLATFORM=android-21
```
This will generate `libandroid_fmod_gdnative.so` for each supported architectures in `libs` folder.

To load fmod dynamic libraries on app or engine loading, the simplest way is to put fmod dynamic dependencies in the 
same folder as GDNative dll, with `libc++_shared.so`.

#### iOS

To build the GDNative for iOS, you should use this command in `fmod-gdnative` folder :
```
p=ios target=release dynamic=no
```
This will generate `libGodotFmod.ios.a` in `fmod-gdnative/bin` folder.

Those libraries are statics, so you need to add fmod static librairies and godot-cpp static library for ios in your
project, with `libGodotFmod.ios.a`.

## Installing the GDNative in your project

In order to understand how to link the driver you built to your game, you can read the [official documentation](https://docs.godotengine.org/en/3.1/tutorials/plugins/gdnative/gdnative-cpp-example.html#using-the-gdnative-module).

### Example project

We provide an [example project](https://github.com/utopia-rise/fmod-gndative-godot-example-project) to help you to understand how to link the driver to your game project, and how to use it. 

It does not contains :
- built driver.

It contains : 
- gdnlib and gdns
- Fmod gdscript singleton
- test scripts.

### Copy built GDNative

You first should copy the built GDNative into your project, for each supported platform.
You can look at [example project](https://github.com/utopia-rise/fmod-gndative-godot-example-project) to get inspired of you don't know how to process.

### Create the gdnlib

In order to create the gdnlib you should go on [the following part of Godot engine documentation](
https://docs.godotengine.org/en/3.1/tutorials/plugins/gdnative/gdnative-cpp-example.html#using-the-gdnative-module).  
You can look at the [example project](https://github.com/utopia-rise/fmod-gndative-godot-example-project) gdnlib in lib folder.  
It tells to godot where to look for dependencies of each platform.

#### iOS specificity

In order to tell godot where to look for static dependencies of the driver, you need to add them in gdnlib that way :
```
[dependencies]
...
iOS.armv7=[ "res://lib/iOS/libfmodstudio_iphoneos.a", "res://lib/iOS/libfmod_iphoneos.a", "res://lib/iOS/libgodot-cpp.ios.release.a" ]
iOS.arm64=[ "res://lib/iOS/libfmodstudio_iphoneos.a", "res://lib/iOS/libfmod_iphoneos.a", "res://lib/iOS/libgodot-cpp.ios.release.a" ]
```

#### Android specificity

In order to tell godot where to look for dependencies of the driver, you need to add them in gdnlib that way :
```
[dependencies]

Android.armeabi-v7a=[ "res://lib/android/armeabi-v7a/libfmod.so", "res://lib/android/armeabi-v7a/libfmodstudio.so" ]
Android.arm64-v8a=[ "res://lib/android/arm64-v8a/libfmod.so", "res://lib/android/arm64-v8a/libfmodstudio.so" ]
```

### Create the gdns

To create the gdns, you still should go on [the same part of documentation](https://docs.godotengine.org/en/3.1/tutorials/plugins/gdnative/gdnative-cpp-example.html#using-the-gdnative-module).
You still can look at the [example project](https://github.com/utopia-rise/fmod-gndative-godot-example-project), the gdns is located in main/backend/sound/  
It is used by the scripts as entry point to get gdnlib.

### Create the Fmod singleton

We recommand to wrap an instance of the GDNative into a singleton script. You also can set the GDNative directly as a singleton, but you won't be able to know what methods you can access.  
You can copy the [Fmod singleton script](https://github.com/utopia-rise/fmod-gndative-godot-example-project/blob/master/main/backend/sound/Fmod.gd) and modify gdns loading according to your project structure.

This script contains :
- fmod binding methods.
- process loop implementation to call Fmod update.

You should set this script as auto-loaded in editor.

If you choose the GDNative as singleton method, you also should provide yourself the process loop method implementation.

### Fmod on android with GDNative

Fmod is a bit tricky to use on android with gdnative. It requires to modify the android export template to load `Fmod.jar`
and load .so using JNI.  
You can take example on our [android export template](https://github.com/utopia-rise/godot-export-android-fmod) or take
the release as is. But if you have you own export template here is what you should do :

- Add fmod.jar as dependency in your project (here we will say it is in libs folder, as [here](https://github.com/utopia-rise/godot-export-android-fmod/tree/master/libs)).
In order to add fmod to gradle you should have dependencies looking like this :  
```
dependencies {
	implementation "com.android.support:support-core-utils:28.0.0"
	compile files("libs/fmod.jar")
}
```
- Modify `onCreate` and `onDestroy` methods in `Godot` java class  

For `onCreate` you should initialize java part of fmod. You should have something like this :  
```java
	@Override
	protected void onCreate(Bundle icicle) {

		super.onCreate(icicle);
		FMOD.init(this);
		Window window = getWindow();
		...
	}
```

For `onDestroy` method, you should close java part of Fmod. It should looks like this :
```java
	@Override
	protected void onDestroy() {

		if (mPaymentsManager != null) mPaymentsManager.destroy();
		for (int i = 0; i < singleton_count; i++) {
			singletons[i].onMainDestroy();
		}
		FMOD.close();
		super.onDestroy();
	}
```

- Modify `GodotLib` java class to load .so using jni.

```java
public class GodotLib {

	public static GodotIO io;

	static {
		System.loadLibrary("fmod");
		System.loadLibrary("fmodstudio");
		System.loadLibrary("godot_android");
	}
	...
}
```
You should not add fmod .so to export template, those lines will use GDNative driver dependencies.

## Using the GDNative

### Basic usage

```gdscript
extends Node

func _ready():
	# set up FMOD
	Fmod.setSoftwareFormat(0, FmodFlags.FMOD_SPEAKER_MODE_FLAGS.FMOD_SPEAKERMODE_STEREO, 0)
	Fmod.init(1024, FmodFlags.FMOD_STUDIO_INIT_FLAGS.FMOD_STUDIO_INIT_LIVEUPDATE, FmodFlags.FMOD_INIT_FLAGS.FMOD_INIT_NORMAL)
	
	# load banks
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/Music.bank", FmodFlags.FMOD_LOAD_BANK_FLAGS.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/Master Bank.bank", FmodFlags.FMOD_LOAD_BANK_FLAGS.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/Master Bank.strings.bank", FmodFlags.FMOD_LOAD_BANK_FLAGS.FMOD_STUDIO_LOAD_BANK_NORMAL)

	# register listener
	Fmod.addListener(self)

	# play some events
	Fmod.playOneShot("event:/Music/Level 02", self)
```

You can look at test scenes in POC folder of [example project](https://github.com/utopia-rise/fmod-gndative-godot-example-project) to find how to use the provided methods.

### Calling Studio events

Following is an example of an event instance called manually (ie. not directly managed by the integration). 
These instances are refered by an int id, returned when created. Remember to release the instance once you're done with it.

```gdscript
func _ready():
	# set up FMOD
	Fmod.setSoftwareFormat(0, FmodFlags.FMOD_SPEAKER_MODE_FLAGS.FMOD_SPEAKERMODE_STEREO, 0)
	Fmod.init(1024, FmodFlags.FMOD_STUDIO_INIT_FLAGS.FMOD_STUDIO_INIT_LIVEUPDATE, FmodFlags.FMOD_INIT_FLAGS.FMOD_INIT_NORMAL)
	Fmod.setSound3DSettings(1.0, 64.0, 1.0)
	
	# load banks
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/Music.bank", FmodFlags.FMOD_LOAD_BANK_FLAGS.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/Master Bank.bank", FmodFlags.FMOD_LOAD_BANK_FLAGS.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/Master Bank.strings.bank", FmodFlags.FMOD_LOAD_BANK_FLAGS.FMOD_STUDIO_LOAD_BANK_NORMAL)
	
	# register listener
	Fmod.addListener(self)
	
	# play some events
	var my_music_event = Fmod.createEventInstance("event:/Music/Level 02")
	Fmod.startEvent(my_music_event)
	var t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	Fmod.stopEvent(my_music_event, FmodFlags.FMOD_STUDIO_STOP_MODES.FMOD_STUDIO_STOP_ALLOWFADEOUT)
	t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	Fmod.releaseEvent(my_music_event)
```

### Using the integration helpers

These are helper functions provided by the integration for attaching event instances to Godot Nodes for 3D/positional audio. The listener position and 3D attributes of any attached instances are automatically updated every time you call `system_update()`. Instances are also automatically cleaned up once finished so you don't have to manually call `event_release()`.

```gdscript
# play an event at this Node's position
# 3D attributes are only set ONCE
# parameters cannot be set
FMOD.playOneShot("event:/Footstep", self)

# same as play_one_shot but lets you set initial parameters
# subsequent parameters cannot be set
FMOD.playOneShotWithParams("event:/Footstep", self, { "Surface": 1.0, "Speed": 2.0 })

# play an event attached to this Node
# 3D attributes are automatically set every frame (when update is called)
# parameters cannot be set
FMOD.playOneShotAttached("event:/Footstep", self)

# same as play_one_shot_attached but lets you set initial parameters
# subsequent parameters cannot be set
FMOD.playOneShotAttachedWithParams("event:/Footstep", self, { "Surface": 1.0, "Speed": 2.0 })

# attaches a manually called instance to a Node
# once attached 3D attributes are automatically set every frame (when update is called)
FMOD.attachInstanceToNode(instanceId, self)

# detaches the instance from its Node
FMOD.detachInstanceFromNode(instanceId)

# blocks the calling thread until all sample loading is done
FMOD.waitForAllLoads()
```

### Attach to existing event, 3D positioning

Here is an example where we attach event and listener to instances. In the [example project](https://github.com/utopia-rise/fmod-gndative-godot-example-project)
you have a scene `AttachToInstanceTest` where you can play with listener position, using mouse cursor.

```gdscript
func _ready():
	# set up FMOD
	Fmod.setSoftwareFormat(0, FmodFlags.FmodSpeakerModeFlags.FMOD_SPEAKERMODE_STEREO, 0)
	Fmod.init(1024, FmodFlags.FmodStudioInitFlags.FMOD_STUDIO_INIT_LIVEUPDATE, FmodFlags.FmodInitFlags.FMOD_INIT_NORMAL)
	Fmod.setSound3DSettings(1.0, 64.0, 1.0)
	
	# load banks
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/SFX.bank", FmodFlags.FmodLoadBankFlags.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/Master Bank.bank", FmodFlags.FmodLoadBankFlags.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/Master Bank.strings.bank", FmodFlags.FmodLoadBankFlags.FMOD_STUDIO_LOAD_BANK_NORMAL)
	
	# register listener
	Fmod.addListener($Listener)
	
	# Create event instance
	var my_music_event = Fmod.createEventInstance("event:/Weapons/Machine Gun")
	
	Fmod.startEvent(my_music_event)
	
	# attach instance to node
	Fmod.attachInstanceToNode(my_music_event, $NodeToAttach)
	
	var t = Timer.new()
	t.set_wait_time(10)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	
	Fmod.detachInstanceFromNode(my_music_event)
	Fmod.stopEvent(my_music_event, FmodFlags.fmodStudioStopModes.FMOD_STUDIO_STOP_IMMEDIATE)
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

You can load and play any sound file in your project directory by using the FMOD Low Level API bindings. Similar to Studio Events these instances are identified by a UUID generated in script. Any instances you create must be released manually. Refer to FMOD's documentation pages for a list of compatible sound formats.

```gdscript
func _ready():
	# set up FMOD
	Fmod.setSoftwareFormat(0, FmodFlags.FMOD_SPEAKER_MODE_FLAGS.FMOD_SPEAKERMODE_STEREO, 0)
	Fmod.init(1024, FmodFlags.FMOD_STUDIO_INIT_FLAGS.FMOD_STUDIO_INIT_LIVEUPDATE, FmodFlags.FMOD_INIT_FLAGS.FMOD_INIT_NORMAL)
	Fmod.setSound3DSettings(1.0, 64.0, 1.0)
	Fmod.addListener(self)
	var my_sound = Fmod.loadSound("./main/sound/20-Title_Gym.wav", FmodFlags.FMOD_SOUND_CONSTANTS.FMOD_DEFAULT)
	Fmod.playSound(my_sound)
	var t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	Fmod.stopSound(my_sound)
	Fmod.releaseSound(my_sound)
```

### Muting all event

You can mute all event using `muteAllEvents`. This will mute the master bus.

```gdscript
func _ready():
	# set up FMOD
	Fmod.setSoftwareFormat(0, FmodFlags.FMOD_SPEAKER_MODE_FLAGS.FMOD_SPEAKERMODE_STEREO, 0)
	Fmod.init(1024, FmodFlags.FMOD_STUDIO_INIT_FLAGS.FMOD_STUDIO_INIT_LIVEUPDATE, FmodFlags.FMOD_INIT_FLAGS.FMOD_INIT_NORMAL)
	Fmod.setSound3DSettings(1.0, 64.0, 1.0)
	
	# load banks
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/Music.bank", FmodFlags.FMOD_LOAD_BANK_FLAGS.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/Master Bank.bank", FmodFlags.FMOD_LOAD_BANK_FLAGS.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/Master Bank.strings.bank", FmodFlags.FMOD_LOAD_BANK_FLAGS.FMOD_STUDIO_LOAD_BANK_NORMAL)
	
	# register listener
	Fmod.addListener(self)
	
	# play some events
	Fmod.playOneShot("event:/Music/Level 02", self)
	var my_music_event = Fmod.createEventInstance("event:/Music/Level 01")
	Fmod.startEvent(my_music_event)
	var t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	Fmod.muteAllEvents();
	t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	Fmod.unmuteAllEvents()
```

### Pausing all events

```gdscript
func _ready():
	# set up FMOD
	Fmod.setSoftwareFormat(0, FmodFlags.FMOD_SPEAKER_MODE_FLAGS.FMOD_SPEAKERMODE_STEREO, 0)
	Fmod.init(1024, FmodFlags.FMOD_STUDIO_INIT_FLAGS.FMOD_STUDIO_INIT_LIVEUPDATE, FmodFlags.FMOD_INIT_FLAGS.FMOD_INIT_NORMAL)
	Fmod.setSound3DSettings(1.0, 64.0, 1.0)
	
	# load banks
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/Music.bank", FmodFlags.FMOD_LOAD_BANK_FLAGS.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/Master Bank.bank", FmodFlags.FMOD_LOAD_BANK_FLAGS.FMOD_STUDIO_LOAD_BANK_NORMAL)
	Fmod.loadbank("./POC/backend/sound/Banks/Desktop/Master Bank.strings.bank", FmodFlags.FMOD_LOAD_BANK_FLAGS.FMOD_STUDIO_LOAD_BANK_NORMAL)
	
	# register listener
	Fmod.addListener(self)
	
	# play some events
	Fmod.playOneShot("event:/Music/Level 02", self)
	var my_music_event = Fmod.createEventInstance("event:/Music/Level 01")
	Fmod.startEvent(my_music_event)
	var t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	Fmod.pauseAllEvents(true)
	t = Timer.new()
	t.set_wait_time(3)
	t.set_one_shot(true)
	self.add_child(t)
	t.start()
	yield(t, "timeout")
	Fmod.pauseAllEvents(false)
```

### Changing the default audio output device

By default, FMOD will use the primary audio output device as determined by the operating system. This can be changed at runtime, ideally through your game's Options Menu.

Here, `getAvailableDrivers()` returns an Array which contains a Dictionary for every audio driver found. Each Dictionary contains fields such as the name, sample rate
and speaker config of the respective driver. Most importantly, it contains the id for that driver.

 ```gdscript
# retrieve all available audio drivers
var drivers = Fmod.getAvailableDrivers()
 # change the audio driver
# you must pass in the id of the respective driver
Fmod.setDriver(id)
 # retrieve the id of the currently set driver
var id = Fmod.getDriver()
```

### Profiling & querying performance data

`getPerformanceData` returns an object which contains current performance stats for CPU, Memory and File Streaming usage of both FMOD Studio and the Core System.

```gdscript
# called every frame
var perf_data = FMOD.getPerformanceData()

print(perf_data.CPU)
print(perf_data.memory)
print(perf_data.file)
```

## Issues

This is a work in progress project, forked from [godot-fmod-integration](https://github.com/alexfonseka/godot-fmod-integration)
from [alexfonseka](https://github.com/alexfonseka). We'd like to thank him for the work he did, we simply adapted his
work to GDNative.  
This is currently not available to be putted in production environment, but we'll improve the driver with time. Feel free
to propose any modification using github's *pull request*. We hope you'll enjoy this driver.


[wowmeme]: .README/wowmeme.png
