#include "audio_callbacks.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <templates/local_vector.hpp>
#include <templates/mutex.hpp>

using namespace godot;

// FMOD's output when it plays on a Godot bus. FmodAudioStream reads FMOD's mix from Godot's audio thread:
// - on demand (MIX_DIRECT), reading runs FMOD's mixer on that thread, so FMOD's time follows Godot's mix, as Movie
//   Maker mode needs;
// - mixed ahead (MIX_BUFFERED), FMOD's own thread keeps its buffers full and reading copies the oldest one.
// FMOD hands out whole DSP blocks: the frames of a block Godot has not asked for yet wait in `block`.
// FMOD calls init, start and stop on its own thread, hence the mutex. Until start, Godot reads silence.
struct GodotOutput {
    Mutex mutex;
    FMOD_OUTPUT_STATE* state = nullptr;
    int rate = 48000;
    LocalVector<AudioFrame> block;
    uint32_t read_position = 0;
};

static GodotOutput output;

static FMOD_RESULT F_CALL get_num_drivers(FMOD_OUTPUT_STATE* output_state, int* numdrivers) {
    *numdrivers = 1;
    return FMOD_OK;
}

static FMOD_RESULT F_CALL get_driver_info(
    FMOD_OUTPUT_STATE* output_state,
    int id,
    char* name,
    int namelen,
    FMOD_GUID* guid,
    int* systemrate,
    FMOD_SPEAKERMODE* speakermode,
    int* speakermodechannels
) {
    snprintf(name, namelen, "Godot");
    *speakermode = FMOD_SPEAKERMODE_STEREO;
    *speakermodechannels = 2;
    return FMOD_OK;
}

static FMOD_RESULT F_CALL init(
    FMOD_OUTPUT_STATE* output_state,
    int selecteddriver,
    FMOD_INITFLAGS flags,
    int* outputrate,
    FMOD_SPEAKERMODE* speakermode,
    int* speakermodechannels,
    FMOD_SOUND_FORMAT* outputformat,
    int dspbufferlength,
    int* dspnumbuffers,
    int* dspnumadditionalbuffers,
    void* extradriverdata
) {
    // Interleaved stereo floats, the layout of Godot's AudioFrame.
    *speakermode = FMOD_SPEAKERMODE_STEREO;
    *speakermodechannels = 2;
    *outputformat = FMOD_SOUND_FORMAT_PCMFLOAT;

    MutexLock lock(output.mutex);
    output.rate = *outputrate;
    output.block.resize(dspbufferlength);
    output.read_position = dspbufferlength;
    return FMOD_OK;
}

static FMOD_RESULT F_CALL start(FMOD_OUTPUT_STATE* output_state) {
    MutexLock lock(output.mutex);
    output.state = output_state;
    return FMOD_OK;
}

static FMOD_RESULT F_CALL stop(FMOD_OUTPUT_STATE* output_state) {
    MutexLock lock(output.mutex);
    output.state = nullptr;
    return FMOD_OK;
}

FMOD_OUTPUT_DESCRIPTION* FmodAudioStream::get_fmod_output(bool p_on_demand) {
    static FMOD_OUTPUT_DESCRIPTION description = {
        FMOD_OUTPUT_PLUGIN_VERSION,
        "Godot",
        1,
        FMOD_OUTPUT_METHOD_MIX_BUFFERED,
        get_num_drivers,
        get_driver_info,
        init,
        start,
        stop,
    };
    description.method = p_on_demand ? FMOD_OUTPUT_METHOD_MIX_DIRECT : FMOD_OUTPUT_METHOD_MIX_BUFFERED;
    return &description;
}

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
    MutexLock lock(output.mutex);
    if (output.state == nullptr) {
        memset(p_dst_buffer, 0, p_frame_count * sizeof(AudioFrame));
        return p_frame_count;
    }

    uint32_t block_length = output.block.size();
    uint32_t mixed = 0;
    while (mixed < static_cast<uint32_t>(p_frame_count)) {
        if (output.read_position == block_length) {
            if (output.state->readfrommixer(output.state, output.block.ptr(), block_length) != FMOD_OK) {
                memset(output.block.ptr(), 0, block_length * sizeof(AudioFrame));
            }
            output.read_position = 0;
        }
        uint32_t count = std::min(p_frame_count - mixed, block_length - output.read_position);
        memcpy(p_dst_buffer + mixed, output.block.ptr() + output.read_position, count * sizeof(AudioFrame));
        output.read_position += count;
        mixed += count;
    }
    return p_frame_count;
}

float FmodAudioStreamPlayback::_get_stream_sampling_rate() const {
    MutexLock lock(output.mutex);
    return output.rate;
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
