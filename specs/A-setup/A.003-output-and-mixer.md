# A.3: Output and mixer

Game code chooses the audio output device and tunes the mixer, and the mixer follows the application's life on mobile.

Players expect to pick their output device in an options menu, and some games trade latency against stability by tuning DSP buffers. On mobile, an application sent to the background must stop using the audio device and take it back when it returns.

The settings the mixer starts with are A.2.

## Capabilities
- **A.3.1**: Game code lists the available output drivers with their name, sample rate and speaker mode, and switches FMOD to one of them at run time.
- **A.3.2**: Game code sets the software format and the DSP buffer size before FMOD starts, and reads the DSP buffer settings in use.
- **A.3.3**: On Android and iOS, the mixer suspends when the application loses focus and resumes when it regains it, without game code.
- **A.3.4**: Game code suspends and resumes the mixer itself.
- **A.3.5**: Game developers send FMOD's output to a Godot audio bus instead of the audio device, so that Godot's bus effects and volume apply to it and Godot's Movie Maker mode records it in time with the frames, and choose whether FMOD mixes ahead of Godot, against crackles, or when Godot asks, for the shortest delay.
