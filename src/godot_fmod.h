#ifndef GODOTFMOD_GODOT_FMOD_H
#define GODOTFMOD_GODOT_FMOD_H

#include <godot.hpp>
#include <classes/global_constants.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>
#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <core/object.hpp>
#include <classes/canvas_item.hpp>
#include <classes/node.hpp>
#include <classes/mutex.hpp>
#include "callback/event_callbacks.h"
#include "callback/file_callbacks.h"
#include "helpers/containers.h"
#include "helpers/constants.h"
#include "helpers/current_function.h"
#include <variant/utility_functions.hpp>

#define CUSTOM_FILESYSTEM

#ifndef CUSTOM_FILESYSTEM
    #ifdef __ANDROID__
        #define DRIVE_PATH(path)\
path = path.replace("res://", "file:///android_asset/");
    #else
        #define DRIVE_PATH(path)\
path = path.replace("res://", "./");
    #endif
#else
    #define DRIVE_PATH(path)
#endif

#define MAX_PATH_SIZE 512
#define MAX_VCA_COUNT 64
#define MAX_BUS_COUNT 64
#define MAX_EVENT_INSTANCE 128
#define MAX_EVENT_COUNT 256
#define MAX_DRIVER_NAME_SIZE 256

namespace godot {

#define GODOT_LOG(level, message)\
    switch (level) {\
        case 0:\
            UtilityFunctions::print(message);\
            break;\
        case 1:\
            UtilityFunctions::push_warning(message, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);\
            break;\
        case 2:\
            UtilityFunctions::push_error(message, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);\
            break;\
    }\

#define FIND_AND_CHECK_WITH_RETURN(instanceId, cont, defaultReturn) \
    auto instance = (cont).get(instanceId); \
    if (!instance) { \
        String message = String("FMOD Sound System: cannot find " + String(instanceId) + " in ##cont collection.");\
        GODOT_LOG(2, message)\
        return defaultReturn; \
    }
#define FIND_AND_CHECK_WITHOUT_RETURN(instanceId, set) FIND_AND_CHECK_WITH_RETURN(instanceId, set, void())
#define FUNC_CHOOSER(_f1, _f2, _f3, _f4, ...) _f4
#define FUNC_RECOMPOSER(argsWithParentheses) FUNC_CHOOSER argsWithParentheses
#define MACRO_CHOOSER(...) FUNC_RECOMPOSER((__VA_ARGS__, FIND_AND_CHECK_WITH_RETURN, FIND_AND_CHECK_WITHOUT_RETURN, ))
#define FIND_AND_CHECK(...) MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define CHECK_SIZE(maxSize, actualSize, type) \
    if((actualSize) > (maxSize)){\
        String message = "FMOD Sound System: type maximum size is " + String::num(maxSize) + " but the bank contains " + String::num(actualSize) + " entries";\
        GODOT_LOG(2, message)\
        (actualSize) = maxSize;\
    }\

    struct EventInfo {
        //Is the event oneshot
        bool isOneShot = false;
        //GameObject to which this event is attached
        Node* gameObj = nullptr;
        // Callback info associated with this event
        Callbacks::CallbackInfo callbackInfo = Callbacks::CallbackInfo();
    };

    struct LoadingBank {
        FMOD::Studio::Bank*  bank;
        String godotPath;
    };

    struct Listener {
        Node* gameObj = nullptr;
        bool listenerLock = false;
        float weight = 1.0;
    };

    class Fmod : public Node {
    GDCLASS(Fmod, Node)
        DECLARE_ALL_CONSTANTS

    protected:
        static void _bind_methods();

    private:
        FMOD::Studio::System *system;
        FMOD::System *coreSystem;

        bool isInitialized;
        bool isNotinitPrinted;

        float distanceScale;

        int systemListenerNumber = 1;
        int actualListenerNumber = 0;
        Listener listeners[FMOD_MAX_LISTENERS];
        bool listenerWarning = true;

        Vector<LoadingBank* > loadingBanks;
        Map<String, FMOD::Studio::Bank*> banks;
        Map<String, FMOD::Studio::EventDescription*> eventDescriptions;
        Map<String, FMOD::Sound*> sounds;
        Map<String, FMOD::Studio::Bus*> buses;
        Map<String, FMOD::Studio::VCA*> VCAs;

        Vector<FMOD::Channel*> channels;
        Vector<FMOD::Studio::EventInstance*> events;

        //Store dictionary of performance data
        Dictionary performanceData;

        void _check_loading_banks();
        void _set_listener_attributes();

        static bool checkErrors(FMOD_RESULT result, const char *function, const char *file, int line);
#define ERROR_CHECK(_result) checkErrors(_result, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__)

        static FMOD_VECTOR _to_fmod_vector(Vector3& vec);
        static FMOD_3D_ATTRIBUTES _get_3d_attributes(const FMOD_VECTOR& pos, const FMOD_VECTOR& up, const FMOD_VECTOR& forward,
                                                     const FMOD_VECTOR& vel);
        FMOD_3D_ATTRIBUTES _get_3d_attributes_from_transform(const Transform3D& transform) const;
        FMOD_3D_ATTRIBUTES _get_3d_attributes_from_transform_2d(const Transform2D& transform) const;
        Dictionary _get_transform_info_from_3d_attribut(FMOD_3D_ATTRIBUTES& attribut) const;
        Dictionary _get_transform_2d_info_from_3d_attribut(FMOD_3D_ATTRIBUTES& attribut) const;

        static bool _is_dead(Node* node);
        static bool _is_fmod_valid(Node* node);
        void _update_instance_3d_attributes(FMOD::Studio::EventInstance* instance, Node* node);
        void _run_callbacks();

        FMOD::Studio::EventInstance* _create_instance(const String& eventName, bool isOneShot, Node* gameObject);
        static EventInfo *_get_event_info(FMOD::Studio::EventInstance* eventInstance);
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

        void _init();
        void _process(float delta);
        void shutdown();
        void init(int numOfChannels, unsigned int studioFlag, unsigned int flag);
        void set_sound_3d_settings(float dopplerScale, float distanceFactor, float rollOffScale);
        void set_software_format(int sampleRate, int speakerMode, int numRawSpeakers);

        void add_listener(int index, Node* gameObj);
        void remove_listener(int index);
        void set_listener_number(int listenerNumber);
        int get_system_num_listeners() const;
        float get_system_listener_weight(int index);
        void set_system_listener_weight(int index, float weight);
        Dictionary get_system_listener_3d_attributes(int index);
        Dictionary get_system_listener_2d_attributes(int index);
        void set_system_listener_3d_attributes(int index, const Transform3D& transform);
        void set_system_listener_2d_attributes(int index, const Transform2D& transform);
        void set_listener_lock(int index, bool isLocked);
        bool get_listener_lock(int index);
        Node* get_object_attached_to_listener(int index);
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
        float get_event_parameter_by_name(uint64_t instanceId, const String& parameterName);
        void set_event_parameter_by_name(uint64_t instanceId, const String& parameterName, float value);
        float get_event_parameter_by_id(uint64_t instanceId, const Array& idPair);
        void set_event_parameter_by_id(uint64_t instanceId, const Array& idPair, float value);
        void release_event(uint64_t instanceId);
        void start_event(uint64_t instanceId);
        void stop_event(uint64_t instanceId, int stopMode);
        void event_key_off(uint64_t instanceId);
        int get_event_playback_state(uint64_t instanceId);
        bool get_event_paused(uint64_t instanceId);
        void set_event_paused(uint64_t instanceId, bool paused);
        float get_event_pitch(uint64_t instanceId);
        void set_event_pitch(uint64_t instanceId, float pitch);
        float get_event_volume(uint64_t instanceId);
        void set_event_volume(uint64_t instanceId, float volume);
        int get_event_timeline_position(uint64_t instanceId);
        void set_event_timeline_position(uint64_t instanceId, int position);
        float get_event_reverb_level(uint64_t instanceId, int index);
        void set_event_reverb_level(uint64_t instanceId, int index, float level);
        bool is_event_virtual(uint64_t instanceId);
        void set_event_listener_mask(uint64_t instanceId, unsigned int mask);
        uint32_t get_event_listener_mask(uint64_t instanceId);
        void set_event_3d_attributes(uint64_t instanceId, const Transform3D& transform);
        Dictionary get_event_3d_attributes(uint64_t instanceId);
        Dictionary get_event_2d_attributes(uint64_t instanceId);
        void set_event_2d_attributes(uint64_t instanceId, Transform2D position);

        /* event descriptions functions */
        int desc_get_length(const String& eventPath);
        Array desc_get_instance_list(const String& eventPath);
        int desc_get_instance_count(const String& eventPath);
        void desc_release_all_instances(const String& eventPath);
        void desc_load_sample_data(const String& eventPath);
        void desc_unload_sample_data(const String& eventPath);
        int desc_get_sample_loading_state(const String& eventPath);
        bool desc_is_3d(const String& eventPath);
        bool desc_is_one_shot(const String& eventPath);
        bool desc_is_snapshot(const String& eventPath);
        bool desc_is_stream(const String& eventPath);
        bool desc_has_sustain_point(const String& eventPath);
        Array desc_get_min_max_distance(const String& eventPath);
        float desc_get_sound_size(const String& eventPath);
        Dictionary desc_get_parameter_description_by_name(const String& eventPath, const String& name);
        Dictionary desc_get_parameter_description_by_id(const String& eventPath, const Array& idPair);
        int desc_get_parameter_description_count(const String& eventPath);
        Dictionary desc_get_parameter_description_by_index(const String& eventPath, int index);
        Dictionary desc_get_user_property(const String& eventPath, const String& name);
        int desc_get_user_property_count(const String& eventPath);
        Dictionary desc_user_property_by_index(const String& eventPath, int index);

        /* bus functions */
        bool get_bus_mute(const String& busPath);
        bool get_bus_paused(const String& busPath);
        float get_bus_volume(const String& busPath);
        void set_bus_mute(const String& busPath, bool mute);
        void set_bus_paused(const String& busPath, bool paused);
        void set_bus_volume(const String& busPath, float volume);
        void stop_all_bus_events(const String& busPath, int stopMode);

        /* VCA functions */
        float get_vca_volume(const String& VCAPath);
        void set_vca_volume(const String& VCAPath, float volume);
        /* Helper methods */
        void play_one_shot(const String& eventName, Node* gameObj);
        void play_one_shot_with_params(const String& eventName, Node* gameObj, const Dictionary& parameters);
        void play_one_shot_attached(const String& eventName, Node* gameObj);
        void play_one_shot_attached_with_params(const String& eventName, Node* gameObj, const Dictionary& parameters);
        void attach_instance_to_node(uint64_t instanceId, Node* gameObj);
        void detach_instance_from_node(const uint64_t instanceId);
        Node* get_object_attached_to_instance(uint64_t instanceId);
        void pause_all_events(bool pause);
        void mute_all_events();
        void unmute_all_events();
        bool banks_still_loading();


        //LOW LEVEL API
        //Load and release memory
        void load_file_as_sound(const String& path);
        void load_file_as_music(const String& path);
        void unload_file(const String& path);
        //Check validity of an instance
        uint64_t create_sound_instance(const String& path);
        bool check_sound_instance(const uint64_t instanceId);
        void release_sound(const uint64_t instanceId);
        //Setting the sound
        void set_sound_paused(const uint64_t instanceId, bool paused);
        void stop_sound(const uint64_t instanceId);
        bool is_sound_playing(const uint64_t instanceId);
        void set_sound_volume(const uint64_t instanceId, float volume);
        float get_sound_volume(const uint64_t instanceId);
        float get_sound_pitch(const uint64_t instanceId);
        void set_sound_pitch(const uint64_t instanceId, float pitch);
        //Playing a sound
        void play_sound(const uint64_t instanceId);

        //MISC
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
    };
}

#endif //GODOTFMOD_GODOT_FMOD_H
