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

        T get(const int id) {
            if(this->find(id) > -1) {
                return (T) ((uint64_t) Array::operator[](id));
            }
            return nullptr;
        }
    };

    template <typename K, typename V>
    class Map: public Dictionary{
    public:
        V get(const K key) {
            if(this->has(key)) {
                return (V) ((uint64_t) Dictionary::operator[](key));
            }
            return nullptr;
        }
    };
}

#endif //GODOTFMOD_POINTERARRAY_H
