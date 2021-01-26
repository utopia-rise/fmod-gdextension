#!/usr/bin/env python

import os
import sys
import subprocess

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

# Workaround for MinGW. See:
# http://www.scons.org/wiki/LongCmdLinesOnWin32
if os.name == "nt":
    import subprocess

    def mySubProcess(cmdline,env):
        #print "SPAWNED : " + cmdline
        startupinfo = subprocess.STARTUPINFO()
        startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        proc = subprocess.Popen(cmdline, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
            stderr=subprocess.PIPE, startupinfo=startupinfo, shell = False, env = env)
        data, err = proc.communicate()
        rv = proc.wait()
        if rv:
            print("=====")
            print(err.decode("utf-8"))
            print("=====")
        return rv

    def mySpawn(sh, escape, cmd, args, env):

        newargs = ' '.join(args[1:])
        cmdline = cmd + " " + newargs

        rv=0
        if len(cmdline) > 32000 and cmd.endswith("ar") :
            cmdline = cmd + " " + args[1] + " " + args[2] + " "
            for i in range(3,len(args)) :
                rv = mySubProcess( cmdline + args[i], env )
                if rv :
                    break
        else:
            rv = mySubProcess( cmdline, env )

        return rv

def sys_exec(args):
    proc = subprocess.Popen(args, stdout=subprocess.PIPE, text=True)
    (out, err) = proc.communicate()
    return out.rstrip("\r\n").lstrip()

#################
#OPTIONS#########
#################

# Try to detect the host platform automatically.
# This is used if no `platform` argument is passed
if sys.platform.startswith('linux'):
    host_platform = 'linux'
elif sys.platform == 'darwin':
    host_platform = 'osx'
elif sys.platform == 'win32' or sys.platform == 'msys':
    host_platform = 'windows'
else:
    raise ValueError(
        'Could not detect platform automatically, please specify with '
        'platform=<platform>'
    )

opts = Variables([], ARGUMENTS)
opts.Add(EnumVariable(
    'platform',
    'Target platform',
    host_platform,
    allowed_values=('linux', 'osx', 'windows', 'android', 'ios'),
    ignorecase=2
))
opts.Add(EnumVariable(
    'bits',
    'Target platform bits',
    'default',
    ('default', '32', '64')
))
opts.Add(BoolVariable(
    'use_llvm',
    'Use the LLVM compiler - only effective when targeting Linux',
    False
))
opts.Add(BoolVariable(
    'use_mingw',
    'Use the MinGW compiler instead of MSVC - only effective on Windows',
    False
))
# Must be the same setting as used for cpp_bindings
opts.Add(EnumVariable(
    'target',
    'Compilation target',
    'debug',
    allowed_values=('debug', 'release'),
    ignorecase=2
))
opts.Add(PathVariable(
    'cpp_bindings_dir',
    'Path to the cpp binding library',
    "../godot-cpp/",
    PathVariable.PathIsDir
))
opts.Add(PathVariable(
    'headers_dir',
    'Path to the directory containing Godot headers',
    'default',
    PathVariable.PathAccept
))
opts.Add(EnumVariable(
    'android_arch',
    'Target Android architecture',
    'armv7',
    ['armv7','arm64v8','x86','x86_64']
))
opts.Add(EnumVariable(
    'ios_arch',
    'Target iOS architecture',
    'arm64',
    ['armv7', 'arm64', 'x86_64']
))
opts.Add(
    'IPHONEPATH',
    'Path to iPhone toolchain',
    '/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain',
)
opts.Add(
    'android_api_level',
    'Target Android API level',
    '18' if ARGUMENTS.get("android_arch", 'armv7') in ['armv7', 'x86'] else '21'
)
opts.Add(
    'ANDROID_NDK_ROOT',
    'Path to your Android NDK installation. By default, uses ANDROID_NDK_ROOT from your defined environment variables.',
    os.environ.get("ANDROID_NDK_ROOT", None)
)
opts.Add(
    'fmod_lib_dir',
    'Path to the FMOD library',
    "../libs/fmod/"
)

env = Environment(ENV = os.environ)
opts.Update(env)
Help(opts.GenerateHelpText(env))

is64 = sys.maxsize > 2**32
if (
    env['TARGET_ARCH'] == 'amd64' or
    env['TARGET_ARCH'] == 'emt64' or
    env['TARGET_ARCH'] == 'x86_64' or
    env['TARGET_ARCH'] == 'arm64-v8a'
):
    is64 = True

# This makes sure to keep the session environment variables on Windows.
# This way, you can run SCons in a Visual Studio 2017 prompt and it will find
# all the required tools
if host_platform == 'windows':
    if env['platform'] != 'android':
        if env['bits'] == '64':
            env = Environment(TARGET_ARCH='amd64')
        elif env['bits'] == '32':
            env = Environment(TARGET_ARCH='x86')
    opts.Update(env)

if env["headers_dir"] == 'default':
    env["headers_dir"] = os.environ.get("GODOT_HEADERS", env["cpp_bindings_dir"] + "godot_headers/")

if env['bits'] == 'default':
    env['bits'] = '64' if is64 else '32'

arch_suffix = env['bits']
if env['platform'] == 'android':
    arch_suffix = env['android_arch']
if env['platform'] == 'ios':
    arch_suffix = env['ios_arch']

###################
####FLAGS##########
###################

if env['platform'] == 'linux':
    if env['use_llvm']:
        env['CXX'] = 'clang++'

    env.Append(CCFLAGS=['-fPIC', '-g', '-std=c++14', '-Wwrite-strings'])
    env.Append(LINKFLAGS=["-Wl,-R,'$$ORIGIN'"])

    if env['target'] == 'debug':
        env.Append(CCFLAGS=['-Og'])
    elif env['target'] == 'release':
        env.Append(CCFLAGS=['-O3'])

    if env['bits'] == '64':
        env.Append(CCFLAGS=['-m64'])
        env.Append(LINKFLAGS=['-m64'])
    elif env['bits'] == '32':
        env.Append(CCFLAGS=['-m32'])
        env.Append(LINKFLAGS=['-m32'])

elif env['platform'] == 'osx':
    # Use Clang on macOS by default
    env['CXX'] = 'clang++'

    if env['bits'] == '32':
        raise ValueError(
            'Only 64-bit builds are supported for the macOS target.'
        )

    env.Append(CCFLAGS=['-g', '-std=c++14', '-arch', 'x86_64'])
    env.Append(LINKFLAGS=[
        '-arch',
        'x86_64',
        '-framework',
        'Cocoa',
        '-Wl,-undefined,dynamic_lookup',
    ])

    if env['target'] == 'debug':
        env.Append(CCFLAGS=['-Og'])
    elif env['target'] == 'release':
        env.Append(CCFLAGS=['-O3'])

elif env['platform'] == 'ios':
    if env['ios_arch'] == 'x86_64':
        sdk_name = 'iphonesimulator'
        env.Append(CCFLAGS=['-mios-simulator-version-min=10.0'])
    else:
        sdk_name = 'iphoneos'
        env.Append(CCFLAGS=['-miphoneos-version-min=10.0'])

    try:
        sdk_path = decode_utf8(subprocess.check_output(['xcrun', '--sdk', sdk_name, '--show-sdk-path']).strip())
    except (subprocess.CalledProcessError, OSError):
        raise ValueError("Failed to find SDK path while running xcrun --sdk {} --show-sdk-path.".format(sdk_name))

    compiler_path = env['IPHONEPATH'] + '/usr/bin/'
    env['ENV']['PATH'] = env['IPHONEPATH'] + "/Developer/usr/bin/:" + env['ENV']['PATH']

    env['CC'] = compiler_path + 'clang'
    env['CXX'] = compiler_path + 'clang++'
    env['AR'] = compiler_path + 'ar'
    env['RANLIB'] = compiler_path + 'ranlib'

    env.Append(CCFLAGS=['-g', '-std=c++14', '-arch', env['ios_arch'], '-isysroot', sdk_path])
    env.Append(LINKFLAGS=[
        '-arch',
        env['ios_arch'],
        '-framework',
        'Cocoa',
        '-Wl,-undefined,dynamic_lookup',
        '-isysroot', sdk_path,
        '-F' + sdk_path
    ])

    if env['target'] == 'debug':
        env.Append(CCFLAGS=['-Og'])
    elif env['target'] == 'release':
        env.Append(CCFLAGS=['-O3'])

elif env['platform'] == 'windows':
    if host_platform == 'windows' and not env['use_mingw']:
        # MSVC
        env.Append(LINKFLAGS=['/WX'])
        if env['target'] == 'debug':
            env.Append(CCFLAGS=['/Z7', '/Od', '/EHsc', '/D_DEBUG', '/MDd'])
        elif env['target'] == 'release':
            env.Append(CCFLAGS=['/O2', '/EHsc', '/DNDEBUG', '/MD'])

    elif host_platform == 'linux' or host_platform == 'osx':
        # Cross-compilation using MinGW
        if env['bits'] == '64':
            env['CXX'] = 'x86_64-w64-mingw32-g++'
            env['AR'] = "x86_64-w64-mingw32-ar"
            env['RANLIB'] = "x86_64-w64-mingw32-ranlib"
            env['LINK'] = "x86_64-w64-mingw32-g++"
        elif env['bits'] == '32':
            env['CXX'] = 'i686-w64-mingw32-g++'
            env['AR'] = "i686-w64-mingw32-ar"
            env['RANLIB'] = "i686-w64-mingw32-ranlib"
            env['LINK'] = "i686-w64-mingw32-g++"
    elif host_platform == 'windows' and env['use_mingw']:
        env = env.Clone(tools=['mingw'])
        env["SPAWN"] = mySpawn

    # Native or cross-compilation using MinGW
    if host_platform == 'linux' or host_platform == 'osx' or env['use_mingw']:
        env.Append(CCFLAGS=['-g', '-O3', '-std=c++14', '-Wwrite-strings'])
        env.Append(LINKFLAGS=[
            '--static',
            '-Wl,--no-undefined',
            '-static-libgcc',
            '-static-libstdc++',
        ])
elif env['platform'] == 'android':
    if host_platform == 'windows':
        env = env.Clone(tools=['mingw'])
        env["SPAWN"] = mySpawn

    # Verify NDK root
    if not 'ANDROID_NDK_ROOT' in env:
        raise ValueError("To build for Android, ANDROID_NDK_ROOT must be defined. Please set ANDROID_NDK_ROOT to the root folder of your Android NDK installation.")

    # Validate API level
    api_level = int(env['android_api_level'])
    if env['android_arch'] in ['x86_64', 'arm64v8'] and api_level < 21:
        print("WARN: 64-bit Android architectures require an API level of at least 21; setting android_api_level=21")
        env['android_api_level'] = '21'
        api_level = 21

    # Setup toolchain
    toolchain = env['ANDROID_NDK_ROOT'] + "/toolchains/llvm/prebuilt/"
    if host_platform == "windows":
        toolchain += "windows"
        import platform as pltfm
        if pltfm.machine().endswith("64"):
            toolchain += "-x86_64"
    elif host_platform == "linux":
        toolchain += "linux-x86_64"
    elif host_platform == "osx":
        toolchain += "darwin-x86_64"
    env.PrependENVPath('PATH', toolchain + "/bin") # This does nothing half of the time, but we'll put it here anyways

    # Get architecture info
    arch_info_table = {
        "armv7" : {
            "march":"armv7-a", "target":"armv7a-linux-androideabi", "tool_path":"arm-linux-androideabi", "compiler_path":"armv7a-linux-androideabi",
            "ccflags" : ['-mfpu=neon'], "target_platform": "arch-arm"
            },
        "arm64v8" : {
            "march":"armv8-a", "target":"aarch64-linux-android", "tool_path":"aarch64-linux-android", "compiler_path":"aarch64-linux-android",
            "ccflags" : [], "target_platform": "arch-arm64"
            },
        "x86" : {
            "march":"i686", "target":"i686-linux-android", "tool_path":"i686-linux-android", "compiler_path":"i686-linux-android",
            "ccflags" : ['-mstackrealign'], "target_platform": "arch-x86"
            },
        "x86_64" : {"march":"x86-64", "target":"x86_64-linux-android", "tool_path":"x86_64-linux-android", "compiler_path":"x86_64-linux-android",
            "ccflags" : [], "target_platform": "arch-x86_64"
        }
    }
    arch_info = arch_info_table[env['android_arch']]

    # Setup tools
    env['CC'] = toolchain + "/bin/clang"
    env['CXX'] = toolchain + "/bin/clang++"
    env['AR'] = toolchain + "/bin/" + arch_info['tool_path'] + "-ar"
    env["AS"] = toolchain + "/bin/" + arch_info['tool_path'] + "-as"
    env["LD"] = toolchain + "/bin/" + arch_info['tool_path'] + "-ld"
    env["STRIP"] = toolchain + "/bin/" + arch_info['tool_path'] + "-strip"
    env["RANLIB"] = toolchain + "/bin/" + arch_info['tool_path'] + "-ranlib"
    env['OBJCOPY'] = toolchain + "/bin/" + arch_info['tool_path'] + "-objcopy"
    env['LINK'] = toolchain + "/bin/clang++"
    target_platform = env['ANDROID_NDK_ROOT'] + ("/platforms/android-%s" % api_level) + ('/%s/usr/lib' % arch_info['target_platform'])
    env['SHLINKFLAGS'] = ["-Wl", "-shared", "--sysroot=%s" % target_platform, "-Wl", "-z", "noexecstack"]

    env.Append(CCFLAGS=['--target=' + arch_info['target'] + env['android_api_level'], '-march=' + arch_info['march'], '-fPIC'])
    env.Append(CCFLAGS= arch_info['ccflags'])
    env.Append(CPPDEFINES = "-DANDROID")


#####################
#ADD SOURCES#########
#####################
cpp_bindings_libname = 'libgodot-cpp.{}.{}.{}'.format(
                        env['platform'],
                        env['target'],
                        arch_suffix)

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
    fmod_info_table = {
        "64" : "x86_64",
        "32" : "x86",
    }
    arch_dir = fmod_info_table[arch_suffix]
    env.Append(CPPPATH=[env['headers_dir'], env['cpp_bindings_dir'] + 'include/', env['cpp_bindings_dir'] + 'include/core/',
               env['cpp_bindings_dir'] + 'include/gen/', env['fmod_lib_dir'] + 'linux/core/inc/', env['fmod_lib_dir'] + 'linux/studio/inc/'])
    env.Append(LIBS=[cpp_bindings_libname, libfmod, libfmodstudio])
    env.Append(LIBPATH=[ env['cpp_bindings_dir'] + 'bin/', env['fmod_lib_dir'] + 'linux/core/lib/' + arch_dir, env['fmod_lib_dir'] + 'linux/studio/lib/' + arch_dir])
elif platform == "windows":
    cpp_bindings_libname += '.lib'
    libfmod = 'fmod%s_vc'% lfix
    libfmodstudio = 'fmodstudio%s_vc'% lfix
    fmod_info_table = {
        "64" : "x64",
        "32" : "x86",
    }
    arch_dir = fmod_info_table[arch_suffix]
    env.Append(CPPPATH=[env['headers_dir'], env['cpp_bindings_dir'] + 'include/', env['cpp_bindings_dir'] + 'include/core/',
                        env['cpp_bindings_dir'] + 'include/gen/', env['fmod_lib_dir'] + 'windows/core/inc/', env['fmod_lib_dir'] + 'windows/studio/inc/'])
    env.Append(LIBS=[cpp_bindings_libname, libfmod, libfmodstudio])
    env.Append(LIBPATH=[ env['cpp_bindings_dir'] + 'bin/', env['fmod_lib_dir'] + 'windows/core/lib/' + arch_dir, env['fmod_lib_dir'] + 'windows/studio/lib/' + arch_dir])
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
    fmod_info_table = {
        "armv7": "armeabi-v7a",
        "arm64v8": "arm64-v8a",
        "x86": "x86",
        "x86_64": "x86_64"
    }
    arch_dir = fmod_info_table[arch_suffix]
    env.Append(CPPPATH=[env['headers_dir'], env['cpp_bindings_dir'] + 'include/', env['cpp_bindings_dir'] + 'include/core/',
                        env['cpp_bindings_dir'] + 'include/gen/', env['fmod_lib_dir'] + 'android/core/inc/', env['fmod_lib_dir'] + 'android/studio/inc/'])
    env.Append(LIBS=[cpp_bindings_libname, libfmod, libfmodstudio])
    env.Append(LIBPATH=[ env['cpp_bindings_dir'] + 'bin/', env['fmod_lib_dir'] + 'android/core/lib/' + arch_dir, env['fmod_lib_dir'] + 'android/studio/lib/' + arch_dir])

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
    arch_suffix)

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
