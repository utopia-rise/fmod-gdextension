# B.6: Event callbacks

Game code reacts to what an event does while it plays, and supplies the sounds of programmer instruments.

Events raise callbacks as they play: they start and stop, pass timeline beats and markers, and ask for the sound of a programmer instrument, such as the line of dialogue to say. Games synchronise visuals to music beats, trigger gameplay on markers, and pick localised dialogue from audio tables.

Callbacks exposed as node signals are C.1.

## Capabilities
- **B.6.1**: Game code receives an event instance's callbacks, filtered by type, in a `Callable` that gets the callback's properties.
- **B.6.2**: Timeline beat and marker callbacks carry the beat, bar, tempo, time signature, marker name and position.
- **B.6.3**: Game code sets the audio table key a programmer instrument plays, so that the event plays the matching entry of a loaded audio table bank.

## Rules
- **B.6.R1**: Callbacks reach game code on Godot's main thread, during the frame's FMOD update, whatever thread FMOD raised them on.
  - Why: FMOD raises callbacks on its own threads, and game code reacting to them touches the scene tree, which only the main thread may.
- **B.6.R2**: A callback whose target object was freed is dropped silently.
