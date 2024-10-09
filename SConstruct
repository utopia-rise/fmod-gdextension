#!/usr/bin/env python
import os
import shutil
import subprocess

from SCons.Script import SConscript, ARGUMENTS, Action, Copy

target_path = ARGUMENTS.pop("target_path", "demo/addons/fmod/libs/")
target_name = ARGUMENTS.pop("target_name", "libGodotFmod")
fmod_lib_dir = ARGUMENTS.pop("fmod_lib_dir", "../libs/fmod/")

env = SConscript("godot-cpp/SConstruct")

# Add those directory manually, so we can skip the godot_cpp directory when including headers in C++ files
source_path = [
    os.path.join("godot-cpp", "include","godot_cpp"),
    os.path.join("godot-cpp", "gen", "include","godot_cpp")
]
env.Append(CPPPATH=[env.Dir(d) for d in source_path])

env.Replace(fmod_lib_dir = fmod_lib_dir)

# For the reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
sources = [
    Glob('src/*.cpp'),
    Glob('src/callback/*.cpp'),
    Glob('src/core/*.cpp'),
    Glob('src/data/*.cpp'),
    Glob('src/tools/*.cpp'),
    Glob('src/helpers/*.cpp'),
    Glob('src/nodes/*.cpp'),
    Glob('src/resources/*.cpp'),
    Glob('src/studio/*.cpp')
    ]

lfix = ""
debug = False
if env["target"] == "template_debug" or env["target"] == "editor":
    lfix = "L"
    debug = True

if env["platform"] == "macos":
    libfmod = 'libfmod%s.dylib' % lfix
    libfmodstudio = 'libfmodstudio%s.dylib' % lfix

    env.Append(CPPPATH=[env['fmod_lib_dir'] + 'osx/core/inc/', env['fmod_lib_dir'] + 'osx/studio/inc/'])
    env.Append(LIBPATH=[env['fmod_lib_dir'] + 'osx/core/lib/', env['fmod_lib_dir'] + 'osx/studio/lib/'])
    env.Append(LIBS=[libfmod, libfmodstudio])

    env.Append(
        LINKFLAGS=[
            "-framework",
            "Cocoa",
            "-Wl,-undefined,dynamic_lookup",
        ]
    )

elif env["platform"] == "linux":
    libfmod = 'libfmod%s.so'% lfix
    libfmodstudio = 'libfmodstudio%s.so'% lfix

    env.Append(CPPPATH=[env['fmod_lib_dir'] + 'linux/core/inc/', env['fmod_lib_dir'] + 'linux/studio/inc/'])
    env.Append(LIBPATH=[env['fmod_lib_dir'] + 'linux/core/lib/' + env["arch"], env['fmod_lib_dir'] + 'linux/studio/lib/' + env["arch"]])
    env.Append(LIBS=[libfmod, libfmodstudio])

    env.Append(CCFLAGS=["-fPIC", "-Wwrite-strings"])
    env.Append(LINKFLAGS=["-Wl,-R,'$$ORIGIN'"])
    env.Append(LINKFLAGS=["-m64", "-fuse-ld=gold"])

elif env["platform"] == "windows":
    libfmod = 'fmod%s_vc'% lfix
    libfmodstudio = 'fmodstudio%s_vc'% lfix
    fmod_info_table = {
        "x86_64" : "x64",
        "x86_32" : "x86",
    }
    arch_suffix_override = fmod_info_table[env["arch"]]

    env.Append(CPPPATH=[env['fmod_lib_dir'] + 'windows/core/inc/', env['fmod_lib_dir'] + 'windows/studio/inc/'])
    env.Append(LIBPATH=[env['fmod_lib_dir'] + 'windows/core/lib/' + arch_suffix_override, env['fmod_lib_dir'] + 'windows/studio/lib/' + arch_suffix_override])
    env.Append(LIBS=[libfmod, libfmodstudio])

    env.Append(LINKFLAGS=["/WX"])
    if debug:
        env.Append(CCFLAGS=["/FS", "/Zi"])

elif env["platform"] == "ios":
    libfmod = 'libfmod%s_iphoneos.a' % lfix
    libfmodstudio = 'libfmodstudio%s_iphoneos.a' % lfix

    env.Append(CPPPATH=[env['fmod_lib_dir'] + 'ios/core/inc/', env['fmod_lib_dir'] + 'ios/studio/inc/'])
    env.Append(LIBPATH=[env['fmod_lib_dir'] + 'ios/core/lib/', env['fmod_lib_dir'] + 'ios/studio/lib/'])
    env.Append(LIBS=[libfmod, libfmodstudio])

    env.Append(LINKFLAGS=[
        '-Wl,-undefined,dynamic_lookup',
    ])

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
    env.Append(LIBPATH=[env['fmod_lib_dir'] + 'android/core/lib/' + arch_dir, env['fmod_lib_dir'] + 'android/studio/lib/' + arch_dir])
    env.Append(LIBS=[libfmod, libfmodstudio])

elif env["platform"] == "web":
    libfmodstudio = os.path.join(fmod_lib_dir, 'web/studio/lib/fastcomp/bitcode/', 'fmodstudio%s.bc' % lfix)

    env.Append(CPPPATH=[env['fmod_lib_dir'] + 'web/core/inc/', env['fmod_lib_dir'] + 'web/studio/inc/'])
    env.Append(LIBPATH=[env['fmod_lib_dir'] + 'web/core/lib/fastcomp/bitcode/', env['fmod_lib_dir'] + 'web/studio/lib/fastcomp/bitcode/'])
    # Instead of LIBS, directly add to LINKFLAGS for explicit paths
    env.Append(LINKFLAGS=[libfmodstudio])

#Output is placed in the addons directory of the demo project directly
target = "{}{}/{}.{}.{}".format(
    target_path, env["platform"], target_name, env["platform"], env["target"]
) if env["platform"] != "android" else "{}{}/{}/{}.{}.{}".format(
    target_path, env["platform"], env["arch"], target_name, env["platform"], env["target"]
)

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

#Necessary so the extension library can find the Fmod libraries
if env["platform"] == "macos":
    def sys_exec(args):
        proc = subprocess.Popen(args, stdout=subprocess.PIPE, text=True)
        (out, err) = proc.communicate()
        return out.rstrip("\r\n").lstrip()

    
    lib_name = "{}.{}.{}".format(
        target,
        target_name,
        env["platform"],
        env["target"]
    )

    def change_id(self, arg, env, executor = None):
        sys_exec(["install_name_tool", "-id", "@rpath/%s" % lib_name , target])
        sys_exec(["install_name_tool", "-change", "@rpath/%s" % libfmodstudio, "@loader_path/../%s" % libfmodstudio, target])
        sys_exec(["install_name_tool", "-change", "@rpath/%s" % libfmod, "@loader_path/../%s" % libfmod, target])
    change_id_action = Action('', change_id)

    AddPostAction(library, change_id_action)


def copy_fmod_libraries(self, arg, env, executor = None):
    fmod_core_lib_dir = ""
    fmod_studio_lib_dir = ""

    addon_fmod_libs_output = "{}{}/".format(
        target_path, env["platform"]
    ) if env["platform"] != "android" else "{}{}/{}/".format(
        target_path, env["platform"], env["arch"]
    )

    if env["platform"] == "macos":
        fmod_core_lib_dir = env['fmod_lib_dir'] + 'osx/core/lib/'
        fmod_studio_lib_dir = env['fmod_lib_dir'] + 'osx/studio/lib/'
    elif env["platform"] == "linux":
        fmod_core_lib_dir = env['fmod_lib_dir'] + 'linux/core/lib/' + env["arch"]
        fmod_studio_lib_dir = env['fmod_lib_dir'] + 'linux/studio/lib/' + env["arch"]
    elif env["platform"] == "windows":
        fmod_core_lib_dir = env['fmod_lib_dir'] + 'windows/core/lib/' + arch_suffix_override + '/'
        fmod_studio_lib_dir = env['fmod_lib_dir'] + 'windows/studio/lib/' + arch_suffix_override + '/'
    elif env["platform"] == "ios":
        fmod_core_lib_dir = env['fmod_lib_dir'] + 'ios/core/lib/'
        fmod_studio_lib_dir = env['fmod_lib_dir'] + 'ios/studio/lib/'
    elif env["platform"] == "android":
        fmod_core_lib_dir = env['fmod_lib_dir'] + 'android/core/lib/' + arch_dir
        fmod_studio_lib_dir = env['fmod_lib_dir'] + 'android/studio/lib/' + arch_dir

    source_files = [env.Glob(os.path.join(source_dir, '*.*')) for source_dir in [fmod_core_lib_dir, fmod_studio_lib_dir]]
    [[shutil.copy(str(file), addon_fmod_libs_output) for file in files] for files in source_files]

# web bundles everything inside the final .wasm - no need to export libs
if env["platform"] != "web":
    copy_fmod_libraries_action = Action('', copy_fmod_libraries)
    AddPostAction(library, copy_fmod_libraries_action)

Default(library)