#ifndef GODOTFMOD_FMOD_RUNTIME_H
#define GODOTFMOD_FMOD_RUNTIME_H

#include <classes/node.hpp>

namespace godot {
    // The node FmodServer adds at the root of the scene tree while FMOD runs: it updates FMOD every frame, before the
    // other nodes.
    class FmodRuntime : public Node {
        GDCLASS(FmodRuntime, Node)

    public:
        FmodRuntime();

        void _enter_tree() override;
        void _exit_tree() override;
        void _process(double p_delta) override;
        void _notification(int p_what);

    private:
        Variant get_performance_value(const StringName& p_property) const;

    protected:
        static void _bind_methods();
    };
} // namespace godot

#endif // GODOTFMOD_FMOD_RUNTIME_H
