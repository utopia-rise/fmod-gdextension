#include "fmod_event_description.h"

#include "fmod_event.h"
#include "fmod_parameter_description.h"
#include "helpers/common.h"

using namespace godot;

constexpr const uint32_t PARAMETER_LABEL_BUFFER_SIZE {256};

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
    ClassDB::bind_method(D_METHOD("get_parameters"), &FmodEventDescription::get_parameters);
    ClassDB::bind_method(D_METHOD("get_parameter_label_by_id"), &FmodEventDescription::get_parameter_label_by_id);
    ClassDB::bind_method(D_METHOD("get_parameter_label_by_name"), &FmodEventDescription::get_parameter_label_by_name);
    ClassDB::bind_method(D_METHOD("get_parameter_label_by_index"), &FmodEventDescription::get_parameter_label_by_index);
    ClassDB::bind_method(D_METHOD("get_parameter_labels_by_id"), &FmodEventDescription::get_parameter_labels_by_id);
    ClassDB::bind_method(D_METHOD("get_parameter_labels_by_name"), &FmodEventDescription::get_parameter_labels_by_name);
    ClassDB::bind_method(D_METHOD("get_parameter_labels_by_index"), &FmodEventDescription::get_parameter_labels_by_index);
    ClassDB::bind_method(D_METHOD("get_user_property", "name"), &FmodEventDescription::get_user_property);
    ClassDB::bind_method(D_METHOD("get_user_property_count"), &FmodEventDescription::get_user_property_count);
    ClassDB::bind_method(D_METHOD("user_property_by_index", "index"), &FmodEventDescription::user_property_by_index);
    ClassDB::bind_method(D_METHOD("is_valid"), &FmodEventDescription::is_valid);

    ClassDB::bind_method(D_METHOD("get_path"), &FmodEventDescription::get_path);
    ClassDB::bind_method(D_METHOD("get_guid"), &FmodEventDescription::get_guid_as_string);
}

int FmodEventDescription::get_length() {
    int length = -1;
    ERROR_CHECK_WITH_REASON(_wrapped->getLength(&length), vformat("Cannot get event %s with guid %s length.", get_path(), get_guid_as_string()));
    return length;
}

Array FmodEventDescription::get_instance_list() {
    Array array;

    int size = 0;
    if (ERROR_CHECK_WITH_REASON(_wrapped->getInstanceCount(&size), vformat("Cannot get instances count for event %s with guid %s", get_path(), get_guid_as_string()))) {
        Vector<FMOD::Studio::EventInstance*> instances;
        instances.resize(size);

        if (ERROR_CHECK_WITH_REASON(_wrapped->getInstanceList(instances.ptrw(), size, &size), vformat("Cannot get instances list for event %s with guid %s", get_path(), get_guid_as_string()))) {
            for (int i = 0; i < size; ++i) {
                godot::FmodEvent* event_instance;
                instances[i]->getUserData((void**) &event_instance);
                array.append(Ref(event_instance));
            }
        }
    }

    return array;
}

int FmodEventDescription::get_instance_count() {
    int count = -1;
    ERROR_CHECK_WITH_REASON(_wrapped->getInstanceCount(&count), vformat("Cannot get instance count for event %s with guid %s", get_path(), get_guid_as_string()));
    return count;
}

void FmodEventDescription::release_all_instances() {
    ERROR_CHECK_WITH_REASON(_wrapped->releaseAllInstances(), vformat("Cannot get release all instances for event %s with guid %s", get_path(), get_guid_as_string()));
}

void FmodEventDescription::load_sample_data() {
    ERROR_CHECK_WITH_REASON(_wrapped->loadSampleData(), vformat("Cannot load sample data for event %s with guid %s", get_path(), get_guid_as_string()));
}

void FmodEventDescription::unload_sample_data() {
    ERROR_CHECK_WITH_REASON(_wrapped->unloadSampleData(), vformat("Cannot unload sample data for event %s with guid %s", get_path(), get_guid_as_string()));
}

int FmodEventDescription::get_sample_loading_state() {
    FMOD_STUDIO_LOADING_STATE s;
    ERROR_CHECK_WITH_REASON(_wrapped->getSampleLoadingState(&s), vformat("Cannot get sample loading state for event %s with guid %s", get_path(), get_guid_as_string()));
    return s;
}

bool FmodEventDescription::is_3d() {
    bool is3D = false;
    ERROR_CHECK_WITH_REASON(_wrapped->is3D(&is3D), vformat("Cannot check is_3d for event %s with guid %s", get_path(), get_guid_as_string()));
    return is3D;
}

bool FmodEventDescription::is_one_shot() {
    bool isOneShot = false;
    ERROR_CHECK_WITH_REASON(_wrapped->isOneshot(&isOneShot), vformat("Cannot check is_one_shot for event %s with guid %s", get_path(), get_guid_as_string()));
    return isOneShot;
}

bool FmodEventDescription::is_snapshot() {
    bool isSnapshot = false;
    ERROR_CHECK_WITH_REASON(_wrapped->isSnapshot(&isSnapshot), vformat("Cannot check is_snapshot for event %s with guid %s", get_path(), get_guid_as_string()));
    return isSnapshot;
}

bool FmodEventDescription::is_stream() {
    bool isStream = false;
    ERROR_CHECK_WITH_REASON(_wrapped->isStream(&isStream), vformat("Cannot check is_stream for event %s with guid %s", get_path(), get_guid_as_string()));
    return isStream;
}

bool FmodEventDescription::has_sustain_point() {
    bool hasSustainPoint = false;
    ERROR_CHECK_WITH_REASON(_wrapped->hasSustainPoint(&hasSustainPoint), vformat("Cannot check has_sustain_point for event %s with guid %s", get_path(), get_guid_as_string()));
    return hasSustainPoint;
}

Array FmodEventDescription::get_min_max_distance() {
    float minDistance;
    float maxDistance;
    Array ret;
    ERROR_CHECK_WITH_REASON(_wrapped->getMinMaxDistance(&minDistance, &maxDistance), vformat("Cannot get min max distance for event %s with guid %s", get_path(), get_guid_as_string()));
    ret.append(minDistance);
    ret.append(maxDistance);
    return ret;
}

float FmodEventDescription::get_sound_size() {
    float soundSize = 0.f;
    ERROR_CHECK_WITH_REASON(_wrapped->getSoundSize(&soundSize), vformat("Cannot get sound size for event %s with guid %s", get_path(), get_guid_as_string()));
    return soundSize;
}

Ref<FmodParameterDescription> FmodEventDescription::get_parameter_by_name(const String& name) const {
    Ref<FmodParameterDescription> param_desc;
    FMOD_STUDIO_PARAMETER_DESCRIPTION fmod_desc;
    if (ERROR_CHECK_WITH_REASON(_wrapped->getParameterDescriptionByName(name.utf8().get_data(), &fmod_desc),
                                vformat("Cannot get parameter %s description for event %s with guid %s", name, get_path(), get_guid_as_string()))) {
        param_desc = FmodParameterDescription::create_ref(fmod_desc);
    }
    return param_desc;
}

Ref<FmodParameterDescription> FmodEventDescription::get_parameter_by_id(uint64_t id) const {
    Ref<FmodParameterDescription> param_desc;
    FMOD_STUDIO_PARAMETER_ID param_id { ulong_to_fmod_parameter_id(id) };
    FMOD_STUDIO_PARAMETER_DESCRIPTION fmod_desc;
    if (ERROR_CHECK_WITH_REASON(_wrapped->getParameterDescriptionByID(param_id, &fmod_desc),
                                vformat("Cannot get parameter %d description for event %s with guid %s", id, get_path(), get_guid_as_string()))) {
        param_desc = FmodParameterDescription::create_ref(fmod_desc);
    }
    return param_desc;
}

int FmodEventDescription::get_parameter_count() const {
    int count = 0;
    ERROR_CHECK_WITH_REASON(_wrapped->getParameterDescriptionCount(&count),
                            vformat("Cannot get parameter count for event %s with guid %s", get_path(), get_guid_as_string()));
    return count;
}

Ref<FmodParameterDescription> FmodEventDescription::get_parameter_by_index(int index) const {
    Ref<FmodParameterDescription> param_desc;
    FMOD_STUDIO_PARAMETER_DESCRIPTION fmod_desc;
    if (ERROR_CHECK_WITH_REASON(_wrapped->getParameterDescriptionByIndex(index, &fmod_desc),
                                vformat("Cannot get parameter with index %d for event %s with guid %s", index, get_path(), get_guid_as_string()))) {
        param_desc = FmodParameterDescription::create_ref(fmod_desc);
    }
    return param_desc;
}

Array FmodEventDescription::get_parameters() const {
    Array parameters;
    for (int i = 0; i < get_parameter_count(); ++i) {
        parameters.append(get_parameter_by_index(i));
    }
    return parameters;
}

String FmodEventDescription::get_parameter_label_by_id(uint64_t id, int label_index) const {
    char label[PARAMETER_LABEL_BUFFER_SIZE];
    int retrieved;
    _wrapped->getParameterLabelByID(
      ulong_to_fmod_parameter_id(id),
      label_index,
      label,
      PARAMETER_LABEL_BUFFER_SIZE,
      &retrieved
    );
    return {label};
}

String FmodEventDescription::get_parameter_label_by_name(const String& parameter_name, int label_index) const {
    char label[PARAMETER_LABEL_BUFFER_SIZE];
    int retrieved;
    _wrapped->getParameterLabelByName(
      parameter_name.utf8().get_data(),
      label_index,
      label,
      PARAMETER_LABEL_BUFFER_SIZE,
      &retrieved
    );
    return {label};
}

String FmodEventDescription::get_parameter_label_by_index(int index, int label_index) const {
    char label[PARAMETER_LABEL_BUFFER_SIZE];
    int retrieved;
    _wrapped->getParameterLabelByIndex(
      index,
      label_index,
      label,
      PARAMETER_LABEL_BUFFER_SIZE,
      &retrieved
    );
    return {label};
}

PackedStringArray FmodEventDescription::get_parameter_labels_by_id(uint64_t id) const {
    PackedStringArray labels;

    Ref<FmodParameterDescription> parameter {get_parameter_by_id(id)};
    if (!parameter->is_labeled()) {
        return labels;
    }

    for (int i = 0; i <= static_cast<int>(parameter->get_maximum()); ++i) {
        labels.append(get_parameter_label_by_id(id, i));
    }

    return labels;
}

PackedStringArray FmodEventDescription::get_parameter_labels_by_name(const String& parameter_name) const {
    PackedStringArray labels;

    Ref<FmodParameterDescription> parameter {get_parameter_by_name(parameter_name)};
    if (!parameter->is_labeled()) {
        return labels;
    }

    for (int i = 0; i <= static_cast<int>(parameter->get_maximum()); ++i) {
        labels.append(get_parameter_label_by_name(parameter_name, i));
    }

    return labels;
}

PackedStringArray FmodEventDescription::get_parameter_labels_by_index(int index) const {
    PackedStringArray labels;

    Ref<FmodParameterDescription> parameter {get_parameter_by_index(index)};
    if (!parameter->is_labeled()) {
        return labels;
    }

    for (int i = 0; i <= static_cast<int>(parameter->get_maximum()); ++i) {
        labels.append(get_parameter_label_by_index(index, i));
    }

    return labels;
}

Dictionary FmodEventDescription::get_user_property(const String& name) {
    Dictionary propDesc;
    FMOD_STUDIO_USER_PROPERTY
    uProp;
    if (ERROR_CHECK_WITH_REASON(_wrapped->getUserProperty(name.utf8().get_data(), &uProp),
                                vformat("Cannot get user property %s for event %s with guid %s", name, get_path(), get_guid_as_string()))) {
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
    ERROR_CHECK_WITH_REASON(_wrapped->getUserPropertyCount(&count),
                vformat("Cannot get user property count for event %s with guid %s", get_path(), get_guid_as_string()));
    return count;
}

Dictionary FmodEventDescription::user_property_by_index(int index) {
    Dictionary propDesc;
    FMOD_STUDIO_USER_PROPERTY
    uProp;
    if (ERROR_CHECK_WITH_REASON(_wrapped->getUserPropertyByIndex(index, &uProp),
                                vformat("Cannot get user property with index %d for event %s with guid %s", index, get_path(), get_guid_as_string()))) {
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