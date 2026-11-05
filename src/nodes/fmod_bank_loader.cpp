#include "fmod_bank_loader.h"

#include "fmod_server.h"
#include "fmod_string_names.h"

#include <classes/engine.hpp>

using namespace godot;

void FmodBankLoader::_enter_tree() {
    load_banks(banks);
}

void FmodBankLoader::_exit_tree() {
    unload_banks(banks);
}

void FmodBankLoader::load_banks(Vector<Ref<FmodBank>>& p_banks) {
#ifdef TOOLS_ENABLED
    if (Engine::get_singleton()->is_editor_hint()) { return; }
#endif
    for (int i = 0; i < bank_paths.size(); ++i) {
        Ref<FmodBank> loaded_bank = FmodServer::get_singleton()->load_bank(bank_paths[i], FmodBank::NORMAL);
        if (loaded_bank.is_null()) { continue; }
        if (preload_sample_data) { loaded_bank->load_sample_data(); }
        p_banks.append(loaded_bank);
    }
}

void FmodBankLoader::unload_banks(Vector<Ref<FmodBank>>& p_banks) {
    if (preload_sample_data) {
        for (const Ref<FmodBank>& loaded_bank : p_banks) {
            loaded_bank->unload_sample_data();
        }
    }
    p_banks.clear();
}

void FmodBankLoader::set_bank_paths(const Array& p_paths) {
    bank_paths = p_paths;
    if (!is_inside_tree()) { return; }
    Vector<Ref<FmodBank>> previous_banks = banks;
    banks.clear();
    load_banks(banks);
    unload_banks(previous_banks);
}

void FmodBankLoader::set_preload_sample_data(bool p_preload_sample_data) {
    if (preload_sample_data == p_preload_sample_data) { return; }
    preload_sample_data = p_preload_sample_data;
    for (const Ref<FmodBank>& loaded_bank : banks) {
        if (preload_sample_data) {
            loaded_bank->load_sample_data();
        } else {
            loaded_bank->unload_sample_data();
        }
    }
}

bool FmodBankLoader::is_preload_sample_data() const {
    return preload_sample_data;
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
    ClassDB::bind_method(D_METHOD("set_bank_paths", "paths"), &FmodBankLoader::set_bank_paths);
    ClassDB::bind_method(D_METHOD("get_bank_paths"), &FmodBankLoader::get_bank_paths);
    ClassDB::bind_method(
        D_METHOD("set_preload_sample_data", "preload_sample_data"),
        &FmodBankLoader::set_preload_sample_data
    );
    ClassDB::bind_method(D_METHOD("is_preload_sample_data"), &FmodBankLoader::is_preload_sample_data);

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
    ADD_PROPERTY(
        PropertyInfo(Variant::BOOL, "preload_sample_data"),
        "set_preload_sample_data",
        "is_preload_sample_data"
    );
}
