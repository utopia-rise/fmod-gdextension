# D.1: Audio files

Game code plays plain audio files through FMOD, without FMOD Studio.

Not every sound comes from FMOD Studio: a jingle, a user-provided track, a quick prototype. Game code loads an audio file of the project, as a sample kept in memory or as a stream, and plays instances of it through FMOD's mixer.

## Capabilities
- **D.1.1**: Game code loads an audio file of the project as a sound kept in memory, or as looping music streamed from disk, and unloads it.
- **D.1.2**: Game code creates instances of a loaded file and plays, stops, pauses them, and changes their volume and pitch.

## Rules
- **D.1.R1**: Audio files are read through Godot's file system, so they load the same from the project folder and from an exported package.
- **D.1.R2**: Creating an instance of a file that is not loaded reports a warning naming the file and returns no instance.

## Non-goals
- **D.1.N1**: Audio files played through FMOD are not positioned in the world.
  - Why: Positional audio is authored as FMOD Studio events (B.2); the Core API path stays a simple player.
  - Reopen if: users need positional playback of audio not authored in FMOD Studio.
