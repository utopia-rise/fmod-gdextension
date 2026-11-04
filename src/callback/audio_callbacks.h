#ifndef GODOTFMOD_AUDIO_CALLBACKS_H
#define GODOTFMOD_AUDIO_CALLBACKS_H

#include <fmod_common.h>

#include <classes/audio_frame.hpp>
#include <classes/audio_stream.hpp>
#include <classes/audio_stream_playback_resampled.hpp>

namespace godot {
    class FmodAudioStreamPlayback : public AudioStreamPlaybackResampled {
        GDCLASS(FmodAudioStreamPlayback, AudioStreamPlaybackResampled)

        bool playing = false;

    public:
        void _start(double p_from_pos) override;
        void _stop() override;
        bool _is_playing() const override;
        double _get_playback_position() const override;
        int32_t _mix_resampled(AudioFrame* p_dst_buffer, int32_t p_frame_count) override;
        float _get_stream_sampling_rate() const override;

    protected:
        static void _bind_methods();
    };

    // Plays FMOD's output on a Godot bus, internal to FmodRuntime's player. FmodServer::init makes get_fmod_output()
    // FMOD's output, which this stream then reads.
    class FmodAudioStream : public AudioStream {
        GDCLASS(FmodAudioStream, AudioStream)

    public:
        static FMOD_OUTPUT_DESCRIPTION* get_fmod_output(bool p_on_demand);

        Ref<AudioStreamPlayback> _instantiate_playback() const override;
        String _get_stream_name() const override;

    protected:
        static void _bind_methods();
    };
} // namespace godot

#endif // GODOTFMOD_AUDIO_CALLBACKS_H
