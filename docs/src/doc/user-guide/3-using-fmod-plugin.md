# Using FMOD plugin

This documentation provides detailed guidance on how to use the FMOD GDExtension plugin for integrating FMOD into your
Godot projects.

The plugin offers two main approaches for interacting with FMOD:
1. **Using FMOD Nodes**: Predefined nodes that simplify common FMOD tasks.
2. **Using the `FmodServer` API**: A singleton that gives you direct access to FMOD's core and system APIs for more
advanced or customized workflows.

#### Core Component: `FmodServer`
At the heart of the plugin is the `FmodServer` singleton. It serves as a bridge to FMOD's powerful API, allowing you to
control audio behavior programmatically. FMOD nodes are built on top of this singleton, offering higher-level
abstractions for convenience.

## Summary
- Loading banks
    - [Using node](4-loading-banks.md#fmodbankloader-node)
    - [Using FmodServer](4-loading-banks.md#fmodserver-api)
- Playing events
    - [Using node](5-playing-events.md#fmodeventemitter-nodes)
    - [Using FmodServer](5-playing-events.md#fmodserver-api)
- Listeners
    - [Using node](6-listeners.md#fmod-listener-nodes)
    - [Using FmodServer](6-listeners.md#using-fmodserver-api)
- [Playing sounds](7-playing-sounds.md)
- [Other low level examples](8-other-low-level-examples.md)


