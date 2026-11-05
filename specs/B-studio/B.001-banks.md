# B.1: Banks

Game code loads the banks built by FMOD Studio from the project's files and keeps them loaded as long as it holds them.

Banks hold everything a sound designer authored: events, buses, VCAs and their sample data. A bank is loaded from a `res://` path like any Godot resource (§2), from the project folder or from an exported package alike (§3), and lives as long as something references it, like a Godot `RefCounted`. Loading a bank makes its events, buses and VCAs findable by path and GUID.

Loading banks from the scene tree without code is C.3; exporting banks with the game is F.2.

## Capabilities
- **B.1.1**: Game code loads a bank from a `res://` path and gets back a bank object that keeps it loaded while referenced.
- **B.1.2**: Game code loads banks without blocking, checks whether banks are still loading, and waits for all pending loads.
- **B.1.3**: Game code lists the loaded banks and, for each, its loading state, events, buses, VCAs and string count.
- **B.1.4**: Game code finds an event, bus or VCA of the loaded banks by path or GUID, checks that one exists, and lists all of them.
- **B.1.5**: Game code loads and unloads the sample data of all of a bank's events ahead of playing them, and checks its sample loading state.

## Rules
- **B.1.R1**: Banks are read through Godot's file system, so a bank loads the same from the project folder and from an exported package.
- **B.1.R2**: Loading a bank that is already loaded returns the same bank and never loads it twice.
- **B.1.R3**: A bank is unloaded when no bank object references it any more, and the events, buses and VCAs it brought are forgotten with it.
- **B.1.R4**: A bank path that does not exist reports an error naming the path, instead of failing inside FMOD.
