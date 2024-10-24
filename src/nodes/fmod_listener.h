#ifndef GODOTFMOD_FMOD_LISTENER_H
#define GODOTFMOD_FMOD_LISTENER_H

#include <fmod_server.h>

#include <classes/engine.hpp>

namespace godot {
    template<class Derived, class NodeType>
    class FmodListener : public NodeType {

        void ready();
        void exit_tree();

    public:
        void _notification(int p_what);

        void set_listener_index(const int index);
        int get_listener_index() const;

        void set_locked(const bool locked);
        bool get_locked() const;

        void set_listener_weight(const float p_weight);
        float get_listener_weight() const;

        static const StringName& get_class_static();

        FmodListener();
        ~FmodListener() = default;

    private:
        float _weight;
        int _listener_index;
        bool _is_locked;

        bool _is_added;

    protected:
        static void _bind_methods();
    };

    template<class Derived, class NodeType>
    void FmodListener<Derived, NodeType>::_notification(int p_what) {
#ifdef TOOLS_ENABLED
        // ensure we only run FMOD when the game is running!
        if (Engine::get_singleton()->is_editor_hint()) { return; }
#endif
        switch(p_what){
            case Node::NOTIFICATION_READY:
                ready();
                break;
            case Node::NOTIFICATION_EXIT_TREE:
                exit_tree();
                break;
            default:
                break;
        }
    }

    template<class Derived, class NodeType>
    void FmodListener<Derived, NodeType>::ready() {
#ifdef TOOLS_ENABLED
        // ensure we only run FMOD when the game is running!
        if (Engine::get_singleton()->is_editor_hint()) { return; }
#endif

        FmodServer::get_singleton()->add_listener(_listener_index, this);
        FmodServer::get_singleton()->set_listener_lock(_listener_index, _is_locked);

        _is_added = true;
    }

    template<class Derived, class NodeType>
    void FmodListener<Derived, NodeType>::exit_tree() {
#ifdef TOOLS_ENABLED
        // ensure we only run FMOD when the game is running!
        if (Engine::get_singleton()->is_editor_hint()) { return; }
#endif

        FmodServer::get_singleton()->remove_listener(_listener_index, this);
        _is_added = false;
    }

    template<class Derived, class NodeType>
    void FmodListener<Derived, NodeType>::set_listener_index(const int index) {
        _listener_index = index;
    }

    template<class Derived, class NodeType>
    int FmodListener<Derived, NodeType>::get_listener_index() const {
        return _listener_index;
    }

    template<class Derived, class NodeType>
    void FmodListener<Derived, NodeType>::set_locked(const bool locked) {
        _is_locked = locked;

        if (!_is_added) {
            return;
        }

        FmodServer::get_singleton()->set_listener_lock(_listener_index, locked);
    }

    template<class Derived, class NodeType>
    bool FmodListener<Derived, NodeType>::get_locked() const {
        if (!_is_added) {
            return _is_locked;
        }

        return FmodServer::get_singleton()->get_listener_lock(_listener_index);
    }

    template<class Derived, class NodeType>
    void FmodListener<Derived, NodeType>::set_listener_weight(const float p_weight) {
        _weight = p_weight;

        if (!_is_added) {
            return;
        }

        FmodServer::get_singleton()->set_system_listener_weight(_listener_index, p_weight);
    }

    template<class Derived, class NodeType>
    float FmodListener<Derived, NodeType>::get_listener_weight() const {
        if (!_is_added) {
            return _weight;
        }

        return FmodServer::get_singleton()->get_system_listener_weight(_listener_index);
    }

    template<class Derived, class NodeType>
    FmodListener<Derived, NodeType>::FmodListener() :
      NodeType(),
      _weight(1.0),
      _listener_index(0),
      _is_locked(false),
      _is_added(false) {}

    template<class Derived, class NodeType>
    void FmodListener<Derived, NodeType>::_bind_methods() {
        ClassDB::bind_method(D_METHOD("set_listener_index", "index"), &Derived::set_listener_index);
        ClassDB::bind_method(D_METHOD("get_listener_index"), &Derived::get_listener_index);
        ClassDB::bind_method(D_METHOD("set_locked", "locked"), &Derived::set_locked);
        ClassDB::bind_method(D_METHOD("get_locked"), &Derived::get_locked);
        ClassDB::bind_method(D_METHOD("set_listener_weight", "p_weight"), &Derived::set_listener_weight);
        ClassDB::bind_method(D_METHOD("get_listener_weight"), &Derived::get_listener_weight);

        ADD_PROPERTY(
          PropertyInfo(
            Variant::INT,
            "listener_index",
            PROPERTY_HINT_NONE,
            "",
            PROPERTY_USAGE_EDITOR
          ),
          "set_listener_index",
          "get_listener_index"
        );
        ADD_PROPERTY(
          PropertyInfo(
            Variant::BOOL,
            "is_locked",
            PROPERTY_HINT_NONE,
            "",
            PROPERTY_USAGE_DEFAULT
          ),
          "set_locked",
          "get_locked"
        );
        ADD_PROPERTY(
          PropertyInfo(
            Variant::FLOAT,
            "weight",
            PROPERTY_HINT_NONE,
            "",
            PROPERTY_USAGE_DEFAULT
          ),
          "set_listener_weight",
          "get_listener_weight"
        );
    }

    template<class Derived, class NodeType>
    const StringName& FmodListener<Derived, NodeType>::get_class_static() {
        return Derived::get_class_static();
    }
}// namespace godot

#endif// GODOTFMOD_FMOD_LISTENER_H
