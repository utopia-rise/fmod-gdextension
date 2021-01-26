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
                return value;
            }
            return nullptr;
        }

        T get(const int id) {
            return (T) ((uint64_t) Array::operator[](id));
        }

        uint64_t append(const T value){
            const auto var = (uint64_t) value;
            Array::append(var);
            return var;
        }

        bool has(const T value) {
            const auto var = (uint64_t) value;
            return Array::has(var);
        }

        void erase(const T value){
            const auto var = (uint64_t) value;
            Array::erase(var);
        }

        T pop_front_value(){
            return (T) ((uint64_t) Array::pop_front());
        }

        void push_front_value(const T &v){
            Array::push_front((uint64_t) v);
        }

        void push_back_value(const T &v){
            Array::push_back((uint64_t) v);
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

        friend void operator<<<V>(Variant &var, V &value);
    };

}

#endif //GODOTFMOD_POINTERARRAY_H
