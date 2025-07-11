#ifndef GODOTFMOD_PLUGINS_HELPER_H
#define GODOTFMOD_PLUGINS_HELPER_H

#include <variant/string.hpp>
#include <resources/fmod_plugins_settings.h>
#include <classes/os.hpp>

namespace godot {
    static String get_fmod_plugins_base_path(const Ref<FmodPluginsSettings>& p_settings) {
#ifdef TOOLS_ENABLED
        return p_settings->get_plugins_base_path();
#else
        return OS::get_singleton()
            ->get_executable_path()
            .get_base_dir()
    #ifdef MACOS_ENABLED
            .path_join("../PlugIns/")
#endif
            ;
#endif
    }

    static Vector<String> get_fmod_plugins_libraries_paths(const Ref<FmodPluginsSettings>& p_settings, const String& os_override = "") {
        String plugin_directory = get_fmod_plugins_base_path(p_settings);
        String os_lower_name = os_override.is_empty() ? OS::get_singleton()->get_name().to_lower() : os_override;

#ifdef TOOLS_ENABLED
        plugin_directory = plugin_directory.path_join(os_lower_name);
#endif

        String plugin_extension;
        String plugin_lib_prefix = "lib";
        if (os_lower_name == "windows") {
            plugin_extension = "dll";
            plugin_lib_prefix = "";
        } else if (os_lower_name == "macos") {
            plugin_extension = "dylib";
        } else if (os_lower_name == "ios") {
            plugin_extension = "a";
        } else {
            plugin_extension = "so";
        }

        PackedStringArray plugin_list;
        Vector<String> result;

        if (os_lower_name == "ios") {
            Array static_plugin_settings = p_settings->get_static_plugins_settings();
            for (int i = 0; i < static_plugin_settings.size(); ++i) {
                Ref<FmodStaticPluginSetting> static_setting = static_plugin_settings[i];
                plugin_list.append(static_setting->get_plugin_name());

                for (const String& dependent_library : static_setting->get_dependent_libraries()) {
                    result.append(plugin_directory.path_join(dependent_library));
                }
            }
        } else {
            plugin_list = p_settings->get_dynamic_plugin_list();
        }

        for (const String& plugin : plugin_list) {
            result.append(plugin_directory.path_join(vformat("%s%s.%s", plugin_lib_prefix, plugin, plugin_extension)));
        }

        return result;
    }
}

#endif //GODOTFMOD_PLUGINS_HELPER_H
