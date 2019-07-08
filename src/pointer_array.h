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
    class PointerArray {
    private:
        Array array;
    public:
        T operator[](uint64_t ptr) {
            return (T) ptr;
        }
        T operator[](int idx) {
            return (T) (uint64_t) array[idx];
        }

        int size() {
            return array.size();
        }
        bool has(uint64_t what) {
            return array.has((T) what);
        }
        void append(T v) {
            array.append(v);
        }
        void erase(T v) {
            array.erase(v);
        }
    };
}

#endif //GODOTFMOD_POINTERARRAY_H
