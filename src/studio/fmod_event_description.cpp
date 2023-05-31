#include "fmod_event_description.h"

#include "fmod_event.h"
#include "helpers/common.h"

using namespace godot;

void FmodEventDescription::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_length"), &FmodEventDescription::get_length);
    ClassDB::bind_method(D_METHOD("get_instance_list"), &FmodEventDescription::get_instance_list);
    ClassDB::bind_method(D_METHOD("get_instance_count"), &FmodEventDescription::get_instance_count);
    ClassDB::bind_method(D_METHOD("release_all_instances"), &FmodEventDescription::release_all_instances);
    ClassDB::bind_method(D_METHOD("load_sample_data"), &FmodEventDescription::load_sample_data);
    ClassDB::bind_method(D_METHOD("unload_sample_data"), &FmodEventDescription::unload_sample_data);
    ClassDB::bind_method(D_METHOD("get_sample_loading_state"), &FmodEventDescription::get_sample_loading_state);
    ClassDB::bind_method(D_METHOD("is_3d"), &FmodEventDescription::is_3d);
    ClassDB::bind_method(D_METHOD("is_one_shot"), &FmodEventDescription::is_one_shot);
    ClassDB::bind_method(D_METHOD("is_snapshot"), &FmodEventDescription::is_snapshot);
    ClassDB::bind_method(D_METHOD("is_stream"), &FmodEventDescription::is_stream);
    ClassDB::bind_method(D_METHOD("has_sustain_point"), &FmodEventDescription::has_sustain_point);
    ClassDB::bind_method(D_METHOD("get_min_max_distance"), &FmodEventDescription::get_min_max_distance);
    ClassDB::bind_method(D_METHOD("get_sound_size"), &FmodEventDescription::get_sound_size);
    ClassDB::bind_method(D_METHOD("get_parameter_by_name", "name"), &FmodEventDescription::get_parameter_by_name);
    ClassDB::bind_method(
      D_METHOD(
        "get_parameter_by_id",
        "eventPath"
        "idPair"
      ),
      &FmodEventDescription::get_parameter_by_id
    );
    ClassDB::bind_method(D_METHOD("get_parameter_count"), &FmodEventDescription::get_parameter_count);
    ClassDB::bind_method(D_METHOD("get_parameter_by_index", "index"), &FmodEventDescription::get_parameter_by_index);
    ClassDB::bind_method(D_METHOD("get_user_property", "name"), &FmodEventDescription::get_user_property);
    ClassDB::bind_method(D_METHOD("get_user_property_count"), &FmodEventDescription::get_user_property_count);
    ClassDB::bind_method(D_METHOD("user_property_by_index", "index"), &FmodEventDescription::user_property_by_index);
    ClassDB::bind_method(D_METHOD("is_valid"), &FmodEventDescription::is_valid);
}

int FmodEventDescription::get_length() {
    int length = -1;
    ERROR_CHECK(_wrapped->getLength(&length));
    return length;
}

Array FmodEventDescription::get_instance_list() {
    Array array;
    FMOD::Studio::EventInstance* instances[MAX_EVENT_INSTANCE_COUNT];
    int count = 0;
    ERROR_CHECK(_wrapped->getInstanceList(instances, MAX_EVENT_INSTANCE_COUNT, &count));
    CHECK_SIZE(MAX_EVENT_INSTANCE_COUNT, count, events)
    for (int i = 0; i < count; ++i) {
        godot::FmodEvent* event_instance;
        instances[i]->getUserData((void**) &event_instance);
        array.append(Ref(event_instance));
    }
    return array;
}

int FmodEventDescription::get_instance_count() {
    int count = -1;
    ERROR_CHECK(_wrapped->getInstanceCount(&count));
    return count;
}

void FmodEventDescription::release_all_instances() {
    ERROR_CHECK(_wrapped->releaseAllInstances());
}

void FmodEventDescription::load_sample_data() {
    ERROR_CHECK(_wrapped->loadSampleData());
}

void FmodEventDescription::unload_sample_data() {
    ERROR_CHECK(_wrapped->unloadSampleData());
}

int FmodEventDescription::get_sample_loading_state() {
    FMOD_STUDIO_LOADING_STATE s;
    ERROR_CHECK(_wrapped->getSampleLoadingState(&s));
    return s;
}

bool FmodEventDescription::is_3d() {
    bool is3D = false;
    ERROR_CHECK(_wrapped->is3D(&is3D));
    return is3D;
}

bool FmodEventDescription::is_one_shot() {
    bool isOneShot = false;
    ERROR_CHECK(_wrapped->isOneshot(&isOneShot));
    return isOneShot;
}

bool FmodEventDescription::is_snapshot() {
    bool isSnapshot = false;
    ERROR_CHECK(_wrapped->isSnapshot(&isSnapshot));
    return isSnapshot;
}

bool FmodEventDescription::is_stream() {
    bool isStream = false;
    ERROR_CHECK(_wrapped->isStream(&isStream));
    return isStream;
}

bool FmodEventDescription::has_sustain_point() {
    bool hasSustainPoint = false;
    ERROR_CHECK(_wrapped->hasSustainPoint(&hasSustainPoint));
    return hasSustainPoint;
}

Array FmodEventDescription::get_min_max_distance() {
    float minDistance;
    float maxDistance;
    Array ret;
    ERROR_CHECK(_wrapped->getMinMaxDistance(&minDistance, &maxDistance));
    ret.append(minDistance);
    ret.append(maxDistance);
    return ret;
}

float FmodEventDescription::get_sound_size() {
    float soundSize = 0.f;
    ERROR_CHECK(_wrapped->getSoundSize(&soundSize));
    return soundSize;
}

Dictionary FmodEventDescription::get_parameter_by_name(const String& name) {
    Dictionary paramDesc;
    FMOD_STUDIO_PARAMETER_DESCRIPTION
    pDesc;
    if (ERROR_CHECK(_wrapped->getParameterDescriptionByName(name.utf8().get_data(), &pDesc))) {
        paramDesc["name"] = String(pDesc.name);
        paramDesc["id_first"] = pDesc.id.data1;
        paramDesc["id_second"] = pDesc.id.data2;
        paramDesc["minimum"] = pDesc.minimum;
        paramDesc["maximum"] = pDesc.maximum;
        paramDesc["default_value"] = pDesc.defaultvalue;
    }
    return paramDesc;
}

Dictionary FmodEventDescription::get_parameter_by_id(const Array& idPair) {
    Dictionary paramDesc;
    FMOD_STUDIO_PARAMETER_ID paramId;
    paramId.data1 = (unsigned int) idPair[0];
    paramId.data2 = (unsigned int) idPair[1];
    FMOD_STUDIO_PARAMETER_DESCRIPTION
    pDesc;
    if (ERROR_CHECK(_wrapped->getParameterDescriptionByID(paramId, &pDesc))) {
        paramDesc["name"] = String(pDesc.name);
        paramDesc["id_first"] = pDesc.id.data1;
        paramDesc["id_second"] = pDesc.id.data2;
        paramDesc["minimum"] = pDesc.minimum;
        paramDesc["maximum"] = pDesc.maximum;
        paramDesc["default_value"] = pDesc.defaultvalue;
    }
    return paramDesc;
}

int FmodEventDescription::get_parameter_count() {
    int count = 0;
    ERROR_CHECK(_wrapped->getParameterDescriptionCount(&count));
    return count;
}

Dictionary FmodEventDescription::get_parameter_by_index(int index) {
    Dictionary paramDesc;
    FMOD_STUDIO_PARAMETER_DESCRIPTION
    pDesc;
    if (ERROR_CHECK(_wrapped->getParameterDescriptionByIndex(index, &pDesc))) {
        paramDesc["name"] = String(pDesc.name);
        paramDesc["id_first"] = pDesc.id.data1;
        paramDesc["id_second"] = pDesc.id.data2;
        paramDesc["minimum"] = pDesc.minimum;
        paramDesc["maximum"] = pDesc.maximum;
        paramDesc["default_value"] = pDesc.defaultvalue;
    }
    return paramDesc;
}

Dictionary FmodEventDescription::get_user_property(const String& name) {
    Dictionary propDesc;
    FMOD_STUDIO_USER_PROPERTY
    uProp;
    if (ERROR_CHECK(_wrapped->getUserProperty(name.utf8().get_data(), &uProp))) {
        FMOD_STUDIO_USER_PROPERTY_TYPE fType = uProp.type;
        if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_INTEGER) propDesc[String(uProp.name)] = uProp.intvalue;
        else if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_BOOLEAN)
            propDesc[String(uProp.name)] = (bool) uProp.boolvalue;
        else if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_FLOAT)
            propDesc[String(uProp.name)] = uProp.floatvalue;
        else if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_STRING)
            propDesc[String(uProp.name)] = String(uProp.stringvalue);
    }
    return propDesc;
}

int FmodEventDescription::get_user_property_count() {
    int count = 0;
    ERROR_CHECK(_wrapped->getUserPropertyCount(&count));
    return count;
}

Dictionary FmodEventDescription::user_property_by_index(int index) {
    Dictionary propDesc;
    FMOD_STUDIO_USER_PROPERTY
    uProp;
    if (ERROR_CHECK(_wrapped->getUserPropertyByIndex(index, &uProp))) {
        FMOD_STUDIO_USER_PROPERTY_TYPE fType = uProp.type;
        if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_INTEGER) propDesc[String(uProp.name)] = uProp.intvalue;
        else if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_BOOLEAN)
            propDesc[String(uProp.name)] = (bool) uProp.boolvalue;
        else if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_FLOAT)
            propDesc[String(uProp.name)] = uProp.floatvalue;
        else if (fType == FMOD_STUDIO_USER_PROPERTY_TYPE_STRING)
            propDesc[String(uProp.name)] = String(uProp.stringvalue);
    }

    return propDesc;
}