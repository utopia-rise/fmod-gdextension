# Output, latency and sync modes

Three settings decide where FMOD's sound goes and how quickly a sound you trigger reaches the speakers:

- **Output → Routing:** FMOD plays to the speakers on its own, or through one of Godot's audio buses.
- **Godot Bus Mixing:** with a Godot bus, FMOD prepares its sound ahead of time, or only when Godot asks for it.
- **Studio Processing:** FMOD Studio carries out your requests on its own thread, or during `FmodServer.update()`.

The defaults suit most games. This page explains what each choice changes, and which ones to pick to avoid stutter, to
get the shortest delay, or to record videos.

## Where the delay comes from

Between the moment your code plays an event and the moment it reaches the speakers, the request goes through these
steps:

1. **It waits for `FmodServer.update()`.** FMOD receives your requests when `FmodServer.update()` runs. The
   `FmodRuntime` node FMOD adds to the scene tree calls it once per frame, before any other node's `_process()`. Requests
   made while handling input or in `_physics_process()` reach FMOD in the same frame; requests made in your nodes'
   `_process()` wait for the next frame, so this wait depends on your frame rate.
2. **FMOD Studio carries it out.** With **Studio Processing** on *Background Thread*, FMOD's own thread picks it up
   on its next update, which runs on a timer of its own. On *Main Thread*, it happens right away, inside
   `FmodServer.update()`.
3. **FMOD mixes it.** FMOD produces sound in small blocks of **Fmod → Mixer → Latency → Buffer Size** samples: the larger
   the blocks, the longer each one lasts.
4. **The sound waits to be played.** Directly to the device, FMOD keeps a few blocks ready (**Buffer Count**, 4 by
   default). Through a Godot bus, *Mix Ahead* keeps all of FMOD's buffers ready, Buffer Size × Buffer Count samples,
   *On Demand* none, and Godot adds its own output delay (**Audio → Driver → Output Latency**).

Each step is a trade: the more sound is prepared in advance, the less likely the sound is to stutter, and the later it
is heard.

## The settings

They are all in **Project Settings → Fmod → Mixer**, in the *Output* and *Latency* groups:
[Routing](../settings/2-mixer.md#routing), [Godot Audio Bus](../settings/2-mixer.md#godot-audio-bus),
[Godot Bus Mixing](../settings/2-mixer.md#godot-bus-mixing),
[Sync to Godot Mix Rate](../settings/2-mixer.md#sync-to-godot-mix-rate), [Buffer Size](../settings/2-mixer.md#buffer-size),
[Buffer Count](../settings/2-mixer.md#buffer-count) and [Studio Processing](../settings/2-mixer.md#studio-processing).
[Settings → Mixer](../settings/2-mixer.md) describes each one; this page explains how they work together.

Restart your game after changing them: they are read when the game starts. If you start FMOD yourself from a script
(see [Initialization](2-initialization.md)), set them on the settings you give it:

```gdscript
var settings := FmodSettings.new()
settings.output_routing = FmodSettings.OutputRouting.GODOT_BUS
settings.godot_audio_bus = "SFX"
settings.godot_bus_mixing = FmodSettings.GodotBusMixing.ON_DEMAND
settings.studio_processing = FmodSettings.StudioProcessing.MAIN_THREAD
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
- **Cons:** more delay than *On Demand*: Buffer Size × Buffer Count samples instead of at most one block. Larger buffers
  make it longer; a rhythm game would feel it. In Movie Maker, FMOD's sound comes that much late, and when one movie
  frame needs more samples than the buffers hold (a low frame rate, or small buffers), some of it is lost.

### On Demand

FMOD prepares its sound exactly when Godot asks for it, so FMOD's time and Godot's audio time are the same.

- **Pros:** the shortest delay: at most one block of Buffer Size samples from FMOD to Godot's audio. In a Movie Maker recording, FMOD's sound matches
  the frames at any frame rate, whatever hiccups the game has.
- **Cons:** FMOD's work happens inside Godot's own audio work, so a very heavy FMOD scene can make the sound crackle on
  a slow computer.

## Studio Processing

### Background Thread

FMOD Studio works on its own thread, as FMOD does by default. This is the default.

- **Pros:** FMOD Studio's work costs your frames nothing, and keeps going smoothly when a frame takes long.
- **Cons:** a request waits for the next update of FMOD's thread before it is carried out.

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
- If FMOD's sound still crackles, raise **Fmod → Mixer → Latency → Buffer Count** (or **Buffer Size**) to keep more sound
  ready, at the cost of a longer delay.
- With a Godot bus, raising **Audio → Driver → Output Latency** gives Godot's audio more margin too.

### I want Godot's buses, effects and Mute Game to apply to FMOD

*Godot Bus* with *Mix Ahead*. Choose the bus in **Godot Audio Bus**.

### I want to record videos with Movie Maker

*Godot Bus* with *On Demand*: FMOD's sound stays in step with the recorded frames, however slowly the game renders.

### I want the shortest delay

For a rhythm game, or anything where a sound must follow an input as closely as possible:

1. *Godot Bus* with *On Demand* (or *Direct to Device* with a smaller **Buffer Size**, such as 256).
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

*Main Thread* Studio Processing: FMOD Studio only works when `FmodServer.update()` runs. `FmodRuntime` calls it once
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
| A Movie Maker movie is recorded | Not recorded | In step with the frames at any frame rate | Late by the size of the buffers; some sound is lost when a frame needs more samples than the buffers hold |

## Sample rate

FMOD and Godot each mix sound at their own sample rate: 48000 Hz for FMOD and 44100 Hz for Godot by default. When they
differ, Godot converts FMOD's sound, which costs a little sound quality and a very small delay.

**Sync to Godot Mix Rate** avoids that: when it is on, FMOD uses Godot's rate (**Project Settings → Audio → Driver →
Mix Rate**) and ignores the *Sample Rate* of **Fmod → Mixer → Format**. It only matters with a Godot bus.
