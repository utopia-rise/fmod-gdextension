# D.1: Audio files

Game code plays plain audio files through FMOD, without FMOD Studio.

Not every sound comes from FMOD Studio: a jingle, a user-provided track, a quick prototype. Game code loads an audio file of the project, as a sample kept in memory or as a stream, and plays instances of it through FMOD's mixer, in the world or not.

## Capabilities
- **D.1.1**: Game code loads an audio file of the project as a sound kept in memory, or as looping music streamed from disk, as a file object that stays loaded while game code or an instance of it references it.
- **D.1.2**: Game code creates instances from a loaded file object and plays, stops, pauses them, and changes their volume and pitch.
- **D.1.3**: Game code places an instance in the world, with a position and a velocity in 3D or in 2D, so that FMOD spatializes it against the listeners.

## Rules
- **D.1.R1**: Audio files are read through Godot's file system, so they load the same from the project folder and from an exported package.
