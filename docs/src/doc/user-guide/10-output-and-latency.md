# Output, latency and sync modes

Three settings decide where FMOD's sound goes and how quickly a sound you trigger reaches the speakers:

- **Output Routing:** FMOD plays to the speakers on its own, or through one of Godot's audio buses.
- **Godot Bus Mixing:** with a Godot bus, FMOD prepares its sound ahead of time, or only when Godot asks for it.
- **Studio Processing:** FMOD Studio carries out your requests on its own thread, or during `FmodServer.update()`.

The defaults suit most games. This page explains what each choice changes, and which ones to pick to avoid stutter, to
get the shortest delay, or to record videos.

## Where the delay comes from

Between the moment your code plays an event and the moment it reaches the speakers, the request goes through these
steps:

1. **It waits for `FmodServer.update()`.** FMOD receives your requests when `FmodServer.update()` runs. The
   `FmodManager` autoload calls it once per frame, in its `_process()`, which runs before your scene's nodes. Requests
   made while handling input or in `_physics_process()` reach FMOD in the same frame; requests made in your nodes'
   `_process()` wait for the next frame, up to 16 ms at 60 frames per second.
2. **FMOD Studio carries it out.** With **Studio Processing** on *Background Thread*, FMOD's own thread picks it up
   within 20 ms. On *Main Thread*, it happens right away, inside `FmodServer.update()`.
3. **FMOD mixes it.** FMOD produces sound in small blocks, set by **Fmod → DSP → DSP Buffer Size** (512 samples, about
   10 ms, by default).
4. **The sound waits to be played.** Directly to the device, FMOD keeps a few blocks ready (**DSP Buffer Count**, 4 by
   default). Through a Godot bus, *Mix Ahead* keeps about 40 ms ready, *On Demand* none, and Godot adds its own output
   delay (**Audio → Driver → Output Latency**, 15 ms by default).

Each step is a trade: the more sound is prepared in advance, the less likely the sound is to stutter, and the later it
is heard.

## The settings

**Project Settings → Fmod → Output:**

| Setting | Choices | What it does |
|---|---|---|
| **Output Routing** | *Direct to Device* (default), *Godot Bus* | Where FMOD's sound goes. |
| **Godot Audio Bus** | `Master` by default | The Godot bus that carries FMOD's sound. |
| **Godot Bus Mixing** | *Mix Ahead* (default), *On Demand* | When FMOD prepares its sound for Godot. |
| **Sync to Godot Mix Rate** | off by default | See [Sample rate](#sample-rate). |

The last three only show once Output Routing is *Godot Bus*.

**Project Settings → Fmod → General:**

| Setting | Choices | What it does |
|---|---|---|
| **Studio Processing** | *Background Thread* (default), *Main Thread* | Where FMOD Studio carries out your requests. |

Studio Processing and Sync to Godot Mix Rate show with *Advanced Settings* on. Hover a setting for a short description.

Restart your game after changing them: they are read when the game starts. If you start FMOD yourself from a script
(see [Initialization](2-initialization.md)), set them on the settings you give it:

```gdscript
var settings := FmodGeneralSettings.new()
settings.output_routing = FmodGeneralSettings.OutputRouting.GODOT_BUS
settings.godot_audio_bus = "SFX"
settings.godot_bus_mixing = FmodGeneralSettings.GodotBusMixing.ON_DEMAND
settings.studio_processing = FmodGeneralSettings.StudioProcessing.MAIN_THREAD
FmodServer.init(settings)
```

## Output Routing

### Direct to Device

FMOD plays to the speakers by itself, next to Godot's audio. This is the default.

- **Pros:** the simplest setup. FMOD keeps its own output device and its surround layouts (5.1, 7.1). It is the only
  choice on the web.
- **Cons:** Godot's buses, volume and effects do not touch FMOD's sound, the "Mute Game" button of Godot's Game view
  does not mute it, and Godot's Movie Maker mode does not record it.

### Godot Bus

FMOD's sound plays on a Godot audio bus, like any other Godot sound. Then:

- **The bus's effects and volume apply to FMOD's sound.** Put a limiter, a reverb or an equalizer on the bus, or turn it
  down in Godot's Audio panel, and FMOD's sound follows.
- **The "Mute Game" button of Godot's Game view mutes FMOD** too, when you run your game from the editor.
- **Godot's Movie Maker mode records FMOD's sound.** Without it, a movie recorded with `--write-movie` has no FMOD sound.
- **Godot's audio speed applies to FMOD's sound.** Slowing Godot's audio down with
  `AudioServer.playback_speed_scale`, as a slow-motion effect, slows FMOD's sound down and lowers its pitch.
  `Engine.time_scale` does not touch it.

Banks, events, parameters, emitters and listeners work exactly as before. The cons: FMOD's sound is mixed down to
stereo, the output device is Godot's (choose it in Godot's audio settings), and it is not available on the web, where
FMOD always plays to the device.

## Godot Bus Mixing

### Mix Ahead

FMOD prepares its sound on its own thread, a little ahead of Godot. This is the default.

- **Pros:** a heavy FMOD scene, or a moment where FMOD's work takes longer, does not make Godot's audio crackle.
- **Cons:** about 40 ms more delay than *On Demand*. Rarely noticeable, but a rhythm game would feel it. In Movie Maker,
  FMOD's sound comes about 40 ms late, and below about 20 frames per second some of it is lost.

### On Demand

FMOD prepares its sound exactly when Godot asks for it, so FMOD's time and Godot's audio time are the same.

- **Pros:** the shortest delay, 10 to 20 ms from FMOD to Godot's audio. In a Movie Maker recording, FMOD's sound matches
  the frames at any frame rate, whatever hiccups the game has.
- **Cons:** FMOD's work happens inside Godot's own audio work, so a very heavy FMOD scene can make the sound crackle on
  a slow computer.

## Studio Processing

### Background Thread

FMOD Studio works on its own thread, as FMOD does by default. This is the default.

- **Pros:** FMOD Studio's work costs your frames nothing, and keeps going smoothly when a frame takes long.
- **Cons:** a request can wait up to 20 ms for FMOD's thread before it is carried out.

### Main Thread

FMOD Studio does its work inside `FmodServer.update()`, on Godot's main thread.

- **Pros:** requests are carried out the moment `FmodServer.update()` runs, without waiting for FMOD's thread. You
  decide when FMOD Studio works: it does nothing between two calls of `FmodServer.update()`.
- **Cons:** FMOD Studio's work is added to your frame time: with many events playing, frames take longer.

## Recipes

### I want it to just work

Keep the defaults: *Direct to Device* and *Background Thread*.

### I want to avoid stutter and crackles

- Keep *Background Thread*, and with a Godot bus, *Mix Ahead*.
- If FMOD's sound still crackles, raise **Fmod → DSP → DSP Buffer Count** (or **DSP Buffer Size**) to keep more sound
  ready, at the cost of a longer delay.
- With a Godot bus, raising **Audio → Driver → Output Latency** gives Godot's audio more margin too.

### I want Godot's buses, effects and Mute Game to apply to FMOD

*Godot Bus* with *Mix Ahead*. Choose the bus in **Godot Audio Bus**.

### I want to record videos with Movie Maker

*Godot Bus* with *On Demand*: FMOD's sound stays in step with the recorded frames, however slowly the game renders.

### I want the shortest delay

For a rhythm game, or anything where a sound must follow an input as closely as possible:

1. *Godot Bus* with *On Demand* (or *Direct to Device* with a smaller **DSP Buffer Size**, such as 256).
2. *Main Thread* Studio Processing.
3. Trigger time-critical sounds from input handling or `_physics_process()`, which reach FMOD in the same frame, or call
   `FmodServer.update()` yourself right after a request made in `_process()`:

```gdscript
func _process(delta: float) -> void:
    if beat_reached():
        FmodServer.play_one_shot("event:/Drum/Hit")
        FmodServer.update()
```

- **Pros:** you save the wait for the next frame (step 1 of [Where the delay comes from](#where-the-delay-comes-from))
  and the wait for FMOD's thread (step 2). With *Background Thread*, calling `FmodServer.update()` still saves the
  first wait.
- **Cons:** every extra call costs FMOD's update work again, on the main thread. Call it after a group of changes that
  belong together, such as moving an emitter and its listener, not in the middle of it: FMOD would otherwise play with
  only part of them for a moment. Smaller DSP buffers and *On Demand* leave less margin, so a busy scene is more likely
  to crackle.

### I want more control over when FMOD works

*Main Thread* Studio Processing: FMOD Studio only works when `FmodServer.update()` runs. `FmodManager` calls it once
per frame; call it more often, for example after your game logic, when you need requests carried out sooner. The
drawback is the same as above: FMOD Studio's work becomes part of your frame time.

## How each choice reacts

| When... | Direct to Device | Godot Bus, On Demand | Godot Bus, Mix Ahead |
|---|---|---|---|
| The game runs slowly, or stutters | Not affected | Not affected | Not affected |
| `Engine.time_scale` is changed | Not affected | Not affected | Not affected |
| `AudioServer.playback_speed_scale` is changed | Not affected | Slows down or speeds up, with its pitch | Slows down or speeds up, with its pitch |
| A very busy scene plays (300 extra sounds in our tests) | Not affected | Not affected | Not affected |
| Godot's output latency is raised (200 ms in our tests) | Not affected | Not affected | Not affected |
| A Movie Maker movie is recorded | Not recorded | In step with the frames at any frame rate | In step above about 20 frames per second, about 40 ms late; below that, some sound is lost |

## Sample rate

FMOD and Godot each mix sound at their own sample rate: 48000 Hz for FMOD and 44100 Hz for Godot by default. When they
differ, Godot converts FMOD's sound, which costs a little sound quality and a very small delay.

**Sync to Godot Mix Rate** avoids that: when it is on, FMOD uses Godot's rate (**Project Settings → Audio → Driver →
Mix Rate**) and ignores the *Sample Rate* of **Fmod → Software Format**. It only matters with a Godot bus.
