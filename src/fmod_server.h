#ifndef GODOTFMOD_FMOD_SERVER_H
#define GODOTFMOD_FMOD_SERVER_H

#include "fmod_cache.h"
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

    class FmodServer : public Object {
        GDCLASS(FmodServer, Object);

        static FmodServer* singleton;

    private:
        FMOD::Studio::System* system;
        FMOD::System* coreSystem;

        bool isInitialized;
        bool isNotinitPrinted;

        float distanceScale;

        FmodCache* cache;

        int systemListenerNumber = 1;
        int actualListenerNumber = 0;
        Listener listeners[FMOD_MAX_LISTENERS];
        bool listenerWarning = true;

        List<FMOD::Studio::EventInstance*> events;

        List<FMOD::Channel*> channels;
        HashMap<String, FMOD::Sound*> sounds;

        // Store dictionary of performance data
        Ref<FmodPerformanceData> performanceData;

        void _set_listener_attributes();

        void _update_instance_3d_attributes(FMOD::Studio::EventInstance* instance, Object* node) const;

        FMOD::Studio::EventInstance* _create_instance(const String& eventName, bool isOneShot, Object* gameObject);
        void _release_one_event(FMOD::Studio::EventInstance* eventInstance);

        void _update_performance_data();

        static EventInfo* _get_event_info(FMOD::Studio::EventInstance* eventInstance);
        static bool _is_channel_valid(FMOD::Channel* channel);

    public:
        FmodServer();

        ~FmodServer();

        static FmodServer* get_singleton();

        //LIFECYCLE
        void init(int numOfChannels, unsigned int studioFlag, unsigned int flag);
        void update();
        void shutdown();

        // SETTINGS
        void set_software_format(int sampleRate, int speakerMode, int numRawSpeakers);
        void set_sound_3d_settings(float dopplerScale, float distanceFactor, float rollOffScale);
        void set_system_dsp_buffer_size(unsigned int bufferlength, int numbuffers);
        Array get_system_dsp_buffer_size();
        unsigned int get_system_dsp_buffer_length();
        int get_system_dsp_num_buffers();

        // INFORMATION
        bool check_vca_path(const String& vcaPath);
        bool check_bus_path(const String& busPath);
        bool check_event_path(const String& eventPath);
        Array get_available_drivers();
        int get_driver();
        void set_driver(int id);
        Ref<FmodPerformanceData> get_performance_data();
        void set_global_parameter_by_name(const String& parameterName, float value);
        float get_global_parameter_by_name(const String& parameterName);
        void set_global_parameter_by_id(const Array& idPair, float value);
        float get_global_parameter_by_id(const Array& idPair);
        Dictionary get_global_parameter_desc_by_name(const String& parameterName);
        Dictionary get_global_parameter_desc_by_id(const Array& idPair);
        int get_global_parameter_desc_count();
        Array get_global_parameter_desc_list();

        // LISTENERS
        void add_listener(int index, Object* gameObj);
        void remove_listener(int index);
        void set_system_listener_number(int listenerNumber);
        int get_system_listener_number() const;
        float get_system_listener_weight(int index);
        void set_system_listener_weight(int index, float weight);
        Transform3D get_listener_3d_transform(int index);
        Transform2D get_listener_2d_transform(int index);
        Vector3 get_listener_3d_velocity(int index);
        Vector2 get_listener_2d_velocity(int index);
        void set_system_listener_3d_attributes(int index, const Transform3D& transform);
        void set_system_listener_2d_attributes(int index, const Transform2D& transform);
        void set_listener_lock(int index, bool isLocked);
        bool get_listener_lock(int index);
        Object* get_object_attached_to_listener(int index);

        // OBJECTS
        Ref<FmodBank> load_bank(const String& pathToBank, unsigned int flag);
        void unload_bank(const String& pathToBank);
        Ref<FmodEvent> create_event_instance(const String& eventPath);
        static void attach_instance_to_node(Ref<FmodEvent> event, Object* gameObj) ;
        static void detach_instance_from_node(Ref<FmodEvent> event) ;
        static Object* get_object_attached_to_instance(Ref<FmodEvent> event) ;

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
        Ref<FmodSound> create_sound_instance(const String& path);

        // Setting the sound

        // Playing a sound
        void play_sound(const uint64_t instanceId);

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_FMOD_SERVER_H
