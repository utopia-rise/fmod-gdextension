#include "fmod_audio_stream.h"

#include <callback/output_callbacks.h>

using namespace godot;

void FmodAudioStreamPlayback::_start(double p_from_pos) {
    playing = true;
    begin_resample();
}

void FmodAudioStreamPlayback::_stop() {
    playing = false;
}

bool FmodAudioStreamPlayback::_is_playing() const {
    return playing;
}

double FmodAudioStreamPlayback::_get_playback_position() const {
    return 0;
}

int32_t FmodAudioStreamPlayback::_mix_resampled(AudioFrame* p_dst_buffer, int32_t p_frame_count) {
    Callbacks::mix_godot_output(p_dst_buffer, p_frame_count);
    return p_frame_count;
}

float FmodAudioStreamPlayback::_get_stream_sampling_rate() const {
    return Callbacks::get_godot_output_rate();
}

void FmodAudioStreamPlayback::_bind_methods() {}

Ref<AudioStreamPlayback> FmodAudioStream::_instantiate_playback() const {
    Ref<FmodAudioStreamPlayback> playback;
    playback.instantiate();
    return playback;
}

String FmodAudioStream::_get_stream_name() const {
    return "FMOD";
}

void FmodAudioStream::_bind_methods() {}
