#ifndef GODOTFMOD_FMOD_AUDIO_STREAM_H
#define GODOTFMOD_FMOD_AUDIO_STREAM_H

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

    class FmodAudioStream : public AudioStream {
        GDCLASS(FmodAudioStream, AudioStream)

    public:
        Ref<AudioStreamPlayback> _instantiate_playback() const override;
        String _get_stream_name() const override;

    protected:
        static void _bind_methods();
    };
} // namespace godot

#endif // GODOTFMOD_FMOD_AUDIO_STREAM_H
