#include "output_callbacks.h"

#include <cstdio>
#include <cstring>
#include <templates/local_vector.hpp>
#include <templates/mutex.hpp>

using namespace godot;

#ifdef WEB_ENABLED
void Callbacks::mix_godot_output(AudioFrame* p_buffer, int p_frames) {
    memset(p_buffer, 0, p_frames * sizeof(AudioFrame));
}

int Callbacks::get_godot_output_rate() {
    return 44100;
}
#else
// Synchronous output: FMOD mixes only when Godot's audio thread asks the stream playing its output for frames, so FMOD's
// time follows Godot's mix, which Movie Maker mode needs. Buffered output: FMOD mixes ahead on its own thread and Godot
// only copies the result. FMOD gives whole DSP blocks, kept here until Godot has read them.
static Mutex output_mutex;
static FMOD_OUTPUT_STATE* running_output = nullptr;
static int output_rate = 0;
static LocalVector<float> block;
static uint32_t block_position = 0;

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
    *speakermode = FMOD_SPEAKERMODE_STEREO;
    *speakermodechannels = 2;
    *outputformat = FMOD_SOUND_FORMAT_PCMFLOAT;

    MutexLock lock(output_mutex);
    output_rate = *outputrate;
    block.resize(dspbufferlength * 2);
    block_position = dspbufferlength;
    return FMOD_OK;
}

static FMOD_RESULT F_CALL start(FMOD_OUTPUT_STATE* output_state) {
    MutexLock lock(output_mutex);
    running_output = output_state;
    return FMOD_OK;
}

static FMOD_RESULT F_CALL stop(FMOD_OUTPUT_STATE* output_state) {
    MutexLock lock(output_mutex);
    running_output = nullptr;
    return FMOD_OK;
}

FMOD_OUTPUT_DESCRIPTION* Callbacks::get_godot_output_description(bool p_synchronous) {
    static FMOD_OUTPUT_DESCRIPTION description = {
        FMOD_OUTPUT_PLUGIN_VERSION,
        "Godot",
        1,
        FMOD_OUTPUT_METHOD_MIX_DIRECT,
        get_num_drivers,
        get_driver_info,
        init,
        start,
        stop,
    };
    description.method = p_synchronous ? FMOD_OUTPUT_METHOD_MIX_DIRECT : FMOD_OUTPUT_METHOD_MIX_BUFFERED;
    return &description;
}

void Callbacks::mix_godot_output(AudioFrame* p_buffer, int p_frames) {
    MutexLock lock(output_mutex);
    if (running_output == nullptr) {
        memset(p_buffer, 0, p_frames * sizeof(AudioFrame));
        return;
    }

    uint32_t block_length = block.size() / 2;
    for (int i = 0; i < p_frames; ++i) {
        if (block_position == block_length) {
            if (running_output->readfrommixer(running_output, block.ptr(), block_length) != FMOD_OK) {
                memset(block.ptr(), 0, block.size() * sizeof(float));
            }
            block_position = 0;
        }
        p_buffer[i].left = block[block_position * 2];
        p_buffer[i].right = block[block_position * 2 + 1];
        ++block_position;
    }
}

int Callbacks::get_godot_output_rate() {
    MutexLock lock(output_mutex);
    return output_rate;
}
#endif
