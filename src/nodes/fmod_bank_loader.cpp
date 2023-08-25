#include "fmod_bank_loader.h"

#include "fmod_server.h"

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

void godot::FmodBankLoader::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_bank_paths", "p_paths"), &FmodBankLoader::set_bank_paths);
    ClassDB::bind_method(D_METHOD("get_bank_paths"), &FmodBankLoader::get_bank_paths);

    ADD_PROPERTY(
      PropertyInfo(
        Variant::ARRAY,
        "bank_paths",
        PROPERTY_HINT_NONE,
        "",
        PROPERTY_USAGE_DEFAULT
      ),
      "set_bank_paths",
      "get_bank_paths"
    );
}
