# C.1: Event emitters

`FmodEventEmitter2D` and `FmodEventEmitter3D` play an FMOD event from the scene tree, set up in the Inspector and controlled like any Godot node.

Most sounds belong to something in the scene: an engine on a car, music in an area, a voice on a character. An emitter is placed in the scene, given an event and options in the Inspector, and plays it from its own position, with the event's parameters as node properties and its callbacks as signals (§2). Everything the node does is also available to code through B.2, B.4 and B.6.

Choosing the event and editing parameters in the Inspector is E.2; finding events by path or GUID is set by A.2.4.

## Capabilities
- **C.1.1**: Game developers add an emitter to a 2D or 3D scene, choose its event by path or GUID, and it plays the event from the node's position.
- **C.1.2**: An emitter plays its event automatically when ready, or when game code calls `play`, and stops it with or without fade out.
- **C.1.3**: An emitter's event follows the node while it moves, unless the emitter is detached.
- **C.1.4**: An emitter exposes its event's parameters as node properties, editable in the Inspector, animatable and set from code by name or ID.
- **C.1.5**: An emitter emits signals when its event starts, restarts, fails to start, stops, passes a timeline beat or a marker.
- **C.1.6**: An emitter plays a one-shot of its event, sets the volume of its event, and sets the audio table key of its programmer instruments.
- **C.1.7**: An emitter preloads its event when ready, and frees itself when its event ends if auto release is on.
- **C.1.8**: An emitter pauses its event when the scene tree pauses it and resumes it when it is unpaused.
- **C.1.9**: Game code gets the event instance an emitter plays, to control it through B.2, and reads and sets the event's timeline position on the emitter.

## Rules
- **C.1.R1**: Changing an emitter's event stops and releases the previous one and clears its parameter values: the node no longer exposes the previous event's parameters. In the editor the node keeps their values, so undoing the change gives them back.
- **C.1.R2**: An emitter leaving the tree stops and releases its event.
- **C.1.R3**: An emitter has no event instance to give before it plays or preloads its event, after its event is released, and in the editor; it never creates one when asked, and a one-shot's instance is never the emitter's.

## Non-goals
- **C.1.N1**: Emitters do not play in the editor.
  - Why: Scenes open in the editor stay silent; sound designers audition events in the bank explorer (E.1).
  - Reopen if: users need to hear an emitter in its scene context while editing it.
