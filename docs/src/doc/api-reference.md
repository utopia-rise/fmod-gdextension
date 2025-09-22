# API Reference

The FMOD GDExtension provides a comprehensive set of classes and nodes for integrating FMOD Studio audio into your Godot projects. This reference covers all the major classes and their functionality.

## Core Classes

### FmodServer

The main singleton class that manages the FMOD Studio API integration. Access it directly as `FmodServer` since it's registered as a singleton.

**Key Methods:**
- `init(p_settings: FmodGeneralSettings)` - Initialize the FMOD system
- `load_bank(pathToBank: String, flag: int) -> FmodBank` - Load an FMOD bank file
- `create_event_instance(eventPath: String) -> FmodEvent` - Create an event instance
- `get_event(eventPath: String) -> FmodEventDescription` - Get event description
- `get_bus(busPath: String) -> FmodBus` - Get bus by path
- `get_vca(vcaPath: String) -> FmodVCA` - Get VCA by path
- `play_one_shot(event_name: String)` - Play a one-shot event
- `update()` - Update the FMOD system (call every frame)
- `shutdown()` - Shutdown and cleanup FMOD

**Global Parameters:**
- `set_global_parameter_by_name(name: String, value: float)` - Set global parameter value
- `get_global_parameter_by_name(name: String) -> float` - Get global parameter value

### FmodEvent

Represents an instance of an FMOD Studio event that can be played and controlled.

**Playback Control:**
- `start()` - Start event playback
- `stop(stopMode: int)` - Stop event with fade options
- `set_paused(paused: bool)` - Pause/unpause the event
- `release()` - Release the event instance
- `event_key_off()` - Trigger key off for sustain points

**Parameters:**
- `set_parameter_by_name(name: String, value: float)` - Set event parameter
- `get_parameter_by_name(name: String) -> float` - Get event parameter
- `set_parameter_by_id(id: int, value: float)` - Set parameter by ID

**Spatial Audio:**
- `set_3d_attributes(transform: Transform3D)` - Set 3D position and rotation
- `set_2d_attributes(position: Transform2D)` - Set 2D position
- `set_node_attributes(node: Node)` - Attach to node's transform

**Properties:**
- `get_playback_state() -> int` - Get current playback state
- `get_volume() -> float` / `set_volume(volume: float)` - Volume control
- `get_pitch() -> float` / `set_pitch(pitch: float)` - Pitch control

### FmodEventDescription

Metadata and factory for FMOD Studio events. Obtained from `FmodServer.get_event()`.

**Instance Management:**
- `create_instance() -> FmodEvent` - Create new event instance  
- `get_instance_count() -> int` - Get active instance count
- `get_instance_list() -> Array` - Get all active instances
- `release_all_instances()` - Release all instances

**Properties:**
- `is_3d() -> bool` - Check if event is 3D
- `is_one_shot() -> bool` - Check if event is one-shot
- `is_stream() -> bool` - Check if event streams audio
- `get_length() -> int` - Get event length in milliseconds
- `get_min_max_distance() -> Array` - Get 3D distance range

**Parameters:**
- `get_parameter_count() -> int` - Get number of parameters
- `get_parameter_by_name(name: String) -> FmodParameterDescription` - Get parameter info
- `get_parameters() -> Array` - Get all parameters

### FmodBank

Represents a loaded FMOD Studio bank containing audio content.

**Content Access:**
- `get_event_description_count() -> int` - Number of events in bank
- `get_description_list() -> Array` - Get all event descriptions
- `get_bus_count() -> int` - Number of buses in bank  
- `get_bus_list() -> Array` - Get all buses
- `get_vca_count() -> int` - Number of VCAs in bank
- `get_vca_list() -> Array` - Get all VCAs

**State:**
- `get_loading_state() -> int` - Check bank loading status
- `update_bank_data()` - Force update cached data

## Node Classes

### FmodEventEmitter3D

A 3D node for playing FMOD Studio events with automatic position tracking.

**Properties:**
- `event_name: String` - Path to FMOD event (e.g., "event:/Music/Level1")
- `event_guid: String` - GUID of the event
- `autoplay: bool` - Start playing when entering scene tree
- `volume: float` - Volume multiplier (default: 1.0)
- `attached: bool` - Auto-update position from node transform

**Methods:**
- `play(restart_if_playing: bool = true)` - Start event playback
- `stop()` - Stop event (respects allow_fadeout setting)
- `play_one_shot()` - Play as one-shot event
- `set_parameter(name: String, value: Variant)` - Set event parameter
- `get_parameter(name: String) -> Variant` - Get event parameter
- `get_event() -> FmodEvent` - Get the underlying event instance

**Signals:**
- `started()` - Event started playing
- `stopped()` - Event stopped
- `restarted()` - Event restarted
- `start_failed()` - Event failed to start

### FmodEventEmitter2D

A 2D node for playing FMOD Studio events with automatic position tracking. Has the same interface as FmodEventEmitter3D but works in 2D space.

### FmodListener3D

A 3D audio listener node that defines where the "ears" are located in 3D space.

**Properties:**
- `listener_index: int` - Listener index (0-7)  
- `listener_weight: float` - Listener influence weight
- `locked: bool` - Lock position updates

**Methods:**
- `set_listener_index(index: int)` - Set listener index
- `set_listener_weight(weight: float)` - Set listener weight
- `set_locked(locked: bool)` - Lock/unlock position updates

### FmodListener2D

A 2D audio listener node. Has the same interface as FmodListener3D but works in 2D space.

### FmodBankLoader

Utility node for automatically loading FMOD Studio banks when entering the scene tree.

**Properties:**
- `bank_paths: Array` - Array of bank file paths to load

**Methods:**
- `set_bank_paths(paths: Array)` - Set banks to load
- `get_bank_paths() -> Array` - Get current bank paths

## Audio System Classes

### FmodBus

Audio bus for mixing and effects processing.

**Volume Control:**
- `get_volume() -> float` / `set_volume(volume: float)` - Bus volume
- `get_mute() -> bool` / `set_mute(mute: bool)` - Mute control
- `get_paused() -> bool` / `set_paused(paused: bool)` - Pause control

**Event Management:**
- `stop_all_events(stopMode: int)` - Stop all events on this bus

### FmodVCA

Volume Control Automation for centralized volume control across multiple buses.

**Methods:**
- `get_volume() -> float` / `set_volume(volume: float)` - VCA volume multiplier

### FmodParameterDescription

Describes the properties and constraints of an FMOD Studio event parameter.

**Properties:**
- `get_name() -> String` - Parameter name
- `get_id() -> int` - Parameter ID
- `get_minimum() -> float` - Minimum value
- `get_maximum() -> float` - Maximum value
- `get_default_value() -> float` - Default value

**Flags:**
- `is_global() -> bool` - Global parameter
- `is_discrete() -> bool` - Discrete values only
- `is_labeled() -> bool` - Has labeled values
- `is_automatic() -> bool` - Automatic parameter
- `is_read_only() -> bool` - Read-only parameter

## Usage Examples

### Basic Event Playback

```gdscript
# Play a simple one-shot event directly
FmodServer.play_one_shot("event:/UI/ButtonClick")

# Create and control an event instance
var music_event = FmodServer.create_event_instance("event:/Music/Level1")
music_event.start()
music_event.set_parameter_by_name("Intensity", 0.8)
```

### Using 3D Event Emitters

```gdscript
# Add an FmodEventEmitter3D node to your scene
var emitter = FmodEventEmitter3D.new()
emitter.event_name = "event:/Ambient/Forest"
emitter.autoplay = true
emitter.volume = 0.7
add_child(emitter)

# Control parameters
emitter.set_parameter("TimeOfDay", 0.5)
```

### Bank Management

```gdscript
# Load a bank
var bank = FmodServer.load_bank("res://audio/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL)

# Wait for async loading
while bank.get_loading_state() == FMOD_STUDIO_LOADING_STATE_LOADING:
    await get_tree().process_frame

# Use events from the bank
var event_desc = FmodServer.get_event("event:/Music/MainTheme")
var event = event_desc.create_instance()
```

### Bus and VCA Control

```gdscript
# Control bus volume
var music_bus = FmodServer.get_bus("bus:/Music")
music_bus.set_volume(0.5)

# Control VCA
var master_vca = FmodServer.get_vca("vca:/Master")
master_vca.set_volume(0.8)
```

This API reference provides the essential information for using the FMOD GDExtension. For complete method signatures and additional details, use Godot's built-in Help system when the extension is loaded.