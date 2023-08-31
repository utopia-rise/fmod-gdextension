#ifndef GODOTFMOD_FMOD_DSP_SETTINGS_H
#define GODOTFMOD_FMOD_DSP_SETTINGS_H

#include <classes/resource.hpp>

namespace godot {
    class FmodDspSettings : public Resource {
        GDCLASS(FmodDspSettings, Resource)

    public:
        void set_dsp_buffer_size(const unsigned int p_dsp_buffer_size);
        unsigned int get_dsp_buffer_size() const;

        void set_dsp_buffer_count(const int p_dsp_buffer_count);
        int get_dsp_buffer_count() const;

        static Ref<FmodDspSettings> get_from_project_settings();

        FmodDspSettings() = default;
        ~FmodDspSettings() = default;

        static constexpr const char* DSP_SETTINGS_BASE_PATH = "DSP";

        static constexpr const char* DSP_BUFFER_SIZE_OPTION = "dsp_buffer_size";
        static constexpr const char* DSP_BUFFER_COUNT_OPTION = "dsp_buffer_count";

        static constexpr const int DEFAULT_DSP_BUFFER_SIZE = 512;
        static constexpr const int DEFAULT_DSP_BUFFER_COUNT = 4;

    private:
        unsigned int _dsp_buffer_size;
        int _dsp_buffer_count;

    protected:
        static void _bind_methods();
    };
}

#endif// GODOTFMOD_FMOD_DSP_SETTINGS_H
