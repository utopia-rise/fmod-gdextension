cmake_minimum_required(VERSION 3.13)
project(GodotFmod)

set(CMAKE_CXX_STANDARD 14)
set(OS "windows")
set(ARCH "")

link_directories("godot-cpp/bin/")
include_directories("godot-cpp/include/")
include_directories("godot-cpp/gen/include/")
include_directories("godot-cpp/include/godot_cpp")
include_directories("godot-cpp/gen/include/godot_cpp")
include_directories("godot-cpp/godot-headers")

link_directories("../libs/fmod/${OS}/core/lib/${ARCH}")
link_directories("../libs/fmod/${OS}/studio/lib/${ARCH}")
include_directories(../libs/fmod/${OS}/core/inc ../libs/fmod/${OS}/studio/inc)

include_directories("src")

file(GLOB_RECURSE SOURCES src/*.c**)
file(GLOB_RECURSE HEADERS src/*.h**)

add_library(GodotFmod SHARED ${SOURCES} ${HEADERS})

if (${OS} EQUAL "osx")
    target_link_libraries(GodotFmod libgodot-cpp.osx.64.a libfmod.dylib libfmodstudio.dylib)
endif ()
if (${OS} EQUAL "windows")
    target_link_libraries(GodotFmod libgodot-cpp.windows.64.lib fmod64.dll fmodstudio64.dll)
endif ()
if (${OS} EQUAL "linux")
    target_link_libraries(GodotFmod libgodot-cpp.linux.64.a libfmod.so libfmodstudio.so)
endif()