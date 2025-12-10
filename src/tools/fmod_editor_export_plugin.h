#ifdef TOOLS_ENABLED

#ifndef GODOTFMOD_FMOD_EDITOR_EXPORT_PLUGIN_H
#define GODOTFMOD_FMOD_EDITOR_EXPORT_PLUGIN_H

#include <resources/fmod_plugins_settings.h>
#include <godot_cpp/classes/editor_export_platform.hpp>
#include <classes/editor_export_plugin.hpp>

namespace godot {
    class FmodEditorExportPlugin : public EditorExportPlugin {
        GDCLASS(FmodEditorExportPlugin, EditorExportPlugin)

    public:
        void _export_begin(const PackedStringArray &features, bool is_debug, const String &path, uint32_t flags) override;
        String _get_name() const override;
        virtual TypedArray<Dictionary> _get_export_options(const Ref<EditorExportPlatform>& platform) const override;

        static void _bind_methods();

        FmodEditorExportPlugin() = default;
        ~FmodEditorExportPlugin() = default;

    private:
        static PackedStringArray _get_libraries_to_export(const Ref<FmodPluginsSettings>& settings, const String& p_os_name, const String& p_extension, const String& p_arch = "");
    };
}

#endif// GODOTFMOD_FMOD_EDITOR_EXPORT_PLUGIN_H

#endif
