#include "fmod_sound.h"

#include "fmod_server.h"
#include "helpers/common.h"
#include "helpers/maths.h"

using namespace godot;

void FmodSound::_bind_methods() {
    ClassDB::bind_method(D_METHOD("is_valid"), &FmodSound::is_valid);
    ClassDB::bind_method(D_METHOD("release"), &FmodSound::release);
    ClassDB::bind_method(D_METHOD("play"), &FmodSound::play);
    ClassDB::bind_method(D_METHOD("stop"), &FmodSound::stop);
    ClassDB::bind_method(D_METHOD("set_paused", "paused"), &FmodSound::set_paused);
    ClassDB::bind_method(D_METHOD("is_playing"), &FmodSound::is_playing);
    ClassDB::bind_method(D_METHOD("set_volume", "volume"), &FmodSound::set_volume);
    ClassDB::bind_method(D_METHOD("get_volume"), &FmodSound::get_volume);
    ClassDB::bind_method(D_METHOD("set_pitch", "pitch"), &FmodSound::set_pitch);
    ClassDB::bind_method(D_METHOD("get_pitch"), &FmodSound::get_pitch);
    ClassDB::bind_method(D_METHOD("set_position", "position"), &FmodSound::set_position);
    ClassDB::bind_method(D_METHOD("get_position"), &FmodSound::get_position);
    ClassDB::bind_method(D_METHOD("set_velocity", "velocity"), &FmodSound::set_velocity);
    ClassDB::bind_method(D_METHOD("get_velocity"), &FmodSound::get_velocity);
    ClassDB::bind_method(D_METHOD("set_position_2d", "position"), &FmodSound::set_position_2d);
    ClassDB::bind_method(D_METHOD("get_position_2d"), &FmodSound::get_position_2d);
    ClassDB::bind_method(D_METHOD("set_velocity_2d", "velocity"), &FmodSound::set_velocity_2d);
    ClassDB::bind_method(D_METHOD("get_velocity_2d"), &FmodSound::get_velocity_2d);

    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_pitch", "get_pitch");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "volume",PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_volume", "get_volume");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "position", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_position", "get_position");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "velocity", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_velocity", "get_velocity");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "position_2d", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_position_2d", "get_position_2d");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "velocity_2d", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NONE), "set_velocity_2d", "get_velocity_2d");
}

FMOD_3D_ATTRIBUTES FmodSound::get_3d_attributes() const {
    FMOD_3D_ATTRIBUTES attributes = {};
    ERROR_CHECK(_wrapped->get3DAttributes(&attributes.position, &attributes.velocity));
    return attributes;
}

// A channel only spatializes in 3D mode, and files load in 2D: placing the sound switches it.
void FmodSound::set_3d_attributes(FMOD_3D_ATTRIBUTES& attributes) const {
    FMOD_MODE mode = 0;
    if (ERROR_CHECK(_wrapped->getMode(&mode)) && !(mode & FMOD_3D)) {
        ERROR_CHECK(_wrapped->setMode((mode & ~FMOD_2D) | FMOD_3D));
    }
    ERROR_CHECK(_wrapped->set3DAttributes(&attributes.position, &attributes.velocity));
}

void FmodSound::set_position(const Vector3& position) const {
    FMOD_3D_ATTRIBUTES attributes = get_3d_attributes();
    attributes.position = get_fmod_vector_from_3d(position / FmodServer::get_singleton()->get_distance_scale());
    set_3d_attributes(attributes);
}

Vector3 FmodSound::get_position() const {
    FMOD_3D_ATTRIBUTES attributes = get_3d_attributes();
    return Vector3(attributes.position.x, attributes.position.y, attributes.position.z) * FmodServer::get_singleton()->get_distance_scale();
}

void FmodSound::set_velocity(const Vector3& velocity) const {
    FMOD_3D_ATTRIBUTES attributes = get_3d_attributes();
    attributes.velocity = get_fmod_vector_from_3d(velocity / FmodServer::get_singleton()->get_distance_scale());
    set_3d_attributes(attributes);
}

Vector3 FmodSound::get_velocity() const {
    FMOD_3D_ATTRIBUTES attributes = get_3d_attributes();
    return Vector3(attributes.velocity.x, attributes.velocity.y, attributes.velocity.z) * FmodServer::get_singleton()->get_distance_scale();
}

void FmodSound::set_position_2d(const Vector2& position) const {
    set_position(Vector3(-position.x, 0.0f, position.y));
}

Vector2 FmodSound::get_position_2d() const {
    Vector3 position = get_position();
    return {-position.x, position.z};
}

void FmodSound::set_velocity_2d(const Vector2& velocity) const {
    set_velocity(Vector3(-velocity.x, 0.0f, velocity.y));
}

Vector2 FmodSound::get_velocity_2d() const {
    Vector3 velocity = get_velocity();
    return {-velocity.x, velocity.z};
}

void FmodSound::release() const {
    ERROR_CHECK(_wrapped->stop());
}

void FmodSound::play() const {
    set_paused(false);
}

void FmodSound::set_paused(bool paused) const {
    ERROR_CHECK(_wrapped->setPaused(paused));
}

void FmodSound::stop() const {
    ERROR_CHECK(_wrapped->stop());
}

bool FmodSound::is_playing() const {
    bool isPlaying = false;
    ERROR_CHECK(_wrapped->isPlaying(&isPlaying));
    return isPlaying;
}

void FmodSound::set_volume(float volume) const {
    ERROR_CHECK(_wrapped->setVolume(volume));
}

float FmodSound::get_volume() const {
    float volume = 0.f;
    ERROR_CHECK(_wrapped->getVolume(&volume));
    return volume;
}

float FmodSound::get_pitch() const {
    float pitch = 0.f;
    ERROR_CHECK(_wrapped->getPitch(&pitch));
    return pitch;
}

void FmodSound::set_pitch(float pitch) {
    ERROR_CHECK(_wrapped->setPitch(pitch));
}

bool FmodSound::is_valid() const {
    bool isPlaying;
    FMOD_RESULT result = _wrapped->isPlaying(&isPlaying);
    return result != FMOD_ERR_INVALID_HANDLE;
}