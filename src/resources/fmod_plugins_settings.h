#ifndef GODOTFMOD_FMOD_PLUGINS_SETTINGS_H
#define GODOTFMOD_FMOD_PLUGINS_SETTINGS_H


#include <classes/resource.hpp>

namespace godot {
    class FmodStaticPluginMethod : public Resource {
        GDCLASS(FmodStaticPluginMethod, Resource)

    public:
        enum Type {
            CODEC,
            DSP,
            OUTPUT,

            COUNT
        };

        void set_type(Type p_type);
        Type get_type() const;

        void set_method_name(const String& p_method_name);
        const String& get_method_name() const;

    private:
        Type _type = Type::CODEC;
        String _method_name;

    protected:
        static void _bind_methods();
    };

    class FmodPluginsSettings : public Resource {
        GDCLASS(FmodPluginsSettings, Resource)

    public:
        void set_plugins_base_path(const String& p_base_path);
        const String& get_plugins_base_path() const;

        void set_dynamic_plugin_list(const PackedStringArray& p_dynamic_plugin_list);
        const PackedStringArray& get_dynamic_plugin_list() const;

        void set_static_plugins_methods(const Array& p_static_plugins_settings);
        const Array& get_static_plugins_methods() const;

        static Ref<FmodPluginsSettings> get_from_project_settings();

        FmodPluginsSettings() = default;
        ~FmodPluginsSettings() = default;

        static constexpr const char* PLUGINS_SETTINGS_BASE_PATH = "Plugins";

        static constexpr const char* RESOURCE_OPTION = "path_to_plugin_configuration";
        static constexpr const char* DEFAULT_RESOURCE_OPTION = "";

    private:
        String _plugins_base_path;
        PackedStringArray _dynamic_plugin_list;
        Array _static_plugins_methods;

    protected:
        static void _bind_methods();
    };
}

VARIANT_ENUM_CAST(godot::FmodStaticPluginMethod::Type);


#endif //GODOTFMOD_FMOD_PLUGINS_SETTINGS_H
