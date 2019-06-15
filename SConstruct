#!python
import os, subprocess

def sys_exec(args):
    proc = subprocess.Popen(args, stdout=subprocess.PIPE)
    (out, err) = proc.communicate()
    return out.rstrip("\r\n").lstrip()

platform = ARGUMENTS.get("platform", ARGUMENTS.get("p", "osx"))
target = ARGUMENTS.get("target", "debug")
godot_headers_path = ARGUMENTS.get("headers", os.getenv("GODOT_HEADERS", "../godot-cpp/godot_headers/"))
godot_bindings_path = ARGUMENTS.get("cpp_bindings", "../godot-cpp/")
dynamic = ARGUMENTS.get("dynamic", "yes")
ndk_path = ARGUMENTS.get("ndk-path", "/Users/piertho/Library/android-sdks/ndk-bundle/")
ndk_toolchain = ARGUMENTS.get("ndk-toolchain", "/tmp/android-21-toolchain/")
fmodLibInstallPath = ARGUMENTS.get("fmod-lib-install-path", "libs")

clang_path = ARGUMENTS.get("clang-path", "")

if target != "debug":
    target = "release"
# This makes sure to keep the session environment variables on windows,
# that way you can run scons in a vs 2017 prompt and it will find all the required tools
env = Environment()
if platform == "windows":
    env = Environment(ENV = os.environ)
elif platform == "ios":
    SDK_MIN_VERSION = "8.0"
    # we could do better to automatically find the right sdk version
    SDK_VERSION = "12.2"
    IOS_PLATFORM_SDK = sys_exec(["xcode-select", "-p"]) + "/Platforms"
    env["CXX"] = sys_exec(["xcrun", "-sdk", "iphoneos", "-find", "clang++"])
elif platform == "android":
    env["AR"] = ndk_toolchain + "/bin/arm-linux-androideabi-ar"
    env["AS"] = ndk_toolchain + "/bin/arm-linux-androideabi-as"
    env["CC"] = ndk_toolchain + "/bin/arm-linux-androideabi-clang"
    env["CXX"] = ndk_toolchain + "/bin/arm-linux-androideabi-clang++"
    env["LD"] = ndk_toolchain + "/bin/arm-linux-androideabi-ld"
    env["STRIP"] = ndk_toolchain + "/bin/arm-linux-androideabi-strip"

if ARGUMENTS.get("use_llvm", "no") == "yes" and platform != "ios" and platform != "android":
    env["CXX"] = "%sclang++" % clang_path

# put stuff that is the same for all first, saves duplication
cpp_bindings_libname = 'libgodot-cpp.%s' % platform
cpp_bindings_libname += '.%s.64' % target
if platform == "osx":
    cpp_bindings_libname += '.a'
    env.Append(CCFLAGS = ['-g','-O3', '-std=c++14', '-arch', 'x86_64'])
    env.Append(LINKFLAGS = ['-arch', 'x86_64', '-framework', 'Cocoa', '-Wl,-undefined,dynamic_lookup'])
elif platform == "ios":
    cpp_bindings_libname += '.a'
    env.Append(CCFLAGS = ['-g','-O3', '-std=c++11', '-arch', 'arm64', '-arch', 'armv7', '-arch', 'armv7s', '-isysroot', '%s/iPhoneOS.platform/Developer/SDKs/iPhoneOS%s.sdk' % (IOS_PLATFORM_SDK, SDK_VERSION) , '-miphoneos-version-min=%s' % SDK_MIN_VERSION])
    env.Append(LINKFLAGS = ['-arch', 'arm64', '-arch', 'armv7', '-arch', 'armv7s', '-isysroot', '%s/iPhoneOS.platform/Developer/SDKs/iPhoneOS%s.sdk' % (IOS_PLATFORM_SDK, SDK_VERSION) , '-miphoneos-version-min=%s' % SDK_MIN_VERSION])
elif platform == "android":
    cpp_bindings_libname += '.a'
    env.Append(CCFLAGS = ['-fPIE', '-fPIC', '-mfpu=neon', '-march=armv7-a'])
    env.Append(LDFLAGS = ['-pie', '-Wl'])
elif platform == "linux":
    env.Append(CCFLAGS = ['-fPIC', '-g','-O3', '-std=c++14'])
    env.Append(LINKFLAGS=[
        '-Wl,-rpath,\'$$ORIGIN\'/%s' % fmodLibInstallPath
    ])
elif platform == "windows":
    if target == "debug":
        env.Append(CCFLAGS = ['-EHsc', '-D_DEBUG', '/MDd'])
    else:
        env.Append(CCFLAGS = ['-O2', '-EHsc', '-DNDEBUG', '/MD'])
    env.Append(LINKFLAGS = ['/WX'])
    cpp_bindings_libname += '.lib'

def add_sources(sources, directory):
    if os.path.isdir(directory):
        for file in os.listdir(directory):
            if file.endswith('.cpp'):
                sources.append(directory + '/' + file)
    else:
        sources.append(directory)

lfix = ""
if target == "debug":
    lfix = "L"
if platform == "osx":
    libfmod = 'libfmod%s.dylib' % lfix
    libfmodstudio = 'libfmodstudio%s.dylib' % lfix
    env.Append(CPPPATH=[godot_headers_path, godot_bindings_path + 'include/', godot_bindings_path + 'include/core/',
               godot_bindings_path + 'include/gen/', '../libs/fmod/osx/core/inc/', '../libs/fmod/osx/studio/inc/'])
    env.Append(LIBS=[cpp_bindings_libname, libfmod, libfmodstudio])
    env.Append(LIBPATH=[ godot_bindings_path + 'bin/', '../libs/fmod/osx/core/lib/', '../libs/fmod/osx/studio/lib/' ])
elif platform == "linux":
    libfmod = 'libfmod%s.so'% lfix
    libfmodstudio = 'libfmodstudio%s.so'% lfix
    env.Append(CPPPATH=[godot_headers_path, godot_bindings_path + 'include/', godot_bindings_path + 'include/core/',
               godot_bindings_path + 'include/gen/', '../libs/fmod/linux/core/inc/', '../libs/fmod/linux/studio/inc/'])
    env.Append(LIBS=[cpp_bindings_libname, libfmod, libfmodstudio])
    env.Append(LIBPATH=[ godot_bindings_path + 'bin/', '../libs/fmod/linux/core/lib/x86_64', '../libs/fmod/linux/studio/lib/x86_64' ])
elif platform == "windows":
    libfmod = 'fmod%s64'% lfix
    libfmodstudio = 'fmodstudio%s64'% lfix
    env.Append(CPPPATH=[godot_headers_path, godot_bindings_path + 'include/', godot_bindings_path + 'include/core/',
                        godot_bindings_path + 'include/gen/', '../libs/fmod/windows/core/inc/', '../libs/fmod/windows/studio/inc/'])
    env.Append(LIBS=[cpp_bindings_libname, libfmod, libfmodstudio])
    env.Append(LIBPATH=[ godot_bindings_path + 'bin/', '../libs/fmod/windows/core/lib/', '../libs/fmod/windows/studio/lib/' ])
elif platform == "ios":
    libfmod = 'libfmod%s_iphoneos.a' % lfix
    libfmodstudio = 'libfmodstudio%s_iphoneos.a' % lfix
    env.Append(CPPPATH=[godot_headers_path, godot_bindings_path + 'include/', godot_bindings_path + 'include/core/',
                        godot_bindings_path + 'include/gen/', '../libs/fmod/ios/core/inc/', '../libs/fmod/ios/studio/inc/'])
    env.Append(LIBS=[cpp_bindings_libname, libfmod, libfmodstudio])
    env.Append(LIBPATH=[ godot_bindings_path + 'bin/', '../libs/fmod/ios/core/lib/', '../libs/fmod/ios/studio/lib/' ])

sources = []
add_sources(sources, "./")

def change_id(self, arg, env):
    sys_exec(["install_name_tool", "-id", "@rpath/libGodotFmod.%s.dylib" % platform, "bin/libGodotFmod.%s.dylib" % platform])
    sys_exec(["install_name_tool", "-change", "@rpath/libfmodstudio.dylib", "@loader_path/%s/libfmodstudio.dylib" % fmodLibInstallPath, "bin/libGodotFmod.%s.dylib" % platform])
    sys_exec(["install_name_tool", "-change", "@rpath/libfmod.dylib", "@loader_path/%s/libfmod.dylib" % fmodLibInstallPath, "bin/libGodotFmod.%s.dylib" % platform])

# determine to link as shared or static library
if dynamic == "yes" and platform != "ios":
    if platform == "osx":
        library = env.SharedLibrary(target='bin/libGodotFmod.%s.dylib' % platform, source=sources)
        change_id_action = Action('', change_id)
        AddPostAction(library, change_id_action)
    elif platform == "android" or platform == "linux":
        library = env.SharedLibrary(target='bin/libGodotFmod.%s.so' % platform, source=sources)
    elif platform == "windows":
        library = env.SharedLibrary(target='bin/libGodotFmod.%s.dll' % platform, source=sources)
else:
    library = env.StaticLibrary(target="bin/libGodotFmod.%s.a" % platform, source=sources)

# can't figure it out what type of parameter should be at 1st one
# send in '' and it works
if dynamic == "yes":
    Default(library)
