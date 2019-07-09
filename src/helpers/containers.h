//
// Created by Pierre-Thomas Meisels on 2019-07-08.
//

#ifndef GODOTFMOD_POINTERARRAY_H
#define GODOTFMOD_POINTERARRAY_H

#include <core/Godot.hpp>
#include <core/Array.hpp>
#include "current_function.h"

namespace godot {

    template <typename T>
    class Vector: public Array {
    public:
        T get(const uint64_t value) {
            if(this->find(value) > -1) {
                return (T) value;
            }
            return nullptr;
        }

        T get(const T value) {
            if(this->find((uint64_t) value) > -1) {
                return T;
            }
            return nullptr;
        }

        T get(const int id) {
            if(this->find(id) > -1) {
                return (T) ((uint64_t) Array::operator[](id));
            }
            return nullptr;
        }

        uint64_t append(const T value){
            const uint64_t var = (uint64_t) value;
            Array::append(var);
            return var;
        }

        uint64_t erase(const T value){
            const uint64_t var = (uint64_t) value;
            Array::erase(var);
            return var;
        }

    };

    template <typename V> void operator<<(Variant &var, V &value){
        var = (uint64_t) value;
    }

    template <typename K, typename V>
    class Map: public Dictionary{
    public:
        V get(const K key) {
            if(this->has(key)) {
                return (V) ((uint64_t) Dictionary::operator[](key));
            }
            return nullptr;
        }

        friend void  operator<<<V>(Variant &var, V &value);
    };

}

#endif //GODOTFMOD_POINTERARRAY_H
