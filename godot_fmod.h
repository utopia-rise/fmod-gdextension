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
        std::map<unsigned int, FMOD::Sound *> sounds;
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
        std::map<unsigned int, FMOD::Studio::EventInstance *> unmanagedEvents;

        unsigned int instanceIdCount;

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
        unsigned int createEventInstance(String eventPath);
        float getEventParameter(unsigned int instanceId, String parameterName);
        void setEventParameter(unsigned int instanceId, String parameterName, float value);
        void releaseEvent(unsigned int instanceId);
        void startEvent(unsigned int instanceId);
        void stopEvent(unsigned int instanceId, int stopMode);
        void triggerEventCue(unsigned int instanceId);
        int getEventPlaybackState(unsigned int instanceId);
        bool getEventPaused(unsigned int instanceId);
        void setEventPaused(unsigned int instanceId, bool paused);
        float getEventPitch(unsigned int instanceId);
        void setEventPitch(unsigned int instanceId, float pitch);
        float getEventVolume(unsigned int instanceId);
        void setEventVolume(unsigned int instanceId, float volume);
        int getEventTimelinePosition(unsigned int instanceId);
        void setEventTimelinePosition(unsigned int instanceId, int position);
        float getEventReverbLevel(unsigned int instanceId, int index);
        void setEventReverbLevel(unsigned int instanceId, int index, float level);
        bool isEventVirtual(unsigned int instanceId);

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
        void attachInstanceToNode(unsigned int instanceId, Object *gameObj);
        void detachInstanceFromNode(unsigned int instanceId);

        void playSound(unsigned int instanceId);
        unsigned int loadSound(String path, int mode);
        void releaseSound(unsigned int instanceId);
        void setSoundPaused(unsigned int instanceId, bool paused);
        void stopSound(unsigned int instanceId);
        bool isSoundPlaying(unsigned int instanceId);
        void setSoundVolume(unsigned int instanceId, float volume);
        float getSoundVolume(unsigned int instanceId);
        float getSoundPitch(unsigned int instanceId);
        void setSoundPitch(unsigned int instanceId, float pitch);

    };
}

#endif //GODOTFMOD_GODOT_FMOD_H
