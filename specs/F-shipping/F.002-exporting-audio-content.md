# F.2: Exporting audio content

Exports carry the game's banks and audio files without game developer action, and each export preset can opt out.

FMOD reads banks and audio files itself, so Godot does not know they are needed and would leave them out of the exported package. The addon adds them, from wherever they are in the project, so that an export plays what the editor played (§3).

## Capabilities
- **F.2.1**: Exports include every bank and every `.ogg`, `.mp3` and `.wav` file of the project, so that the game finds them at the same `res://` paths.
- **F.2.2**: Game developers turn the automatic export of audio content off for one export preset, to ship the banks some other way.

## Rules
- **F.2.R1**: Exporting audio content never picks up files of the Android Gradle build directory.
