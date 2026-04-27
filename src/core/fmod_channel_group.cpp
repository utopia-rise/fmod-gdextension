#include "fmod_channel_group.h"

#include "helpers/common.h"

using namespace godot;

void FmodChannelGroup::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_dsp_clock"), &FmodChannelGroup::get_dsp_clock);
    ClassDB::bind_method(D_METHOD("get_parent_dsp_clock"), &FmodChannelGroup::get_parent_dsp_clock);
}


uint64_t FmodChannelGroup::get_dsp_clock() const {
    unsigned long long dspclock = 0;
    unsigned long long parentclock = 0;
    ERROR_CHECK(_wrapped->getDSPClock(&dspclock, &parentclock));
    return (uint64_t) dspclock; // godot does not support unsigned ints?
}

uint64_t FmodChannelGroup::get_parent_dsp_clock() const {
    unsigned long long dspclock = 0;
    unsigned long long parentclock = 0;
    ERROR_CHECK(_wrapped->getDSPClock(&dspclock, &parentclock));
    return (uint64_t) parentclock; // godot does not support unsigned ints?
}