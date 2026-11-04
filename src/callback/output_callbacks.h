#ifndef GODOTFMOD_OUTPUT_CALLBACKS_H
#define GODOTFMOD_OUTPUT_CALLBACKS_H

#include <fmod_common.h>

#include <classes/audio_frame.hpp>

namespace Callbacks {
    FMOD_OUTPUT_DESCRIPTION* get_godot_output_description(bool p_synchronous);

    void mix_godot_output(godot::AudioFrame* p_buffer, int p_frames);
    int get_godot_output_rate();
} // namespace Callbacks

#endif // GODOTFMOD_OUTPUT_CALLBACKS_H
