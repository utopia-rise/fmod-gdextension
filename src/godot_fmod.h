#ifndef GODOTFMOD_GODOT_FMOD_H
#define GODOTFMOD_GODOT_FMOD_H

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
#include <helpers/containers.h>
#include <variant/utility_functions.hpp>

namespace godot {
    struct EventInfo {
        // Is the event oneshot
        bool isOneShot = false;
        // GameObject to which this event is attached
        Object* gameObj = nullptr;
        // Callback info associated with this event
        Callbacks::CallbackInfo callbackInfo = Callbacks::CallbackInfo();
    };

    struct LoadingBank {
        FMOD::Studio::Bank* bank;
        String godotPath;
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

        Vector<LoadingBank*> loadingBanks;
        Map<String, FMOD::Studio::Bank*> banks;
        Map<String, FMOD::Studio::EventDescription*> eventDescriptions;
        Map<String, FMOD::Sound*> sounds;
        Map<String, FMOD::Studio::Bus*> buses;
        Map<String, FMOD::Studio::VCA*> VCAs;

        Vector<FMOD::Channel*> channels;
        Vector<FMOD::Studio::EventInstance*> events;

        // Store dictionary of performance data
        Dictionary performanceData;

        void _check_loading_banks();
        void _set_listener_attributes();



        void _update_instance_3d_attributes(FMOD::Studio::EventInstance* instance, Object* node);
        void _run_callbacks();

        FMOD::Studio::EventInstance* _create_instance(const String& eventName, bool isOneShot, Object* gameObject);
        static EventInfo* _get_event_info(FMOD::Studio::EventInstance* eventInstance);
        void _release_one_event(FMOD::Studio::EventInstance* eventInstance);
        void _load_bank_data(LoadingBank* loadingBank);
        void _load_all_vca(FMOD::Studio::Bank* bank);
        void _load_all_buses(FMOD::Studio::Bank* bank);
        void _load_all_event_descriptions(FMOD::Studio::Bank* bank);
        void _unload_all_vca(FMOD::Studio::Bank* bank);
        void _unload_all_buses(FMOD::Studio::Bank* bank);
        void _unload_all_event_descriptions(FMOD::Studio::Bank* bank);
        static bool _is_channel_valid(FMOD::Channel* channel);

    public:
        Fmod();

        ~Fmod();

        static Fmod* get_singleton();

        void update();
        void shutdown();
        void init(int numOfChannels, unsigned int studioFlag, unsigned int flag);
        void set_sound_3d_settings(float dopplerScale, float distanceFactor, float rollOffScale);
        void set_software_format(int sampleRate, int speakerMode, int numRawSpeakers);

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
        void set_system_dsp_buffer_size(unsigned int bufferlength, int numbuffers);
        unsigned int get_system_dsp_buffer_length();
        int get_system_dsp_num_buffers();
        Array get_system_dsp_buffer_size();

        String load_bank(const String& pathToBank, unsigned int flag);
        void unload_bank(const String& pathToBank);
        bool check_vca_path(const String& vcaPath);
        bool check_bus_path(const String& busPath);
        bool check_event_path(const String& eventPath);
        int get_bank_loading_state(const String& pathToBank);
        int get_bank_bus_count(const String& pathToBank);
        int get_bank_event_count(const String& pathToBank);
        int get_bank_string_count(const String& pathToBank);
        int get_bank_vca_count(const String& pathToBank);
        uint64_t create_event_instance(const String& eventPath);

        /* event descriptions functions */


        /* bus functions */


        /* VCA functions */

        /* Helper methods */
        void play_one_shot(const String& eventName, Object* gameObj);
        void play_one_shot_with_params(const String& eventName, Object* gameObj, const Dictionary& parameters);
        void play_one_shot_attached(const String& eventName, Object* gameObj);
        void play_one_shot_attached_with_params(const String& eventName, Object* gameObj, const Dictionary& parameters);


        void pause_all_events(bool pause);
        void mute_all_events();
        void unmute_all_events();
        bool banks_still_loading();

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
        void set_sound_paused(const uint64_t instanceId, bool paused);
        void stop_sound(const uint64_t instanceId);
        bool is_sound_playing(const uint64_t instanceId);
        void set_sound_volume(const uint64_t instanceId, float volume);
        float get_sound_volume(const uint64_t instanceId);
        float get_sound_pitch(const uint64_t instanceId);
        void set_sound_pitch(const uint64_t instanceId, float pitch);
        // Playing a sound
        void play_sound(const uint64_t instanceId);

        // MISC
        void wait_for_all_loads();
        Array get_available_drivers();
        int get_driver();
        void set_driver(int id);
        Dictionary get_performance_data();
        void set_global_parameter_by_name(const String& parameterName, float value);
        float get_global_parameter_by_name(const String& parameterName);
        void set_global_parameter_by_id(const Array& idPair, const float value);
        float get_global_parameter_by_id(const Array& idPair);
        Dictionary get_global_parameter_desc_by_name(const String& parameterName);
        Dictionary get_global_parameter_desc_by_id(const Array& idPair);
        int get_global_parameter_desc_count();
        Array get_global_parameter_desc_list();

        void set_callback(const uint64_t instanceId, int callbackMask);

    protected:
        static void _bind_methods();
    };
}// namespace godot

#endif// GODOTFMOD_GODOT_FMOD_H
