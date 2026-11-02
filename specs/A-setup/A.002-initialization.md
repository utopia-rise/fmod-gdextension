# A.2: Initialization

FMOD is configured from Project Settings and starts with the game, or when game code decides.

FMOD needs its system settings (channel count, sample rate, speaker mode, DSP buffers, 3D scaling) before it starts, and most cannot change afterwards. They live in Project Settings under `Fmod/`, like any Godot setting (§2), and the same values apply in the editor and in exports (§3). A game that must choose them at run time, from an options menu for instance, turns automatic start off and starts FMOD itself.

Output devices and the mixer at run time are A.3; FMOD logging and profiling are E.3.

## Capabilities
- **A.2.1**: Game developers configure FMOD's general, software format, DSP and 3D settings in Project Settings.
- **A.2.2**: FMOD starts automatically when the game starts, using the Project Settings values.
- **A.2.3**: Game code turns automatic start off and starts FMOD itself with settings of its own, before any other FMOD call.
- **A.2.4**: Game developers choose in Project Settings whether nodes find events and parameters by path and name or by GUID and ID.
- **A.2.5**: Game developers set in Project Settings the folder holding the project's banks, used by the editor addon.

## Rules
- **A.2.R1**: FMOD always starts in the editor, whatever the automatic start setting, so that the editor addon can read banks and audition events.
- **A.2.R2**: Calling FMOD before it is started reports an error and never crashes.
- **A.2.R3**: 3D positions use Godot's units: 3D distances in meters and 2D distances in pixels, converted with the distance factor setting.
