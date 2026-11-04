# Initialization

FMOD starts with the game, using the settings in **Project Settings → Fmod**. The [Settings](../settings/index.md)
section describes each of them, section by section.

## Starting FMOD yourself

With [Auto Initialize](../settings/1-general.md#auto-initialize) on, FMOD starts with the game. To choose its settings
at run time instead, from an options menu for instance, turn Auto Initialize off and call `FmodServer.init()` yourself,
before any other FMOD call:

```gdscript
# With the Project Settings.
FmodServer.init()
```

```gdscript
# With settings of your own.
var settings := FmodSettings.new()
settings.channel_count = 512
settings.sample_rate = 44100
FmodServer.init(settings)
```

`FmodSettings` holds every setting, under the names given in each setting's **Property** line. A new one holds the
[defaults of the platform](../settings/index.md#defaults-per-platform) the game runs on.

## Fmod explorer

When all is set up, you can explore your project's banks with the `Fmod Explorer` button of the editor's toolbar.
![fmod-explorer]

[fmod-explorer]: ./assets/fmod-explorer.png
