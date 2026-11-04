# A.1: Installation

Game developers add FMOD to a Godot project by copying one addon folder and enabling its plugin.

The addon carries everything a project needs: the native libraries of every supported platform, the FMOD runtime libraries they load, and the editor tooling. Nothing has to be built, downloaded from FMOD or placed by hand (§4).

Configuring FMOD once installed is A.2; the platforms the addon ships for are F.1.

## Capabilities
- **A.1.1**: Game developers install FMOD GDExtension by copying the released `addons/fmod` folder into their project and enabling the plugin.
- **A.1.2**: Once started, FMOD updates every frame by itself, with or without the plugin enabled, and stops updating when it shuts down.
- **A.1.3**: A project upgrades to a new FMOD GDExtension release by replacing the addon folder, with no other step.

## Rules
- **A.1.R1**: The addon ships the FMOD runtime libraries matching the FMOD version it was built against.
  - Why: A mismatch between the extension and the FMOD libraries it loads fails at load time or behaves unpredictably.
- **A.1.R2**: One release supports one Godot minor version and later ones, declared as the `compatibility_minimum` of the extension.

## Non-goals
- **A.1.N1**: FMOD GDExtension does not distribute the FMOD Studio authoring tool or manage FMOD licences.
  - Why: FMOD Studio and its licensing belong to Firelight Technologies; the addon only redistributes the runtime libraries their licence allows.
