# Custom FMOD Plugins

The Godot FMOD integration allows you to load custom FMOD plugins, such as effects or codecs, to extend the capabilities of the FMOD Core API. You can load these plugins either automatically at startup via Project Settings or manually at runtime using GDScript.

## General Plugin Guidelines

Before diving into the loading methods, please keep these points in mind:

*   **Compatibility:** Ensure your custom FMOD plugins are compatible with the FMOD Core API version used by this Godot FMOD integration. You can usually find the FMOD version details in the integration's release notes or main documentation page.
*   **Platform-Specific Libraries:** Plugin files are platform-specific. Use `.dll` for Windows, `.so` for Linux, and `.dylib` for macOS. Ensure you provide the correct plugin for each target platform.
*   **Godot Resource Paths:** When specifying paths, use Godot's resource path system (e.g., `res://`, `user://`).
*   **Troubleshooting:** If a plugin fails to load, FMOD will typically output error messages to Godot's console or log output. Check these logs for details if you encounter issues.
*   **FMOD Studio Projects:** While this feature primarily concerns runtime/Core API plugins, if your plugins are also referenced within an FMOD Studio project (e.g., as a plugin effect on an event), ensure your Studio project setup aligns with the plugin names and types.

## Automatic Plugin Loading via Project Settings

You can configure FMOD to load a list of plugins automatically when the FMOD system initializes. This is useful for plugins that are essential for your project's audio setup.

1.  Go to **Project > Project Settings...**.
2.  Navigate to the **FMOD > General** tab.
3.  Find the **Plugin Paths** setting.

    ![FMOD Plugin Paths Setting](assets/fmod-plugin-paths-setting.png) <!-- TODO: Add an image for this setting -->

4.  This setting is a `PackedStringArray`. Each string in the array should be a valid Godot resource path to an FMOD plugin's dynamic library.
    *   Example for Windows: `res://fmod_plugins/my_custom_effect.dll`
    *   Example for Linux: `res://fmod_plugins/my_custom_codec.so`
    *   Example for macOS: `user://custom_plugins/my_filter.dylib`

Plugins listed here are loaded by the FMOD Core API early in the initialization sequence, before most other FMOD setup occurs but after the basic Core System object is created.

## Manual Plugin Loading via GDScript

For more dynamic control, or for plugins that are not known at startup, you can load them manually using a GDScript function.

The `FmodServer` singleton provides the following function:

```gdscript
FmodServer.load_plugin(path_to_plugin: String)
```

*   `path_to_plugin`: A String representing a Godot resource path to the plugin's dynamic library.

**Example:**

```gdscript
func _ready():
    # Load a plugin from the "res://fmod_plugins/" directory
    FmodServer.load_plugin("res://fmod_plugins/my_runtime_plugin.dll")

    # You might want to check FMOD logs for successful loading here
    # or attempt to use the plugin's features.
```

This method offers flexibility, allowing you to load plugins based on game state, platform, or other runtime conditions.

Remember to consult the FMOD documentation for details on creating and using FMOD plugins.
