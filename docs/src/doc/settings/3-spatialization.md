# Spatialization

How sounds are placed around the listeners, in 2D and in 3D.

![The Spatialization section](assets/settings-spatialization.png)

## Listener Count

| **Project Setting** | `Fmod/Spatialization/listener_count` |
|---|---|
| **Property** | `listener_count` |
| **Default** | 1 |
| **Values** | 1 to 8 |

The number of listeners FMOD starts with. Use more than one for split-screen games, where each player hears the sounds
around their own camera. See [Listeners](../user-guide/6-listeners.md).

## Distance Factor 2D

| **Project Setting** | `Fmod/Spatialization/distance_factor_2d` |
|---|---|
| **Property** | `distance_factor_2d` |
| **Default** | 64 |

The number of pixels in one meter of your 2D world. FMOD Studio sets the distances at which events fade in meters, and
this setting converts the positions of 2D nodes and sounds to them. Set it to match your art, such as 64 if a character
1.8 meters tall is 115 pixels tall. If every pixel counted as a meter, sounds would fade out far too quickly.

## Distance Factor 3D

| **Project Setting** | `Fmod/Spatialization/distance_factor_3d` |
|---|---|
| **Property** | `distance_factor_3d` |
| **Default** | 1 |

The number of 3D units in one meter. Godot's 3D units are already meters: keep it at 1 unless your world uses another
scale.

The two factors are independent, so a project can mix 2D and 3D nodes and have correct distances in both.

## Rolloff Scale

| **Project Setting** | `Fmod/Spatialization/rolloff_scale` |
|---|---|
| **Property** | `rolloff_scale` |
| **Default** | 1 |

Scales how fast sounds fade with distance, for sounds using one of FMOD's built-in rolloff curves rather than a custom
one. Above 1, sounds fade sooner; below 1, they carry further.

## Doppler Scale

| **Project Setting** | `Fmod/Spatialization/doppler_scale` |
|---|---|
| **Property** | `doppler_scale` |
| **Default** | 1 |

Scales the pitch change of sounds moving toward or away from a listener, for events with Doppler enabled in FMOD
Studio. 0 turns it off, 2 doubles it.

## Changing them while the game runs

Rolloff Scale and Doppler Scale, and the two Distance Factors, can change after FMOD has started, for example per level:

```gdscript
FmodServer.set_spatialization(1.0, 1.0)  # Doppler Scale, Rolloff Scale
FmodServer.set_distance_factors(64.0, 1.0)  # Distance Factor 2D, Distance Factor 3D
```
