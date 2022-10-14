#!/usr/bin/env python
import os
import sys

target_path = ARGUMENTS.pop("target_path", "demo/addons/fmod/bin/")
target_name = ARGUMENTS.pop("target_name", "libGodotFmod")
fmod_lib_dir = ARGUMENTS.pop("fmod_lib_dir", "../libs/fmod/")

env = SConscript("godot-cpp/SConstruct")

# Replace the source directory so we can include godot_cpp/ in them when possible
# Quite hacky. A better would be nice if Scons API allows
new_cpppath = []
for directories in env["CPPPATH"]:
  new_directories = []
  for directory in directories:
    new_directory = env.Dir(os.path.join(directory.get_path(), "godot_cpp"))
    new_directories.append(new_directory if new_directory.exists() else directory)
  new_cpppath.append(new_directories)
env.Append(CPPPATH=new_cpppath)

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
sources = [Glob('src/*.cpp'), Glob('src/callback/*.cpp'), Glob('src/nodes/*.cpp')]

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
    env.Append(LIBPATH=[env['fmod_lib_dir'] + 'linux/core/lib/' + env["arch"], env['fmod_lib_dir'] + 'linux/studio/lib/' + env["arch"]])

elif env["platform"] == "windows":
    libfmod = 'fmod%s_vc'% lfix
    libfmodstudio = 'fmodstudio%s_vc'% lfix
    fmod_info_table = {
        "x86_64" : "x64",
        "x86_32" : "x64",
    }
    arch_suffix_override = fmod_info_table[env["arch"]]
    env.Append(CPPPATH=[env['fmod_lib_dir'] + 'windows/core/inc/', env['fmod_lib_dir'] + 'windows/studio/inc/'])
    env.Append(LIBS=[libfmod, libfmodstudio])
    env.Append(LIBPATH=[env['fmod_lib_dir'] + 'windows/core/lib/' + arch_suffix_override, env['fmod_lib_dir'] + 'windows/studio/lib/' + arch_suffix_override])

elif env["platform"] == "ios":
    libfmod = 'libfmod%s_iphoneos.a' % lfix
    libfmodstudio = 'libfmodstudio%s_iphoneos.a' % lfix
    env.Append(CPPPATH=[env['fmod_lib_dir'] + 'ios/core/inc/', env['fmod_lib_dir'] + 'ios/studio/inc/'])
    env.Append(LIBS=[libfmod, libfmodstudio])
    env.Append(LIBPATH=[env['fmod_lib_dir'] + 'ios/core/lib/', env['fmod_lib_dir'] + 'ios/studio/lib/'])

elif env["platform"] == "android":
    libfmod = 'libfmod%s.so' % lfix
    libfmodstudio = 'libfmodstudio%s.so' % lfix
    fmod_info_table = {
        "armv7": "armeabi-v7a",
        "arm64": "arm64-v8a",
        "x86": "x86",
        "x86_64": "x86_64"
    }
    arch_dir = fmod_info_table[env["arch"]]
    env.Append(CPPPATH=[env['fmod_lib_dir'] + 'android/core/inc/', env['fmod_lib_dir'] + 'android/studio/inc/'])
    env.Append(LIBS=[libfmod, libfmodstudio])
    env.Append(LIBPATH=[env['fmod_lib_dir'] + 'android/core/lib/' + arch_dir, env['fmod_lib_dir'] + 'android/studio/lib/' + arch_dir])

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
        env["arch"],
        env["SHLIBSUFFIX"]
    )

library = env.SharedLibrary(target=target, source=sources)
Default(library)