#ifndef GODOTFMOD_COMMON_H
#define GODOTFMOD_COMMON_H

#include "classes/canvas_item.hpp"
#include "classes/node3d.hpp"
#include "fmod_common.h"
#include "variant/utility_functions.hpp"
#include <fmod_errors.h>
#include <helpers/current_function.h>

#define MAX_PATH_SIZE 512
#define MAX_DRIVER_NAME_SIZE 256

#define MAX_VCA_COUNT 64
#define MAX_BUS_COUNT 64
#define MAX_EVENT_DESCRIPTION_COUNT 256
#define MAX_EVENT_INSTANCE_COUNT 128

#define GODOT_LOG(level, message)                                                                \
    switch (level) {                                                                             \
        case 0:                                                                                  \
            UtilityFunctions::print(message);                                                    \
            break;                                                                               \
        case 1:                                                                                  \
            UtilityFunctions::push_warning(message, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__); \
            break;                                                                               \
        case 2:                                                                                  \
            UtilityFunctions::push_error(message, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__);   \
            break;                                                                               \
    }

#define CHECK_SIZE(maxSize, actualSize, type)                                                                                                                  \
    if ((actualSize) > (maxSize)) {                                                                                                                            \
        String message = "FMOD Sound System: type maximum size is " + String::num(maxSize) + " but the bank contains " + String::num(actualSize) + " entries"; \
        GODOT_LOG(2, message)                                                                                                                                  \
        (actualSize) = maxSize;                                                                                                                                \
    }

#define ERROR_CHECK(_result) checkErrors(_result, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__)

#define GDTEMPLATE(m_class, m_inherits)                                                                                                                                                   \
private:                                                                                                                                                                               \
	void operator=(const m_class &p_rval) {}                                                                                                                                           \
	friend class ::godot::ClassDB;                                                                                                                                                     \
                                                                                                                                                                                       \
protected:                                                                                                                                                                             \
	virtual const ::godot::StringName *_get_extension_class_name() const override {                                                                                                    \
		static ::godot::StringName string_name = get_class_static();                                                                                                                   \
		return &string_name;                                                                                                                                                           \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	virtual const GDExtensionInstanceBindingCallbacks *_get_bindings_callbacks() const override {                                                                                      \
		return &___binding_callbacks;                                                                                                                                                  \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static void (*_get_bind_methods())() {                                                                                                                                             \
		return &m_class::_bind_methods;                                                                                                                                                \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static void (::godot::Wrapped::*_get_notification())(int) {                                                                                                                        \
		return (void(::godot::Wrapped::*)(int)) & m_class::_notification;                                                                                                              \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static bool (::godot::Wrapped::*_get_set())(const ::godot::StringName &p_name, const ::godot::Variant &p_property) {                                                               \
		return (bool(::godot::Wrapped::*)(const ::godot::StringName &p_name, const ::godot::Variant &p_property)) & m_class::_set;                                                     \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static bool (::godot::Wrapped::*_get_get())(const ::godot::StringName &p_name, ::godot::Variant &r_ret) const {                                                                    \
		return (bool(::godot::Wrapped::*)(const ::godot::StringName &p_name, ::godot::Variant &r_ret) const) & m_class::_get;                                                          \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static void (::godot::Wrapped::*_get_get_property_list())(::godot::List<::godot::PropertyInfo> * p_list) const {                                                                   \
		return (void(::godot::Wrapped::*)(::godot::List<::godot::PropertyInfo> * p_list) const) & m_class::_get_property_list;                                                         \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static bool (::godot::Wrapped::*_get_property_can_revert())(const ::godot::StringName &p_name) {                                                                                   \
		return (bool(::godot::Wrapped::*)(const ::godot::StringName &p_name)) & m_class::_property_can_revert;                                                                         \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static bool (::godot::Wrapped::*_get_property_get_revert())(const ::godot::StringName &p_name, ::godot::Variant &) {                                                               \
		return (bool(::godot::Wrapped::*)(const ::godot::StringName &p_name, ::godot::Variant &)) & m_class::_property_get_revert;                                                     \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static ::godot::String (::godot::Wrapped::*_get_to_string())() {                                                                                                                   \
		return (::godot::String(::godot::Wrapped::*)()) & m_class::_to_string;                                                                                                         \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	template <class T, class B>                                                                                                                                                        \
	static void register_virtuals() {                                                                                                                                                  \
		m_inherits::template register_virtuals<T, B>();                                                                                                                                         \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
public:                                                                                                                                                                                \
	static void initialize_class() {                                                                                                                                                   \
		static bool initialized = false;                                                                                                                                               \
		if (initialized) {                                                                                                                                                             \
			return;                                                                                                                                                                    \
		}                                                                                                                                                                              \
		m_inherits::initialize_class();                                                                                                                                                \
		if (m_class::_get_bind_methods() != m_inherits::_get_bind_methods()) {                                                                                                         \
			_bind_methods();                                                                                                                                                           \
			m_inherits::template register_virtuals<m_class, m_inherits>();                                                                                                                      \
		}                                                                                                                                                                              \
		initialized = true;                                                                                                                                                            \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static ::godot::StringName &get_class_static() {                                                                                                                                   \
		static ::godot::StringName string_name = ::godot::StringName(#m_class);                                                                                                        \
		return string_name;                                                                                                                                                            \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static ::godot::StringName &get_parent_class_static() {                                                                                                                            \
		return m_inherits::get_class_static();                                                                                                                                         \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static GDExtensionObjectPtr create(void *data) {                                                                                                                                   \
		m_class *new_object = memnew(m_class);                                                                                                                                         \
		return new_object->_owner;                                                                                                                                                     \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static void notification_bind(GDExtensionClassInstancePtr p_instance, int32_t p_what) {                                                                                            \
		if (p_instance && m_class::_get_notification()) {                                                                                                                              \
			if (m_class::_get_notification() != m_inherits::_get_notification()) {                                                                                                     \
				m_class *cls = reinterpret_cast<m_class *>(p_instance);                                                                                                                \
				return cls->_notification(p_what);                                                                                                                                     \
			}                                                                                                                                                                          \
			m_inherits::notification_bind(p_instance, p_what);                                                                                                                         \
		}                                                                                                                                                                              \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static GDExtensionBool set_bind(GDExtensionClassInstancePtr p_instance, GDExtensionConstStringNamePtr p_name, GDExtensionConstVariantPtr p_value) {                                \
		if (p_instance && m_class::_get_set()) {                                                                                                                                       \
			if (m_class::_get_set() != m_inherits::_get_set()) {                                                                                                                       \
				m_class *cls = reinterpret_cast<m_class *>(p_instance);                                                                                                                \
				return cls->_set(*reinterpret_cast<const ::godot::StringName *>(p_name), *reinterpret_cast<const ::godot::Variant *>(p_value));                                        \
			}                                                                                                                                                                          \
			return m_inherits::set_bind(p_instance, p_name, p_value);                                                                                                                  \
		}                                                                                                                                                                              \
		return false;                                                                                                                                                                  \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static GDExtensionBool get_bind(GDExtensionClassInstancePtr p_instance, GDExtensionConstStringNamePtr p_name, GDExtensionVariantPtr r_ret) {                                       \
		if (p_instance && m_class::_get_get()) {                                                                                                                                       \
			if (m_class::_get_get() != m_inherits::_get_get()) {                                                                                                                       \
				m_class *cls = reinterpret_cast<m_class *>(p_instance);                                                                                                                \
				return cls->_get(*reinterpret_cast<const ::godot::StringName *>(p_name), *reinterpret_cast<::godot::Variant *>(r_ret));                                                \
			}                                                                                                                                                                          \
			return m_inherits::get_bind(p_instance, p_name, r_ret);                                                                                                                    \
		}                                                                                                                                                                              \
		return false;                                                                                                                                                                  \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static const GDExtensionPropertyInfo *get_property_list_bind(GDExtensionClassInstancePtr p_instance, uint32_t *r_count) {                                                          \
		if (p_instance && m_class::_get_get_property_list()) {                                                                                                                         \
			if (m_class::_get_get_property_list() != m_inherits::_get_get_property_list()) {                                                                                           \
				m_class *cls = reinterpret_cast<m_class *>(p_instance);                                                                                                                \
				ERR_FAIL_COND_V_MSG(!cls->plist_owned.is_empty() || cls->plist != nullptr || cls->plist_size != 0, nullptr, "Internal error, property list was not freed by engine!"); \
				cls->_get_property_list(&cls->plist_owned);                                                                                                                            \
				cls->plist = reinterpret_cast<GDExtensionPropertyInfo *>(memalloc(sizeof(GDExtensionPropertyInfo) * cls->plist_owned.size()));                                         \
				cls->plist_size = 0;                                                                                                                                                   \
				for (const ::godot::PropertyInfo &E : cls->plist_owned) {                                                                                                              \
					cls->plist[cls->plist_size].type = static_cast<GDExtensionVariantType>(E.type);                                                                                    \
					cls->plist[cls->plist_size].name = E.name._native_ptr();                                                                                                           \
					cls->plist[cls->plist_size].hint = E.hint;                                                                                                                         \
					cls->plist[cls->plist_size].hint_string = E.hint_string._native_ptr();                                                                                             \
					cls->plist[cls->plist_size].class_name = E.class_name._native_ptr();                                                                                               \
					cls->plist[cls->plist_size].usage = E.usage;                                                                                                                       \
					cls->plist_size++;                                                                                                                                                 \
				}                                                                                                                                                                      \
				if (r_count)                                                                                                                                                           \
					*r_count = cls->plist_size;                                                                                                                                        \
				return cls->plist;                                                                                                                                                     \
			}                                                                                                                                                                          \
			return m_inherits::get_property_list_bind(p_instance, r_count);                                                                                                            \
		}                                                                                                                                                                              \
		return nullptr;                                                                                                                                                                \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static void free_property_list_bind(GDExtensionClassInstancePtr p_instance, const GDExtensionPropertyInfo *p_list) {                                                               \
		if (p_instance) {                                                                                                                                                              \
			m_class *cls = reinterpret_cast<m_class *>(p_instance);                                                                                                                    \
			ERR_FAIL_COND_MSG(cls->plist == nullptr, "Internal error, property list double free!");                                                                                    \
			memfree(cls->plist);                                                                                                                                                       \
			cls->plist = nullptr;                                                                                                                                                      \
			cls->plist_size = 0;                                                                                                                                                       \
			cls->plist_owned.clear();                                                                                                                                                  \
		}                                                                                                                                                                              \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static GDExtensionBool property_can_revert_bind(GDExtensionClassInstancePtr p_instance, GDExtensionConstStringNamePtr p_name) {                                                    \
		if (p_instance && m_class::_get_property_can_revert()) {                                                                                                                       \
			if (m_class::_get_property_can_revert() != m_inherits::_get_property_can_revert()) {                                                                                       \
				m_class *cls = reinterpret_cast<m_class *>(p_instance);                                                                                                                \
				return cls->_property_can_revert(*reinterpret_cast<const ::godot::StringName *>(p_name));                                                                              \
			}                                                                                                                                                                          \
			return m_inherits::property_can_revert_bind(p_instance, p_name);                                                                                                           \
		}                                                                                                                                                                              \
		return false;                                                                                                                                                                  \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static GDExtensionBool property_get_revert_bind(GDExtensionClassInstancePtr p_instance, GDExtensionConstStringNamePtr p_name, GDExtensionVariantPtr r_ret) {                       \
		if (p_instance && m_class::_get_property_get_revert()) {                                                                                                                       \
			if (m_class::_get_property_get_revert() != m_inherits::_get_property_get_revert()) {                                                                                       \
				m_class *cls = reinterpret_cast<m_class *>(p_instance);                                                                                                                \
				return cls->_property_get_revert(*reinterpret_cast<const ::godot::StringName *>(p_name), *reinterpret_cast<::godot::Variant *>(r_ret));                                \
			}                                                                                                                                                                          \
			return m_inherits::property_get_revert_bind(p_instance, p_name, r_ret);                                                                                                    \
		}                                                                                                                                                                              \
		return false;                                                                                                                                                                  \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static void to_string_bind(GDExtensionClassInstancePtr p_instance, GDExtensionBool *r_is_valid, GDExtensionStringPtr r_out) {                                                      \
		if (p_instance && m_class::_get_to_string()) {                                                                                                                                 \
			if (m_class::_get_to_string() != m_inherits::_get_to_string()) {                                                                                                           \
				m_class *cls = reinterpret_cast<m_class *>(p_instance);                                                                                                                \
				*reinterpret_cast<::godot::String *>(r_out) = cls->_to_string();                                                                                                       \
				*r_is_valid = true;                                                                                                                                                    \
				return;                                                                                                                                                                \
			}                                                                                                                                                                          \
			m_inherits::to_string_bind(p_instance, r_is_valid, r_out);                                                                                                                 \
		}                                                                                                                                                                              \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static void free(void *data, GDExtensionClassInstancePtr ptr) {                                                                                                                    \
		if (ptr) {                                                                                                                                                                     \
			m_class *cls = reinterpret_cast<m_class *>(ptr);                                                                                                                           \
			cls->~m_class();                                                                                                                                                           \
			::godot::Memory::free_static(cls);                                                                                                                                         \
		}                                                                                                                                                                              \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static void *___binding_create_callback(void *p_token, void *p_instance) {                                                                                                         \
		return nullptr;                                                                                                                                                                \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static void ___binding_free_callback(void *p_token, void *p_instance, void *p_binding) {                                                                                           \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static GDExtensionBool ___binding_reference_callback(void *p_token, void *p_instance, GDExtensionBool p_reference) {                                                               \
		return true;                                                                                                                                                                   \
	}                                                                                                                                                                                  \
                                                                                                                                                                                       \
	static constexpr GDExtensionInstanceBindingCallbacks ___binding_callbacks = {                                                                                                      \
		___binding_create_callback,                                                                                                                                                    \
		___binding_free_callback,                                                                                                                                                      \
		___binding_reference_callback,                                                                                                                                                 \
	};




namespace godot {
    static bool checkErrors(FMOD_RESULT result, const char *function, const char *file, int line) {
        if (result != FMOD_OK) {
            UtilityFunctions::push_error(FMOD_ErrorString(result), function, file, line);
            return false;
        }
        return true;
    }

    static bool is_dead(Object *node) {
        if (!node) {
            return true;
        }
        return !UtilityFunctions::is_instance_valid(Object::cast_to<Node>(node)->get_owner());
    }

    static bool is_fmod_valid(Object *node) {
        if (node) {
            bool ret = Node::cast_to<Node3D>(node) || Node::cast_to<CanvasItem>(node);
            if (!ret) {
                GODOT_LOG(2, "Invalid Object. A listener has to be either a Node3D or CanvasItem.")
            }
            return ret;
        }
        GODOT_LOG(2, "Object is null")
        return false;
    }

    template<class T>
    static inline Ref<T> create_ref() {
        Ref<T> ref;
        ref.instantiate();
        return ref;
    }

}// namespace godot

#endif// GODOTFMOD_COMMON_H
