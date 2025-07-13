#ifdef TOOLS_ENABLED

#ifndef GODOTFMOD_FMOD_EDITOR_PLUGIN_H
#define GODOTFMOD_FMOD_EDITOR_PLUGIN_H

#include "fmod_editor_export_plugin.h"
#include "studio/fmod_bank.h"

#include <classes/editor_plugin.hpp>

namespace godot {
    class FmodEditorPlugin : public EditorPlugin {
        GDCLASS(FmodEditorPlugin, EditorPlugin)
    public:
        void _ready() override;

        FmodEditorPlugin() = default;
        ~FmodEditorPlugin() = default;

    private:
        static void add_setting(
          const String& p_name,
          const Variant& p_default_value,
          Variant::Type p_type,
          PropertyHint p_hint = PROPERTY_HINT_NONE,
          const String& p_hint_string = "",
          int p_usage = PROPERTY_USAGE_DEFAULT
        );

    protected:
        static void _bind_methods();
    };
}

#endif// GODOTFMOD_FMOD_EDITOR_PLUGIN_H

#endif
