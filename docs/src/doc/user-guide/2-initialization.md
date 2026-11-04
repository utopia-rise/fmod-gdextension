# Initialization

When you first add fmod addon to your godot project, you need to setup fmod options within projects parameters.  
Parameters are split in several categories:  
- General  
- Output  
- Software Format  
- Dsp  
- 3d Settings  

## General

- "Auto Initialize": If `true`, will start fmod on engine startup.
- "Channel Count": Maximum number of Channel objects available for playback, also known as virtual voices. Virtual 
voices will play with minimal overhead, with a subset of 'real' voices that are mixed, and selected based on priority
and audibility. See the Virtual Voices guide for more information.
- "Live update": Enable live update.
- "Memory Tracking": Enables detailed memory usage statistics. Increases memory footprint and impacts performance.
See Studio::Bus::getMemoryUsage and Studio::EventInstance::getMemoryUsage for more information.
Implies FMOD_INIT_MEMORY_TRACKING.
- Default Listener count: set max listener count (should be between 1 and 8).  
- Should Load by Name: If true will load events and parameters by name instead of id when using fmod nodes.
- "Studio Processing": Where FMOD Studio carries out your requests: on its own thread (default), or during
`FmodServer.update()`. See [Output, latency and sync modes](10-output-and-latency.md#studio-processing).

![general-tab]

## Output

- "Output Routing": Where FMOD's sound goes: straight to the speakers (default), or through one of Godot's audio buses.
- "Godot Audio Bus", "Godot Bus Mixing" and "Sync to Godot Mix Rate": With a Godot bus, the bus FMOD plays on, when FMOD
prepares its sound, and its sample rate.

See [Output, latency and sync modes](10-output-and-latency.md).

## Software Format

- "Sample Rate": Sample rate of the mixer. Range: [8000, 192000] Units: Hertz Default: 48000
- "Speaker Mode": Speaker setup of the mixer.
[FMOD_SPEAKERMODE](https://www.fmod.com/docs/2.03/api/core-api-common.html#fmod_speakermode)
- "Raw Speaker Count": Number of speakers for
[FMOD_SPEAKERMODE_RAW](https://www.fmod.com/docs/2.03/api/core-api-common.html#fmod_speakermode_raw) mode.
Range: [0, [FMOD_MAX_CHANNEL_WIDTH](https://www.fmod.com/docs/2.03/api/core-api-common.html#fmod_max_channel_width)]

![software-format-tab]

## Dsp

- "Dsp Buffer Size": The mixer engine block size. Use this to adjust mixer update granularity. Units: Samples Default:
1024
- "Dsp Buffer Count": The mixer engine number of buffers used. Use this to adjust mixer latency. Default: 4

![dsp-tab]

## 3d Settings

- "Doppler Scale": A scaling factor for doppler shift. Default 1.
- "Distance Factor": A factor for converting game distance units to FMOD distance units. Default 1.  
  !!! warning
  In 2D this value represents pixels, so you should set it to the number of pixel for your world meter (If your world
  meter is 64px, set it to 64). In 3D this represents meter, so we recommend to set it to 1.
- "Rolloff Scale": A scaling factor for distance attenuation. When a sound uses a roll-off mode other than
FMOD_3D_CUSTOMROLLOFF and the distance is greater than the sound's minimum distance, the distance is scaled by the
roll-off scale.

![3d-tab]

## Starting FMOD yourself

With "Auto Initialize" on, FMOD starts with the game using the Project Settings above. To choose its settings at run
time instead, from an options menu for instance, turn "Auto Initialize" off and call `FmodServer.init()` yourself:

```gdscript
# With the Project Settings.
FmodServer.init()
```

```gdscript
# With settings of your own.
var settings := FmodGeneralSettings.new()
settings.channel_count = 512
settings.software_format = FmodSoftwareFormatSettings.new()
settings.software_format.sample_rate = 44100
FmodServer.init(settings)
```

`FmodGeneralSettings` holds the general and output settings, and the software format, DSP, 3D, logging and plugin
settings as `software_format`, `dsp`, `sound_3d`, `logging` and `plugins`. One you leave empty is not applied: FMOD
keeps what it already uses.

## Fmod explorer

When all is setup you can explore your project's banks using `Fmod Explorer`.  
![fmod-explorer]


[general-tab]: ./assets/parameter-general.png
[software-format-tab]: ./assets/parameters-software-format.png
[dsp-tab]: ./assets/parameters-dsp.png
[3d-tab]: ./assets/parameters-3d.png
[fmod-explorer]: ./assets/fmod-explorer.png