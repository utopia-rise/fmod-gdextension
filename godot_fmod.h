#ifndef GODOTFMOD_GODOT_FMOD_H
#define GODOTFMOD_GODOT_FMOD_H

#include <Godot.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>
#include <fmod_studio.hpp>
#include <Spatial.hpp>
#include <Object.hpp>
#include <CanvasItem.hpp>
#include <Node.hpp>
#include <vector>
#include <map>

namespace godot {
    class GodotFmod : public Node {
        FMOD::Studio::System *system;
        FMOD::System *lowLevelSystem;

        float distanceScale;

        Object *listener;

        bool nullListenerWarning = true;

        std::map<String, FMOD::Studio::Bank *> banks;
        std::map<String, FMOD::Studio::EventDescription *> eventDescriptions;
        std::map<String, FMOD::Studio::Bus *> buses;
        std::map<String, FMOD::Studio::VCA *> VCAs;
        std::map<const uint64_t, FMOD::Sound *> sounds;
        std::map<FMOD::Sound *, FMOD::Channel *> channels;

        // keep track of one shot instances internally
        std::vector<FMOD::Studio::EventInstance *> oneShotInstances;
        struct AttachedOneShot {
            FMOD::Studio::EventInstance *instance;
            Object *gameObj;
        };
        std::vector<AttachedOneShot> attachedOneShots;

        // events not directly managed by the integration
        // referenced through uuids generated in script
        std::map<const uint64_t, FMOD::Studio::EventInstance *> unmanagedEvents;

    GODOT_CLASS(GodotFmod, Node)

    private:
        int checkErrors(FMOD_RESULT result);
        void setListenerAttributes();
        FMOD_VECTOR toFmodVector(Vector3 &vec);
        FMOD_3D_ATTRIBUTES get3DAttributes(const FMOD_VECTOR &pos, const FMOD_VECTOR &up, const FMOD_VECTOR &forward,
                                           const FMOD_VECTOR &vel);
        bool isNull(Object *o);
        void loadBus(const String &busPath);
        void loadVCA(const String &VCAPath);
        void updateInstance3DAttributes(FMOD::Studio::EventInstance *instance, Object *o);

    public:
        GodotFmod();

        ~GodotFmod();

        static void _register_methods();
        void _init();
        void init(int numOfChannels, unsigned int studioFlag, unsigned int flag);
        void setSound3DSettings(float dopplerScale, float distanceFactor, float rollOffScale);
        void update();
        void shutdown();
        void addListener(Object *gameObj);
        void setSoftwareFormat(int sampleRate, int speakerMode, int numRawSpeakers);
        String loadbank(String pathToBank, unsigned int flag);
        void unloadBank(String pathToBank);
        int getBankLoadingState(String pathToBank);
        int getBankBusCount(String pathToBank);
        int getBankEventCount(String pathToBank);
        int getBankStringCount(String pathToBank);
        int getBankVCACount(String pathToBank);
        const uint64_t createEventInstance(String eventPath);
        float getEventParameter(uint64_t instanceId, String parameterName);
        void setEventParameter(uint64_t instanceId, String parameterName, float value);
        void releaseEvent(uint64_t instanceId);
        void startEvent(uint64_t instanceId);
        void stopEvent(uint64_t instanceId, int stopMode);
        void triggerEventCue(uint64_t instanceId);
        int getEventPlaybackState(uint64_t instanceId);
        bool getEventPaused(uint64_t instanceId);
        void setEventPaused(uint64_t instanceId, bool paused);
        float getEventPitch(uint64_t instanceId);
        void setEventPitch(uint64_t instanceId, float pitch);
        float getEventVolume(uint64_t instanceId);
        void setEventVolume(uint64_t instanceId, float volume);
        int getEventTimelinePosition(uint64_t instanceId);
        void setEventTimelinePosition(uint64_t instanceId, int position);
        float getEventReverbLevel(uint64_t instanceId, int index);
        void setEventReverbLevel(uint64_t instanceId, int index, float level);
        bool isEventVirtual(uint64_t instanceId);

        /* bus functions */
        bool getBusMute(String busPath);
        bool getBusPaused(String busPath);
        float getBusVolume(String busPath);
        void setBusMute(String busPath, bool mute);
        void setBusPaused(String busPath, bool paused);
        void setBusVolume(String busPath, float volume);
        void stopAllBusEvents(String busPath, int stopMode);

        /* VCA functions */
        float getVCAVolume(String VCAPath);
        void setVCAVolume(String VCAPath, float volume);
        void playOneShot(String eventName, Object *gameObj);
        void playOneShotWithParams(String eventName, Object *gameObj, Dictionary parameters);
        void playOneShotAttached(String eventName, Object *gameObj);
        void playOneShotAttachedWithParams(String eventName, Object *gameObj, Dictionary parameters);
        void attachInstanceToNode(uint64_t instanceId, Object *gameObj);
        void detachInstanceFromNode(uint64_t instanceId);

        void playSound(uint64_t instanceId);
        const uint64_t loadSound(String path, int mode);
        void releaseSound(uint64_t instanceId);
        void setSoundPaused(uint64_t instanceId, bool paused);
        void stopSound(uint64_t instanceId);
        bool isSoundPlaying(uint64_t instanceId);
        void setSoundVolume(uint64_t instanceId, float volume);
        float getSoundVolume(uint64_t instanceId);
        float getSoundPitch(uint64_t instanceId);
        void setSoundPitch(uint64_t instanceId, float pitch);

        Array getAvailableDrivers();
        int getDriver();
        void setDriver(int id);

    };
}

#endif //GODOTFMOD_GODOT_FMOD_H
