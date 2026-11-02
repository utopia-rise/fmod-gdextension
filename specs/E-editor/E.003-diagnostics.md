# E.3: Diagnostics

Game developers see what FMOD does: its logs in Godot's output, its performance in Godot's monitors, and live update with FMOD Studio.

Audio bugs are found by listening and by measuring. FMOD reports through Godot's own tools (§2): its messages go to the Output panel, its CPU, memory and file usage to the debugger's monitors, and FMOD Studio connects to a running game to mix it live.

## Capabilities
- **E.3.1**: Game developers choose in Project Settings FMOD's logging level and whether its messages go to Godot's output, the terminal or a file.
- **E.3.2**: The debugger's monitors show FMOD's CPU, memory and file usage while the game runs.
- **E.3.3**: Game code reads FMOD's current CPU, memory and file usage.
- **E.3.4**: Sound designers connect FMOD Studio to a running game with live update, enabled in Project Settings, and track memory when memory tracking is enabled.

## Rules
- **E.3.R1**: The editor never enables live update, so that FMOD Studio connects to the running game and not to the editor.
- **E.3.R2**: Errors returned by FMOD that game code can act on reach Godot's output with the failing operation named.
