# Settings

FMOD's settings are in **Project → Project Settings → Fmod**. They are read once, when FMOD starts: restart the game
after changing them.

They are split into five sections, each with its own page:

| Section | What it covers |
|---|---|
| [General](1-general.md) | Whether FMOD starts by itself, and where your banks are. |
| [Mixer](2-mixer.md) | How many sounds FMOD plays, its sound format, where its sound goes, and how much delay you trade for stability. |
| [Spatialization](3-spatialization.md) | How sounds are placed around the listeners, in 2D and in 3D. |
| [Debugging](4-debugging.md) | Connecting FMOD Studio to the running game, and FMOD's logs. |
| [Plugins](5-plugins.md) | The FMOD plugins your banks need. |

## Reading these pages

Each setting is described with a short table:

| **Project Setting** | Its path, below `Fmod/`. |
|---|---|
| **Property** | The matching property of `FmodSettings`, to set it from code. |
| **Default** | Its value until you change it. Some differ per platform. |
| **Advanced** | *Yes* when it only shows with **Advanced Settings** on, in the top right corner of Project Settings. |

## Defaults per platform

A few defaults depend on the platform the game runs on, following FMOD's own recommendations for each:

| Setting | Desktop | Mobile | Web | Web, with threads |
|---|---|---|---|---|
| [Channel Count](2-mixer.md#channel-count) | 1024 | 512 | 512 | 512 |
| [Buffer Size](2-mixer.md#buffer-size) | 512 | 1024 | 2048 | 1024 |
| [Buffer Count](2-mixer.md#buffer-count) | 4 | 4 | 2 | 4 |
| [Studio Processing](2-mixer.md#studio-processing) | Background Thread | Background Thread | Main Thread | Background Thread |
| [Live Update](4-debugging.md#enabled) | On | On | Off | Off |

The editor shows the desktop values. A setting you leave at its default takes the value of the platform you export to,
so you only need to change one to move away from these defaults.

## A different value per platform

Any setting can have its own value on some platforms, with Godot's *feature overrides*:

1. Turn on **Advanced Settings**.
2. In the bar at the top, type the setting's path, such as `Fmod/Mixer/latency/buffer_size`.
3. Pick a feature in the drop-down next to it, such as `mobile`, `web`, `ios` or `release`, and click **Add**.
4. A new line, such as *Buffer Size.mobile*, appears under the setting: its value replaces the setting's on that
   platform.

## Setting them from code

To start FMOD yourself, turn [Auto Initialize](1-general.md#auto-initialize) off and give `FmodServer.init()` an
`FmodSettings`, which holds every setting of these pages, under the names given in their **Property** line:

```gdscript
var settings := FmodSettings.new()
settings.channel_count = 512
settings.output_routing = FmodSettings.OutputRouting.GODOT_BUS
FmodServer.init(settings)
```

A new `FmodSettings` holds the [defaults of the platform](#defaults-per-platform) it runs on. Call `FmodServer.init()`
without settings to use the Project Settings. See [Initialization](../user-guide/2-initialization.md).

## Upgrading from an older version

Older versions had different sections: *General*, *Software Format*, *DSP*, *3D Settings*, *Logging* and *Plugins*.
When the editor opens a project made with one of them, it moves the values you set to the new settings and saves the
project. Feature overrides of the old settings, such as `DSP/dsp_buffer_size.mobile`, are not moved: add them again on
the new settings.

Scripts that read FMOD's settings with `ProjectSettings.get_setting()` need the new paths, given on each page.
