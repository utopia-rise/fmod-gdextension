#ifndef GODOTFMOD_FMOD_SERVER_H
#define GODOTFMOD_FMOD_SERVER_H

#include "core/fmod_file.h"
#include "core/fmod_sound.h"
#include "data/performance_data.h"
#include "fmod_cache.h"
#include "studio/fmod_bank.h"
#include "studio/fmod_bus.h"
#include "studio/fmod_event.h"
#include "studio/fmod_event_description.h"
#include "studio/fmod_vca.h"
#include "templates/hash_map.hpp"
#include "templates/vector.hpp"
#include "variant/string.hpp"

#include <callback/event_callbacks.h>
#include <callback/file_callbacks.h>
#include <helpers/constants.h>
#include <resources/fmod_dsp_settings.h>
#include <resources/fmod_settings.h>
#include <resources/fmod_software_format_settings.h>
#include <resources/fmod_sound_3d_settings.h>

#include <classes/canvas_item.hpp>
#include <classes/global_constants.hpp>
#include <classes/mutex.hpp>
#include <classes/object.hpp>
#include <core/object.hpp>
#include <fmod.hpp>
#include <fmod_studio.hpp>
#include <godot.hpp>
#include <variant/utility_functions.hpp>

namespace godot {

    struct OneShot {
        Node* gameObj = nullptr;
        Ref<FmodEvent> instance;
    };

    struct Listener {
        Object* gameObj = nullptr;
        bool listenerLock = false;
        float weight = 1.0;
    };

    struct Callback {
        FMOD_STUDIO_EVENT_CALLBACK_TYPE type;
        godot::Callable callable;
        Dictionary fmod_callback_properties;
    };

    class FmodServer : public Object {
        GDCLASS(FmodServer, Object);

        static FmodServer* singleton;

    public:
        union EventIdentifier {
            const char* string_identifier;
            FMOD_GUID guid;
            FmodEventDescription* event_description;
        };

        enum EventIdentifierType {
            PATH,
            GUID,
            EVENT_DESCRIPTION
        };

        union ParameterIdentifier {
            String* name;
            uint64_t id;
        };

        struct ParameterValue {
            ParameterIdentifier identifier;
            Variant value;
            Variant::Type variant_type;
            bool should_load_by_id;
        };

    private:
        FMOD::Studio::System* system;
        FMOD::System* coreSystem;

        bool isInitialized;
        bool isNotInitializedPrinted;

        float distanceScale;

        FmodCache* cache;

        int systemListenerNumber = 1;
        int actualListenerNumber = 0;
        bool listenerWarning = true;
        Listener listeners[FMOD_MAX_LISTENERS];

        Vector<OneShot*> oneShots;
        Vector<Ref<FmodEvent>> runningEvents;

        Ref<FmodPerformanceData> performanceData;

        // Direct call from fmod thread cannot be made as we cannot interact with scene tree from another thread (thread
        // guard).
        // call_deferred is not implemented in godot-cpp.
        // Would have prefered a SpinLock but does not seems to exist in godot-cpp.
        // TODO: Change when https://github.com/godotengine/godot-cpp/pull/1091 is merged.
        Ref<Mutex> callback_mutex;
        List<Callback> callbacks_to_process;


        void _set_listener_attributes();

        Ref<FmodEventDescription> _get_event_description(const String& event_name);
        Ref<FmodEventDescription> _get_event_description(const FMOD_GUID& guid);

        void _update_performance_data();

    public:
        FmodServer();

        ~FmodServer() override;

        static FmodServer* get_singleton();

        // LIFECYCLE
        void init(const Ref<FmodGeneralSettings>& p_settings);
        void update();
        void shutdown();

        // SETTINGS
        void set_software_format(const Ref<FmodSoftwareFormatSettings>& p_settings);
        void set_sound_3d_settings(const Ref<FmodSound3DSettings>& p_settings);
        void set_system_dsp_buffer_size(const Ref<FmodDspSettings>& p_settings);
        Ref<FmodDspSettings> get_system_dsp_buffer_settings();
        unsigned int get_system_dsp_buffer_length();
        int get_system_dsp_num_buffers();

        // OBJECTS
        bool check_vca_guid(const String& guid);
        bool check_vca_path(const String& vcaPath);
        bool check_bus_guid(const String& guid);
        bool check_bus_path(const String& busPath);
        bool check_event_guid(const String& guid);
        bool check_event_guid_internal(const FMOD_GUID& guid);
        bool check_event_path(const String& eventPath);
        Ref<FmodVCA> get_vca_from_guid(const String& guid);
        Ref<FmodVCA> get_vca(const String& vcaPath);
        Ref<FmodBus> get_bus_from_guid(const String& guid);
        Ref<FmodBus> get_bus(const String& busPath);
        Ref<FmodEventDescription> get_event_from_guid(const String& guid);
        Ref<FmodEventDescription> get_event_from_guid_internal(const FMOD_GUID& guid);
        Ref<FmodEventDescription> get_event(const String& eventPath);
        FMOD_GUID get_event_guid_internal(const String& event_path);
        String get_event_guid(const String& event_path);
        String get_event_path_internal(const FMOD_GUID& guid);
        String get_event_path(const String& guid);
        Array get_all_vca();
        Array get_all_buses();
        Array get_all_event_descriptions();
        Array get_all_banks();

        // DEBUGGING
        Array get_available_drivers();
        int get_driver();
        void set_driver(int id);
        Ref<FmodPerformanceData> get_performance_data();

        // GLOBAL PARAMETERS
        void set_global_parameter_by_name(const String& parameter_name, float value);
        void set_global_parameter_by_name_with_label(const String& parameter_name, const String& label);
        float get_global_parameter_by_name(const String& parameter_name);
        void set_global_parameter_by_id(uint64_t parameter_id, float value);
        void set_global_parameter_by_id_with_label(uint64_t parameter_id, const String& label);
        float get_global_parameter_by_id(uint64_t parameter_id);
        Dictionary get_global_parameter_desc_by_name(const String& parameter_name);
        Dictionary get_global_parameter_desc_by_id(uint64_t parameter_id);
        int get_global_parameter_desc_count();
        Array get_global_parameter_desc_list();

        // LISTENERS
        void add_listener(int index, Object* game_obj);
        void remove_listener(int index, Object* game_obj);
        void set_system_listener_number(int listenerNumber);
        int get_system_listener_number() const;
        float get_system_listener_weight(int index);
        void set_system_listener_weight(int index, float weight);
        Transform3D get_listener_transform3d(int index);
        Transform2D get_listener_transform2d(int index);
        Vector3 get_listener_3d_velocity(int index);
        Vector2 get_listener_2d_velocity(int index);
        void set_listener_transform3d(int index, const Transform3D& transform);
        void set_listener_transform2d(int index, const Transform2D& transform);
        void set_listener_lock(int index, bool isLocked);
        bool get_listener_lock(int index);
        Object* get_object_attached_to_listener(int index);

        // BANKS
        Ref<FmodBank> load_bank(const String& pathToBank, unsigned int flag);
        void unload_bank(const String& pathToBank);
        bool banks_still_loading();

        // EVENTS
    private:
        template<EventIdentifierType parameter_type>
        Ref<FmodEvent> _create_event_instance(const EventIdentifier& identifier);
    public:
        Ref<FmodEvent> create_event_instance_with_guid(const String& guid);
        Ref<FmodEvent> create_event_instance_with_guid_internal(const FMOD_GUID& guid);
        Ref<FmodEvent> create_event_instance(const String& eventPath);
        Ref<FmodEvent> create_event_instance_from_description(const Ref<FmodEventDescription>& event_description);

        // SOUNDS
        Ref<FmodFile> load_file_as_sound(const String& path);
        Ref<FmodFile> load_file_as_music(const String& path);
        void unload_file(const String& path);
        Ref<FmodSound> create_sound_instance(const String& path);
        FMOD_STUDIO_SOUND_INFO get_sound_info(const String& sound_key) const;
        FMOD::Sound* create_sound(FMOD_STUDIO_SOUND_INFO& sound_info, FMOD_MODE mode) const;

        //CALLBACKS
        void add_callback(const Callback& callback);

        /* Helper methods */
    private:
        template<EventIdentifierType parameter_type>
        Ref<FmodEventDescription> fetch_event_description(const EventIdentifier& identifier);
        template<EventIdentifierType parameter_type>
        void _play_one_shot(const EventIdentifier& identifier, Node* game_obj, const Dictionary& parameters = Dictionary());
        static void _apply_parameter_dict_to_event(const Ref<FmodEvent>& p_event, const Dictionary& parameters);

    public:
        template<class TParameter>
        void apply_parameter_list_to_event(const Ref<FmodEvent>& p_event, const List<TParameter>& parameters);

        void play_one_shot(const String& event_name);
        void play_one_shot_with_params(const String& event_name, const Dictionary& parameters);
        void play_one_shot_attached(const String& event_name, Node* game_obj);
        void play_one_shot_attached_with_params(const String& event_name, Node* game_obj, const Dictionary& parameters);

        void play_one_shot_using_guid(const String& guid);
        void play_one_shot_using_guid_with_params(const String& guid, const Dictionary& parameters);
        void play_one_shot_using_guid_attached(const String& guid, Node* game_obj);
        void play_one_shot_using_guid_attached_with_params(const String& guid, Node* game_obj, const Dictionary& parameters);

        void play_one_shot_using_event_description(const Ref<FmodEventDescription>& event_description);
        void play_one_shot_using_event_description_with_params(const Ref<FmodEventDescription>& event_description, const Dictionary& parameters);
        void play_one_shot_using_event_description_attached(const Ref<FmodEventDescription>& event_description, Node* game_obj);
        void play_one_shot_using_event_description_attached_with_params(const Ref<FmodEventDescription>& event_description, Node* game_obj, const Dictionary& parameters);

        void pause_all_events();
        void unpause_all_events();
        void mute_all_events();
        void unmute_all_events();
        void wait_for_all_loads();

    protected:
        static void _bind_methods();
    };


    template<FmodServer::EventIdentifierType parameter_type>
    Ref<FmodEventDescription> FmodServer::fetch_event_description(const FmodServer::EventIdentifier& identifier){
        Ref<FmodEventDescription> desc;
        switch (parameter_type) {
            case PATH:
                desc = _get_event_description(identifier.string_identifier);
                break;
            case GUID:
                desc = _get_event_description(identifier.guid);
                break;
            case EVENT_DESCRIPTION:
                desc = Ref<FmodEventDescription>(identifier.event_description);
                break;
        }
        return desc;
    }

    template<FmodServer::EventIdentifierType parameter_type>
    Ref<FmodEvent> FmodServer::_create_event_instance(const EventIdentifier& identifier) {
        FMOD::Studio::EventInstance* eventInstance = nullptr;
        ERROR_CHECK(fetch_event_description<parameter_type>(identifier)->get_wrapped()->createInstance(&eventInstance));

        Ref<FmodEvent> ref = FmodEvent::create_ref(eventInstance);
        if (ref.is_null() || !ref->is_valid()) {
            GODOT_LOG_WARNING("Event Instance is invalid.")
            return {};
        }

        ref->get_wrapped()->setUserData(ref.ptr());
        ref->set_distance_scale(distanceScale);
        runningEvents.push_back(ref);
        return ref;
    }

    template<FmodServer::EventIdentifierType parameter_type>
    void FmodServer::_play_one_shot(const FmodServer::EventIdentifier& identifier, Node* game_obj, const Dictionary& parameters) {
        Ref<FmodEventDescription> desc =  fetch_event_description<parameter_type>(identifier);
        if (!desc->is_one_shot()){
            GODOT_LOG_WARNING(desc->get_path() + " is not a OneShot event.")
            return;
        }

        if (game_obj && !is_fmod_valid(game_obj)) {
            return;
        }

        Ref<FmodEvent> ref = _create_event_instance<parameter_type>(identifier);

        if (ref.is_null()) { return; }

        if (!parameters.is_empty()) {
            // set the initial parameter values
            _apply_parameter_dict_to_event(ref, parameters);
        }

        if(game_obj){
            auto* oneShot = new OneShot();
            oneShot->gameObj = game_obj;
            oneShot->instance = ref;
            ref->set_node_attributes(game_obj);
            oneShots.push_back(oneShot);
        }

        ref->start();
        ref->release();
    }

    template<class TParameter>
    void FmodServer::apply_parameter_list_to_event(const Ref<FmodEvent>& p_event, const List<TParameter>& parameters) {
        for (const TParameter& parameter : parameters) {
            if (parameter.should_load_by_id) {
                if (parameter.variant_type == Variant::Type::STRING) {
                    p_event->set_parameter_by_id_with_label(parameter.identifier.id, parameter.value);
                    continue;
                }
                p_event->set_parameter_by_id(parameter.identifier.id, parameter.value);
                continue;
            }

            if (parameter.variant_type == Variant::Type::STRING) {
                p_event->set_parameter_by_name_with_label(*parameter.identifier.name, parameter.value);
                continue;
            }
            p_event->set_parameter_by_name(*parameter.identifier.name, parameter.value);
        }
    }
}// namespace godot

#endif// GODOTFMOD_FMOD_SERVER_H
