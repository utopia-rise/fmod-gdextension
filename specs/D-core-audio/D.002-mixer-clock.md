# D.2: Mixer clock

Game code reads the DSP clock of the mixer an event plays through, to schedule audio and visuals with sample accuracy.

Rhythm games and music-driven gameplay need a clock tied to the audio actually mixed, not to frames. The clock of an event's channel group, and of its parent, gives that time base.

## Capabilities
- **D.2.1**: Game code gets the channel group of an event instance and reads its DSP clock and its parent's DSP clock.
