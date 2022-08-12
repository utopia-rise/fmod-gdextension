#!/usr/bin/env python
import os
import sys

target_path = ARGUMENTS.pop("target_path", "demo/addons/fmod/bin/")
target_name = ARGUMENTS.pop("target_name", "libGodotFmod")
fmod_lib_dir = ARGUMENTS.pop("fmod_lib_dir", "../libs/fmod/")

env = SConscript("godot-cpp/SConstruct")
env.Replace(fmod_lib_dir = fmod_lib_dir)

target = "{}{}.{}.{}".format(
    target_path, target_name, env["platform"], env["target"]
)

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
sources = [Glob('src/*.cpp'), Glob('src/callback/*.cpp')]

lfix = ""
if env["target"] == "debug":
    lfix = "L"

if env["platform"] == "macos":
    libfmod = 'libfmod%s.dylib' % lfix
    libfmodstudio = 'libfmodstudio%s.dylib' % lfix
    env.Append(CPPPATH=[env['fmod_lib_dir'] + 'osx/core/inc/', env['fmod_lib_dir'] + 'osx/studio/inc/'])
    env.Append(LIBS=[libfmod, libfmodstudio])
    env.Append(LIBPATH=[env['fmod_lib_dir'] + 'osx/core/lib/', env['fmod_lib_dir'] + 'osx/studio/lib/'])
elif env["platform"] == "linux":
    libfmod = 'libfmod%s.so'% lfix
    libfmodstudio = 'libfmodstudio%s.so'% lfix
    env.Append(CPPPATH=[env['fmod_lib_dir'] + 'linux/core/inc/', env['fmod_lib_dir'] + 'linux/studio/inc/'])
    env.Append(LIBS=[libfmod, libfmodstudio])
    env.Append(LIBPATH=[env['fmod_lib_dir'] + 'linux/core/lib/' + env["arch_suffix"], env['fmod_lib_dir'] + 'linux/studio/lib/' + env["arch_suffix"]])
elif env["platform"] == "windows":
    libfmod = 'fmod%s_vc'% lfix
    libfmodstudio = 'fmodstudio%s_vc'% lfix
    fmod_info_table = {
        "x86_64" : "x64",
        "x86_32" : "x64",
    }
    arch_suffix_override = fmod_info_table[env["arch_suffix"]]
    env.Append(CPPPATH=[env['fmod_lib_dir'] + 'windows/core/inc/', env['fmod_lib_dir'] + 'windows/studio/inc/'])
    env.Append(LIBS=[libfmod, libfmodstudio])
    env.Append(LIBPATH=[env['fmod_lib_dir'] + 'windows/core/lib/' + arch_suffix_override, env['fmod_lib_dir'] + 'windows/studio/lib/' + arch_suffix_override])

if env["platform"] == "macos":
    target = "{}.framework/{}.{}.{}".format(
        target, 
        target_name,
        env["platform"],
        env["target"]
    )
else:
    target = "{}.{}{}".format(
        target,
        env["arch_suffix"],
        env["SHLIBSUFFIX"]
    )

library = env.SharedLibrary(target=target, source=sources)
Default(library)