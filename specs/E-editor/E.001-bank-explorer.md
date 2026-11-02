# E.1: Bank explorer

The bank explorer shows the content of the project's banks in the editor and lets sound designers audition events.

Opened from the editor toolbar, the explorer reads the banks found in the banks folder (A.2.5): their events, buses, VCAs and parameters, with paths and GUIDs. Sound designers and game developers find what FMOD Studio built without leaving Godot, and hear an event before placing it.

## Capabilities
- **E.1.1**: Game developers open the bank explorer from the editor toolbar and browse the events, buses, VCAs and banks of the project with their paths and GUIDs.
- **E.1.2**: Sound designers play and stop an event from the bank explorer, with or without fade out, and change its parameters while it plays.
- **E.1.3**: The bank explorer reloads the banks on demand, so that banks rebuilt in FMOD Studio show without restarting the editor.

## Rules
- **E.1.R1**: The bank explorer warns, naming the expected path, when the master banks are missing from the banks folder.
