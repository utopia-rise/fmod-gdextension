# B.5: Mixing

Game code controls the mix a sound designer built: buses, VCAs, and every event at once.

Options menus and game states act on the mix rather than on single events: a music volume slider drives a VCA, a pause menu pauses the game's events, a cutscene mutes a bus.

## Capabilities
- **B.5.1**: Game code mutes, pauses, changes the volume of a bus and stops all the events routed through it.
- **B.5.2**: Game code reads and changes the volume of a VCA.
- **B.5.3**: Game code pauses, resumes, mutes and unmutes all events at once.
