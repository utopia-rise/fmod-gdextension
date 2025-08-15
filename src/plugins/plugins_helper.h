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

#if !defined(ANDROID_ENABLED) || defined(TOOLS_ENABLED)
    static String get_plugins_os_directory(const Ref<FmodPluginsSettings>& p_settings, const String& p_os_lower_name, const String& p_arch) {
        String plugin_directory = get_fmod_plugins_base_path(p_settings);

#ifdef TOOLS_ENABLED
        plugin_directory = plugin_directory.path_join(p_os_lower_name);

        if (!p_arch.is_empty()) {
            plugin_directory = plugin_directory.path_join(p_arch);
        }
#endif
        return plugin_directory;
    }
#endif

    static Vector<String> get_fmod_plugins_libraries_paths(const Ref<FmodPluginsSettings>& p_settings, const String& os_override = "", const String& arch = "") {
        Vector<String> result;

#if !defined(ANDROID_ENABLED) || defined(TOOLS_ENABLED)
        String os_lower_name = os_override.is_empty() ? OS::get_singleton()->get_name().to_lower() : os_override.to_lower();
        String plugin_directory = get_plugins_os_directory(p_settings, os_lower_name, arch);

        String plugin_extension;
        String plugin_lib_prefix = "lib";
        if (os_lower_name == "windows") {
            plugin_extension = "dll";
            plugin_lib_prefix = "";
        } else if (os_lower_name == "macos") {
            plugin_extension = "dylib";
        } else {
            plugin_extension = "so";
        }

        PackedStringArray plugin_list = p_settings->get_dynamic_plugin_list();

        for (const String& plugin : plugin_list) {
            result.append(plugin_directory.path_join(vformat("%s%s.%s", plugin_lib_prefix, plugin, plugin_extension)));
        }
#else
        for (const String& plugin : p_settings->get_dynamic_plugin_list()) {
            result.append(vformat("lib%s.so", plugin));
        }
#endif

        return result;
    }
}

#endif //GODOTFMOD_PLUGINS_HELPER_H
