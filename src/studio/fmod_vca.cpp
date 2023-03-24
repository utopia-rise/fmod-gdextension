#include "fmod_vca.h"

#include "helpers/common.h"

using namespace godot;

void FmodVCA::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_volume"), &FmodVCA::get_vca_volume);
    ClassDB::bind_method(
      D_METHOD("set_volume"
               "volume"),
      &FmodVCA::set_vca_volume
    );
    ClassDB::bind_method(D_METHOD("is_valid"), &FmodVCA::is_valid);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume"), "set_volume", "get_volume");
}

float FmodVCA::get_vca_volume(const String& VCAPath) {
    float volume = 0.0f;
    ERROR_CHECK(_wrapped->getVolume(&volume));
    return volume;
}

void FmodVCA::set_vca_volume(const String& VCAPath, float volume) {
    ERROR_CHECK(_wrapped->setVolume(volume));
}