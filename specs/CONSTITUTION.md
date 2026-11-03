# Constitution

Global rules of FMOD GDExtension. No spec may contradict an article. An implementation that does not yet satisfy an article breaks it, and the closest capability is `partial` with a tracker note naming the article. Articles have no priority order; when two pull in different directions, the maintainer decides. Adding, changing or deleting an article needs the maintainer's approval. Format: specs/FORMAT.md.

## §1: Godot integration goes through GDExtension only
- FMOD GDExtension integrates with Godot as a GDExtension addon and runs on the official, unmodified Godot editor and export templates.
  - Why: Users install an addon instead of building or downloading a custom engine.

## §2: Godot comes first
- FMOD is offered through the concepts a Godot user already knows: nodes, resources, signals, properties, Project Settings, `res://` paths, editor tooling and the built-in class reference, under Godot's naming conventions.
  - Why: FMOD GDExtension is used to make Godot games, so Godot's habits and workflows must keep applying; FMOD's own concepts (events, banks, buses, parameters) keep their FMOD names so FMOD Studio and its documentation stay recognisable.

## §3: The same game everywhere
- A game has the same audio features in the editor, in a debug export and in a release export, on every supported platform.
  - Except: debugging aids (live update, profiling, verbose FMOD logging); a mechanism imposed by a platform, such as static FMOD plugins on iOS; a feature FMOD's runtime does not offer on a platform, such as FMOD plugins on the web.
  - Why: What a sound designer hears while developing must be what players hear once the game ships, wherever it ships.

## §4: Users never touch the native side
- Everything a user needs is done from Godot: scripts, the Inspector, Project Settings and the editor addon; they never compile C++, link FMOD or place native libraries by hand.
  - Except: the libraries of third-party FMOD plugins, which the user supplies.
  - Why: The bridge between Godot and FMOD is FMOD GDExtension's job, not the user's.

## §5: Every Godot language is served
- Every capability is reachable from every Godot scripting language through the same API registered with Godot; nothing is reserved to GDScript.
  - Except: the editor addon's own user interface, which is written in GDScript.
  - Why: C# and other GDExtension languages are first-class Godot languages, and their users choose FMOD for the same reasons.

## §6: The API follows Godot's stability
- FMOD GDExtension's public API (classes, methods, signals, node properties and Project Settings names) changes incompatibly only in a major version or when a Godot or FMOD change forces it.
  - Why: Outside major versions, upgrading FMOD GDExtension must never cost more than upgrading Godot itself.
