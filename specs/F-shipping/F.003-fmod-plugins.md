# F.3: FMOD plugins

Games use third-party FMOD plugins, such as Steam Audio or Resonance, configured from a Godot resource and loaded and exported on every platform.

FMOD plugins add DSP effects, codecs and outputs that FMOD Studio projects depend on. Their libraries are the only native files a user supplies (§4 Except); the addon loads them at startup and packs them with each export, in the form each platform requires: dynamic libraries on desktop and Android, static libraries registered through generated code on iOS.

## Capabilities
- **F.3.1**: Game developers declare their FMOD plugins in an `FmodPluginsSettings` resource set in Project Settings, and FMOD loads them at startup.
- **F.3.2**: Exports pack each declared plugin's libraries, and the libraries placed next to them, for the exported platform and architecture.
- **F.3.3**: On iOS, plugins are linked statically from the methods declared in the settings resource, without game developer code.
- **F.3.4**: Game code loads, unloads and checks a plugin at run time.

## Rules
- **F.3.R1**: Plugin libraries live in one folder per platform (and per architecture on Android) under the directory set in the plugins resource.
