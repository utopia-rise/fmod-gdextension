#ifdef TOOLS_ENABLED

#ifndef GODOTFMOD_FMOD_EDITOR_EXPORT_PLUGIN_H
#define GODOTFMOD_FMOD_EDITOR_EXPORT_PLUGIN_H

#include <classes/editor_export_plugin.hpp>

namespace godot {
    class FmodEditorExportPlugin : public EditorExportPlugin {
        GDCLASS(FmodEditorExportPlugin, EditorExportPlugin)

    public:
        void _export_begin(const PackedStringArray &features, bool is_debug, const String &path, uint32_t flags) override;
        String _get_name() const override;

        static void _bind_methods();

        FmodEditorExportPlugin() = default;
        ~FmodEditorExportPlugin() = default;
    };
}

#endif// GODOTFMOD_FMOD_EDITOR_EXPORT_PLUGIN_H

#endif
