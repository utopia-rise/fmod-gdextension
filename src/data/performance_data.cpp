#include "performance_data.h"

using namespace godot;

void FmodCPUUsage::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_dsp"), &FmodCPUUsage::get_dsp);
    ClassDB::bind_method(D_METHOD("get_geometry"), &FmodCPUUsage::get_geometry);
    ClassDB::bind_method(D_METHOD("get_stream"), &FmodCPUUsage::get_stream);
    ClassDB::bind_method(D_METHOD("get_update"), &FmodCPUUsage::get_update);
    ClassDB::bind_method(D_METHOD("get_convolution1"), &FmodCPUUsage::get_convolution1);
    ClassDB::bind_method(D_METHOD("get_convolution2"), &FmodCPUUsage::get_convolution2);

    ClassDB::bind_method(D_METHOD("get_currently_allocated"), &FmodCPUUsage::get_currently_allocated);
    ClassDB::bind_method(D_METHOD("get_max_allocated"), &FmodCPUUsage::get_max_allocated);

    ClassDB::bind_method(D_METHOD("get_sample_bytes_read"), &FmodCPUUsage::get_sample_bytes_read);
    ClassDB::bind_method(D_METHOD("get_stream_bytes_read"), &FmodCPUUsage::get_stream_bytes_read);
    ClassDB::bind_method(D_METHOD("get_other_bytes_read"), &FmodCPUUsage::get_other_bytes_read);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dsp", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_dsp");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "geometry", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_geometry");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "stream", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_stream");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "update", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_update");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "convolution1", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_convolution1");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "convolution2", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_convolution2");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "studio", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_studio");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "currently_allocated", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_currently_allocated");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "max_allocated", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_max_allocated");

    ADD_PROPERTY(PropertyInfo(Variant::INT, "sample_bytes_read", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_sample_bytes_read");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "stream_bytes_read", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_stream_bytes_read");
    ADD_PROPERTY(PropertyInfo(Variant::INT, "other_bytes_read", PropertyHint::PROPERTY_HINT_NONE, "", PROPERTY_USAGE_READ_ONLY), "", "get_other_bytes_read");
}

float FmodCPUUsage::get_dsp() const {
    return dsp;
}
float FmodCPUUsage::get_geometry() const {
    return geometry;
}
float FmodCPUUsage::get_stream() const {
    return stream;
}
float FmodCPUUsage::get_update() const {
    return update;
}
float FmodCPUUsage::get_convolution1() const {
    return convolution1;
}
float FmodCPUUsage::get_convolution2() const {
    return convolution2;
}
float FmodCPUUsage::get_studio() const {
    return studio;
}

int FmodCPUUsage::get_currently_allocated() const {
    return currently_allocated;
}
int FmodCPUUsage::get_max_allocated() const {
    return max_allocated;
}

int FmodCPUUsage::get_sample_bytes_read() const {
    return sample_bytes_read;
}
int FmodCPUUsage::get_stream_bytes_read() const {
    return stream_bytes_read;
}
int FmodCPUUsage::get_other_bytes_read() const {
    return other_bytes_read;
}
