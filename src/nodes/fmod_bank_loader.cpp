#include "fmod_bank_loader.h"

#include "fmod_server.h"
#include "fmod_string_names.h"

#include <classes/engine.hpp>

using namespace godot;

void FmodBankLoader::_enter_tree() {
#ifdef TOOLS_ENABLED
    if (Engine::get_singleton()->is_editor_hint()) {
        return;
    }
#endif
    for (int i = 0; i < bank_paths.size(); ++i) {
        bank.append(
          FmodServer::get_singleton()->load_bank(bank_paths[i], FMOD_STUDIO_LOAD_BANK_NORMAL)
        );
    }
}

void FmodBankLoader::set_bank_paths(const Array& p_paths) {
    bank_paths = p_paths;
}

const Array& FmodBankLoader::get_bank_paths() const {
    return bank_paths;
}

bool FmodBankLoader::_property_can_revert(const StringName& p_property) const {
    if (p_property == FmodStringNames::get_instance()->bank_path_property_name) {
        return true;
    }

    return false;
}

bool FmodBankLoader::_property_get_revert(const StringName& p_property, Variant& result) const {
    if (p_property != FmodStringNames::get_instance()->bank_path_property_name) {
        return false;
    }

    result = Array();
    return true;
}

void godot::FmodBankLoader::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_bank_paths", "p_paths"), &FmodBankLoader::set_bank_paths);
    ClassDB::bind_method(D_METHOD("get_bank_paths"), &FmodBankLoader::get_bank_paths);

    ADD_PROPERTY(
      PropertyInfo(
        Variant::ARRAY,
        FmodStringNames::get_instance()->bank_path_property_name,
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_bank_paths",
      "get_bank_paths"
    );
}