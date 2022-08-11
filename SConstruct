#!/usr/bin/env python
import os
import sys
from glob import glob
from pathlib import Path

def add_sources(sources, dir, extension):
    for f in os.listdir(dir):
        if f.endswith('.' + extension):
            sources.append(dir + '/' + f)

if sys.version_info < (3,):
    def decode_utf8(x):
        return x
else:
    import codecs
    def decode_utf8(x):
        return codecs.utf_8_decode(x)[0]

# TODO: Do not copy environment after godot-cpp/test is updated <https://github.com/godotengine/godot-cpp/blob/master/test/SConstruct>.
env = SConscript("godot-cpp/SConstruct")

# Add source files.
env.Append(CPPPATH=["src/"])
sources = Glob("src/*.cpp")

(extension_path,) = glob("demo/addons/fmod/*.gdextension")

# Find the addon path (e.g. project/addons/example).
addon_path = Path(extension_path).parent

# Find the extension name from the gdextension file (e.g. example).
extension_name = Path(extension_path).stem

# TODO: Cache is disabled currently.
# scons_cache_path = os.environ.get("SCONS_CACHE")
# if scons_cache_path != None:
#     CacheDir(scons_cache_path)
#     print("Scons cache enabled... (path: '" + scons_cache_path + "')")

if env["platform"] == "osx":
    library = env.SharedLibrary(
        "{}/bin/lib{}.{}.{}.framework/{1}.{2}.{3}".format(
            addon_path,
            extension_name,
            env["platform"],
            env["target"],
        ),
        source=sources,
    )
else:
    library = env.SharedLibrary(
        "{}/bin/lib{}.{}.{}.{}{}".format(
            addon_path,
            extension_name,
            env["platform"],
            env["target"],
            env["arch_suffix"],
            env["SHLIBSUFFIX"],
        ),
        source=sources,
    )

Default(library)

opts = Variables([], ARGUMENTS)

opts.Add(PathVariable(
    'cpp_bindings_dir',
    'Path to the cpp binding library',
    "godot-cpp/",
    PathVariable.PathIsDir
))
opts.Add(
    'fmod_lib_dir',
    'Path to the FMOD library',
    "../libs/fmod/"
)

opts.Update(env)
Help(opts.GenerateHelpText(env))

cpp_bindings_libname = 'libgodot-cpp.{}.{}.{}'.format(
                        env['platform'],
                        env['target'],
                        env["arch_suffix"])

lfix = ""
if env["target"] == "debug":
    lfix = "L"
platform = env['platform']
if platform == "osx":
    libfmod = 'libfmod%s.dylib' % lfix
    libfmodstudio = 'libfmodstudio%s.dylib' % lfix
    cpp_bindings_libname += '.a'
    env.Append(CPPPATH=[env['headers_dir'], env['cpp_bindings_dir'] + 'include/', env['cpp_bindings_dir'] + 'include/core/',
               env['cpp_bindings_dir'] + 'include/gen/', env['fmod_lib_dir'] + 'osx/core/inc/', env['fmod_lib_dir'] + 'osx/studio/inc/'])
    env.Append(LIBS=[cpp_bindings_libname, libfmod, libfmodstudio])
    env.Append(LIBPATH=[ env['cpp_bindings_dir'] + 'bin/', env['fmod_lib_dir'] + 'osx/core/lib/', env['fmod_lib_dir'] + 'osx/studio/lib/'])
elif platform == "linux":
    libfmod = 'libfmod%s.so'% lfix
    libfmodstudio = 'libfmodstudio%s.so'% lfix

    env.Append(CPPPATH=[env['headers_dir'], env['cpp_bindings_dir'] + 'include/', env['cpp_bindings_dir'] + 'include/core/',
               env['cpp_bindings_dir'] + 'include/gen/', env['fmod_lib_dir'] + 'linux/core/inc/', env['fmod_lib_dir'] + 'linux/studio/inc/'])
    env.Append(LIBS=[cpp_bindings_libname, libfmod, libfmodstudio])
    env.Append(LIBPATH=[ env['cpp_bindings_dir'] + 'bin/', env['fmod_lib_dir'] + 'linux/core/lib/' + env["arch_suffix"], env['fmod_lib_dir'] + 'linux/studio/lib/' + env["arch_suffix"]])
elif platform == "windows":
    cpp_bindings_libname += '.lib'
    libfmod = 'fmod%s_vc'% lfix
    libfmodstudio = 'fmodstudio%s_vc'% lfix

    fmod_info_table = {
        "x86_64" : "x64",
        "x86_32" : "x86",
    }
    arch_suffix_override = fmod_info_table[env["arch_suffix"]]
    env.Append(CPPPATH=[env['headers_dir'], env['cpp_bindings_dir'] + 'include/', env['cpp_bindings_dir'] + 'include/core/',
                        env['cpp_bindings_dir'] + 'include/gen/', env['fmod_lib_dir'] + 'windows/core/inc/', env['fmod_lib_dir'] + 'windows/studio/inc/'])
    env.Append(LIBS=[cpp_bindings_libname, libfmod, libfmodstudio])
    env.Append(LIBPATH=[ env['cpp_bindings_dir'] + 'bin/', env['fmod_lib_dir'] + 'windows/core/lib/' + arch_suffix_override, env['fmod_lib_dir'] + 'windows/studio/lib/' + arch_suffix_override])
elif platform == "ios":
    cpp_bindings_libname += '.a'
    libfmod = 'libfmod%s_iphoneos.a' % lfix
    libfmodstudio = 'libfmodstudio%s_iphoneos.a' % lfix
    env.Append(CPPPATH=[env['headers_dir'], env['cpp_bindings_dir'] + 'include/', env['cpp_bindings_dir'] + 'include/core/',
                        env['cpp_bindings_dir'] + 'include/gen/', env['fmod_lib_dir'] + 'ios/core/inc/', env['fmod_lib_dir'] + 'ios/studio/inc/'])
    env.Append(LIBS=[cpp_bindings_libname, libfmod, libfmodstudio])
    env.Append(LIBPATH=[ env['cpp_bindings_dir'] + 'bin/', env['fmod_lib_dir'] + 'ios/core/lib/', env['fmod_lib_dir'] + 'ios/studio/lib/'])
elif platform == "android":
    cpp_bindings_libname += '.a'
    libfmod = 'libfmod%.so' % lfix
    libfmodstudio = 'libfmodstudio%.so' % lfix
    env.Append(CPPPATH=[env['headers_dir'], env['cpp_bindings_dir'] + 'include/', env['cpp_bindings_dir'] + 'include/core/',
                        env['cpp_bindings_dir'] + 'include/gen/', env['fmod_lib_dir'] + 'android/core/inc/', env['fmod_lib_dir'] + 'android/studio/inc/'])
    env.Append(LIBS=[cpp_bindings_libname, libfmod, libfmodstudio])
    env.Append(LIBPATH=[ env['cpp_bindings_dir'] + 'bin/', env['fmod_lib_dir'] + 'android/core/lib/' + env["arch_suffix"], env['fmod_lib_dir'] + 'android/studio/lib/' + env["arch_suffix"]])

sources = []
add_sources(sources, "./src", 'cpp')
add_sources(sources, "./src/callback", 'cpp')

###############
#BUILD LIB#####
###############

# determine to link as shared or static library

lib_name_without_bin = 'libGodotFmod.{}.{}.{}'.format(
    env['platform'],
    env['target'],
    env["arch_suffix"])

lib_name = 'bin/' + lib_name_without_bin

if platform == "osx":
    lib_path = lib_name + '.dylib'
    lib_name_dylib = lib_name_without_bin + '.dylib'
    def change_id(self, arg, env, executor = None):
        sys_exec(["install_name_tool", "-id", "@rpath/%s" % lib_name_dylib , lib_path])
        sys_exec(["install_name_tool", "-change", "@rpath/libfmodstudio.dylib", "@loader_path/libfmodstudio.dylib", lib_path])
        sys_exec(["install_name_tool", "-change", "@rpath/libfmod.dylib", "@loader_path/libfmod.dylib", lib_path])
    library = env.SharedLibrary(target=lib_path, source=sources)
    change_id_action = Action('', change_id)

    AddPostAction(library, change_id_action)
elif platform == "android":
    library = env.SharedLibrary(target=lib_name +".dll", source=sources)
elif platform == "linux":
    library = env.SharedLibrary(target=lib_name +".so", source=sources)
elif platform == "windows":
    library = env.SharedLibrary(target=lib_name +".dll", source=sources)
elif platform == "ios":
    library = env.StaticLibrary(target=lib_name +".a", source=sources)
Default(library)