# B.2: Events

Game code creates and controls instances of the events a sound designer authored in FMOD Studio.

An event is what FMOD Studio hands to the game: a sound with its own logic, played as instances. Game code drives an instance's whole life (start, pause, stop, release) and places it in the world from a Godot transform or node. An event's description tells what the sound designer built, without playing it.

Fire-and-forget events are B.3, parameters are B.4, callbacks are B.6, and the node that plays an event from the scene is C.1.

## Capabilities
- **B.2.1**: Game code creates an instance of an event by its path, its GUID or its description.
- **B.2.2**: Game code starts, stops (immediately or letting it fade out), pauses, releases an event instance and reads its playback state.
- **B.2.3**: Game code changes an event instance's volume, pitch, timeline position, reverb levels and listener mask.
- **B.2.4**: Game code places an event instance in the world from a 2D transform, a 3D transform or a node.
- **B.2.5**: Game code triggers an event instance's sustain point cue (key off).
- **B.2.6**: Game code inspects an event's description: length, 3D or one-shot or snapshot or stream nature, distances, sound size, parameters, user properties and live instances.
- **B.2.7**: Game code loads and unloads an event's sample data ahead of playing it.

## Rules
- **B.2.R1**: An event is identified by the same path or GUID as in FMOD Studio.
- **B.2.R2**: Using a released or invalid event instance reports an error and never crashes.
