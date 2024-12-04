#ifndef GODOTFMOD_FMOD_PLUGINS_SETTINGS_H
#define GODOTFMOD_FMOD_PLUGINS_SETTINGS_H


#include <classes/resource.hpp>

namespace godot {
    class FmodPluginsSettings : public Resource {
        GDCLASS(FmodPluginsSettings, Resource)

    public:
        void set_plugins_base_path(const String& p_base_path);
        const String& get_plugins_base_path() const;

        void set_dynamic_plugin_list(const PackedStringArray& p_dynamic_plugin_list);
        const PackedStringArray& get_dynamic_plugin_list() const;

        static Ref<FmodPluginsSettings> get_from_project_settings();

        FmodPluginsSettings() = default;
        ~FmodPluginsSettings() = default;

        static constexpr const char* PLUGINS_SETTINGS_BASE_PATH = "Plugins";

        static constexpr const char* BASE_PATH_OPTION = "base_path";
        static constexpr const char* DYNAMIC_PLUGIN_LIST_OPTION = "dynamic_plugin_list";

        static constexpr const char* DEFAULT_BASE_PATH = "res://";

    private:
        String _plugins_base_path;
        PackedStringArray _dynamic_plugin_list;

    protected:
        static void _bind_methods();
    };
}


#endif //GODOTFMOD_FMOD_PLUGINS_SETTINGS_H
