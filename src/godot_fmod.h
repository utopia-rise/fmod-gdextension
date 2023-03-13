#ifndef GODOTFMOD_GODOT_FMOD_H
#define GODOTFMOD_GODOT_FMOD_H

#include "studio/fmod_bank.h"
#include "studio/fmod_bus.h"
#include "studio/fmod_event.h"
#include "studio/fmod_event_description.h"
#include "studio/fmod_vca.h"
#include "templates/hash_map.hpp"
#include "variant/string.hpp"
#include <callback/event_callbacks.h>
#include <callback/file_callbacks.h>
#include <classes/canvas_item.hpp>
#include <classes/global_constants.hpp>
#include <classes/mutex.hpp>
#include <classes/object.hpp>
#include <core/object.hpp>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <godot.hpp>
#include <helpers/constants.h>
#include <variant/utility_functions.hpp>

namespace godot {

    struct EventInfo {
        // Is the event oneshot
        bool isOneShot = false;
        // GameObject to which this event is attached
        Object* gameObj = nullptr;
    };

    struct Listener {
        Object* gameObj = nullptr;
        bool listenerLock = false;
        float weight = 1.0;
    };

    class Fmod : public Object {
        GDCLASS(Fmod, Object);

        static Fmod* singleton;

    private:
        FMOD::Studio::System* system;
        FMOD::System* coreSystem;

        bool isInitialized;
        bool isNotinitPrinted;

        float distanceScale;

        int systemListenerNumber = 1;
        int actualListenerNumber = 0;
        Listener listeners[FMOD_MAX_LISTENERS];
        bool listenerWarning = true;

        List<FMOD::Studio::EventInstance*> events;

        List<FMOD::Channel*> channels;
        HashMap<String, FMOD::Sound*> sounds;

        // Store dictionary of performance data
        Dictionary performanceData;

        void _set_listener_attributes();

        void _update_instance_3d_attributes(FMOD::Studio::EventInstance* instance, Object* node);
        void _run_callbacks();

        FMOD::Studio::EventInstance* _create_instance(const String& eventName, bool isOneShot, Object* gameObject);
        static EventInfo* _get_event_info(FMOD::Studio::EventInstance* eventInstance);
        void _release_one_event(FMOD::Studio::EventInstance* eventInstance);
        static bool _is_channel_valid(FMOD::Channel* channel);

    public:
        Fmod();

        ~Fmod();

        static Fmod* get_singleton();

        // SETTINGS
        void update();
        void shutdown();
        void init(int numOfChannels, unsigned int studioFlag, unsigned int flag);
        void set_sound_3d_settings(float dopplerScale, float distanceFactor, float rollOffScale);
        void set_software_format(int sampleRate, int speakerMode, int numRawSpeakers);
        void set_system_dsp_buffer_size(unsigned int bufferlength, int numbuffers);
        unsigned int get_system_dsp_buffer_length();
        int get_system_dsp_num_buffers();
        Array get_system_dsp_buffer_size();

        // LISTENERS
        void add_listener(int index, Object* gameObj);
        void remove_listener(int index);
        void set_system_listener_number(int listenerNumber);
        int get_system_listener_number() const;
        float get_system_listener_weight(int index);
        void set_system_listener_weight(int index, float weight);
        Dictionary get_system_listener_3d_attributes(int index);
        Dictionary get_system_listener_2d_attributes(int index);
        void set_system_listener_3d_attributes(int index, const Transform3D& transform);
        void set_system_listener_2d_attributes(int index, const Transform2D& transform);
        void set_listener_lock(int index, bool isLocked);
        bool get_listener_lock(int index);
        Object* get_object_attached_to_listener(int index);

        // INFORMATION
        bool check_vca_path(const String& vcaPath);
        bool check_bus_path(const String& busPath);
        bool check_event_path(const String& eventPath);
        Array Fmod::get_description_list();
        int Fmod::get_description_count();
        void Fmod::release_all_descriptions();
        void set_global_parameter_by_name(const String& parameterName, float value);
        float get_global_parameter_by_name(const String& parameterName);
        void set_global_parameter_by_id(const Array& idPair, const float value);
        float get_global_parameter_by_id(const Array& idPair);
        Dictionary get_global_parameter_desc_by_name(const String& parameterName);
        Dictionary get_global_parameter_desc_by_id(const Array& idPair);
        int get_global_parameter_desc_count();
        Array get_global_parameter_desc_list();

        // PERFORMANCE
        Array get_available_drivers();
        int get_driver();
        void set_driver(int id);
        Dictionary get_performance_data();

        // OBJECTS
        FmodBank load_bank(const String& pathToBank, unsigned int flag);
        void unload_bank(const String& pathToBank);
        FmodEvent create_event_instance(const String& eventPath);
        void attach_instance_to_node(Object* gameObj) const;
        void detach_instance_from_node() const;
        Object* get_object_attached_to_instance() const;

        /* Helper methods */
        void play_one_shot(const String& eventName, Object* gameObj);
        void play_one_shot_with_params(const String& eventName, Object* gameObj, const Dictionary& parameters);
        void play_one_shot_attached(const String& eventName, Object* gameObj);
        void play_one_shot_attached_with_params(const String& eventName, Object* gameObj, const Dictionary& parameters);
        void pause_all_events(bool pause);
        void mute_all_events();
        void unmute_all_events();
        void wait_for_all_loads();

        // LOW LEVEL API
        // Load and release memory
        void load_file_as_sound(const String& path);
        void load_file_as_music(const String& path);
        void unload_file(const String& path);
        // Check validity of an instance
        uint64_t create_sound_instance(const String& path);
        bool check_sound_instance(const uint64_t instanceId);
        void release_sound(const uint64_t instanceId);
        // Setting the sound

        // Playing a sound
        void play_sound(const uint64_t instanceId);

        static FMOD_VECTOR to_fmod_vector(Vector3& vec) {
            FMOD_VECTOR fv;
            fv.x = vec.x;
            fv.y = vec.y;
            fv.z = vec.z;
            return fv;
        }

        static FMOD_3D_ATTRIBUTES get_3d_attributes(const FMOD_VECTOR& pos, const FMOD_VECTOR& up, const FMOD_VECTOR& forward, const FMOD_VECTOR& vel) {
            FMOD_3D_ATTRIBUTES f3d;
            f3d.forward = forward;
            f3d.position = pos;
            f3d.up = up;
            f3d.velocity = vel;
            return f3d;
        }

        FMOD_3D_ATTRIBUTES get_3d_attributes_from_transform(const Transform3D& transform) const {
            Vector3 pos = transform.get_origin() / distanceScale;
            Vector3 up = transform.get_basis().rows[1];
            Vector3 forward = transform.get_basis().rows[2];
            Vector3 vel(0, 0, 0);
            return get_3d_attributes(to_fmod_vector(pos), to_fmod_vector(up), to_fmod_vector(forward), to_fmod_vector(vel));
        }

        FMOD_3D_ATTRIBUTES get_3d_attributes_from_transform_2d(const Transform2D& transform) const {
            Vector2 posVector = transform.get_origin() / distanceScale;
            Vector3 pos(posVector.x, 0.0f, posVector.y);
            Vector3 up(0, 1, 0);
            Vector3 forward = Vector3(transform.columns[1].x, 0, transform.columns[1].y).normalized();
            Vector3 vel(0, 0, 0);// TODO: add doppler
            const FMOD_VECTOR& posFmodVector = to_fmod_vector(pos);
            return get_3d_attributes(posFmodVector, to_fmod_vector(up), to_fmod_vector(forward), to_fmod_vector(vel));
        }

        Dictionary get_transform_info_from_3d_attributes(FMOD_3D_ATTRIBUTES& attr) const {
            Dictionary _3Dattr;
            Transform3D transform;
            transform.origin = Vector3(attr.position.x, attr.position.y, attr.position.z) * distanceScale;
            const Vector3& upVector = Vector3(attr.up.x, attr.up.y, attr.up.z);
            transform.basis.rows[1] = upVector;
            const Vector3& forwardVector = Vector3(attr.forward.x, attr.forward.y, attr.forward.z);
            transform.basis.rows[2] = forwardVector;
            transform.basis.rows[0] = upVector.cross(forwardVector);
            Vector3 velocity(attr.velocity.x, attr.velocity.y, attr.velocity.z);
            _3Dattr["transform"] = transform;
            _3Dattr["velocity"] = velocity;
            return _3Dattr;
        }

        Dictionary get_transform_2d_info_from_3d_attributes(FMOD_3D_ATTRIBUTES& attr) const {
            Dictionary _2Dattr;
            Transform2D transform;
            transform.set_origin(Vector2(attr.position.x, attr.position.z) * distanceScale);
            const Vector2& forward = Vector2(attr.forward.x, attr.forward.z);
            transform.columns[1] = forward;
            transform.columns[0] = Vector2(forward.y, -forward.x);
            Vector2 velocity(attr.velocity.x, attr.velocity.z);
            _2Dattr["transform"] = transform;
            _2Dattr["velocity"] = velocity;
            return _2Dattr;
        }

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_GODOT_FMOD_H
