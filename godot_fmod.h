//
// Created by ptmeisels on 27/02/19.
//

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
        std::map<String, FMOD::Sound *> sounds;
        std::map<FMOD::Sound *, FMOD::Channel *> channels;

        std::map<std::string, FMOD_INITFLAGS> fmodInitFlags;
        std::map<std::string, FMOD_STUDIO_INITFLAGS > fmodStudioInitFlags;
        std::map<std::string, FMOD_SPEAKERMODE> fmodSpeakerModeFlags;
        std::map<std::string, FMOD_STUDIO_LOAD_BANK_FLAGS> fmodLoadBankFlags;
        std::map<std::string, int > fmodSoundConstants;
        std::map<std::string, FMOD_STUDIO_STOP_MODE> fmodStudioStopModes;

        // keep track of one shot instances internally
        std::vector<FMOD::Studio::EventInstance *> oneShotInstances;
        struct AttachedOneShot {
            FMOD::Studio::EventInstance *instance;
            Object *gameObj;
        };
        std::vector<AttachedOneShot> attachedOneShots;

        // events not directly managed by the integration
        // referenced through uuids generated in script
        std::map<String, FMOD::Studio::EventInstance *> unmanagedEvents;

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
        void init(int numOfChannels, String studioFlags, String flags);
        void setSound3DSettings(float dopplerScale, float distanceFactor, float rollOffScale);
        void update();
        void shutdown();
        void addListener(Object *gameObj);
        void setSoftwareFormat(int sampleRate, String speakerMode, int numRawSpeakers);
        String loadbank(String pathToBank, String flags);
        void unloadBank(String pathToBank);
        int getBankLoadingState(String pathToBank);
        int getBankBusCount(String pathToBank);
        int getBankEventCount(String pathToBank);
        int getBankStringCount(String pathToBank);
        int getBankVCACount(String pathToBank);
        String createEventInstance(String uuid, String eventPath);
        float getEventParameter(String uuid, String parameterName);
        void setEventParameter(String uuid, String parameterName, float value);
        void releaseEvent(String uuid);
        void startEvent(String uuid);
        void stopEvent(String uuid, String stopModeStr);
        void triggerEventCue(String uuid);
        int getEventPlaybackState(String uuid);
        bool getEventPaused(String uuid);
        void setEventPaused(String uuid, bool paused);
        float getEventPitch(String uuid);
        void setEventPitch(String uuid, float pitch);
        float getEventVolume(String uuid);
        void setEventVolume(String uuid, float volume);
        int getEventTimelinePosition(String uuid);
        void setEventTimelinePosition(String uuid, int position);
        float getEventReverbLevel(String uuid, int index);
        void setEventReverbLevel(String uuid, int index, float level);
        bool isEventVirtual(String uuid);

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
        void attachInstanceToNode(String uuid, Object *gameObj);
        void detachInstanceFromNode(String uuid);

        void playSound(String uuid);
        String loadSound(String uuid, String path, String modeStr);
        void releaseSound(String path);
        void setSoundPaused(String uuid, bool paused);
        void stopSound(String uuid);
        bool isSoundPlaying(String uuid);
        void setSoundVolume(String uuid, float volume);
        float getSoundVolume(String uuid);
        float getSoundPitch(String uuid);
        void setSoundPitch(String uuid, float pitch);

    };
}

#endif //GODOTFMOD_GODOT_FMOD_H
