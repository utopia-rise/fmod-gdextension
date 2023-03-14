#ifndef GODOTFMOD_PERFORMANCE_DATA_H
#define GODOTFMOD_PERFORMANCE_DATA_H

#include "classes/ref_counted.hpp"

namespace godot {
    class FmodPerformanceData : public RefCounted {
        GDCLASS(FmodPerformanceData, RefCounted);

        float dsp = 0;
        float geometry = 0;
        float stream = 0;
        float update = 0;
        float convolution1 = 0;
        float convolution2 = 0;
        float studio = 0;

        int currently_allocated = 0;
        int max_allocated = 0;

        int sample_bytes_read = 0;
        int stream_bytes_read = 0;
        int other_bytes_read = 0;

    public:
        float get_geometry() const;
        float get_stream() const;
        float get_update() const;
        float get_convolution1() const;
        float get_convolution2() const;
        float get_studio() const;
        float get_dsp() const;

        int get_currently_allocated() const;
        int get_max_allocated() const;

        int get_sample_bytes_read() const;
        int get_stream_bytes_read() const;
        int get_other_bytes_read() const;

    protected:
        static void _bind_methods();

    };

}// namespace godot

#endif// GODOTFMOD_PERFORMANCE_DATA_H
