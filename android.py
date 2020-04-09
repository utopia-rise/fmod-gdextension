#!/usr/bin/env python
# Licensed under the MIT license:
# http://www.opensource.org/licenses/mit-license.php
"""
SCons Tool to Build Android Applications
"""

import os
from SCons.Builder import Builder
from SCons.Defaults import DirScanner, Copy
from SCons.Errors import UserError
from xml.dom import minidom
import SCons.Tool.javac
from SCons.Tool.JavaCommon import parse_java_file

NSURI = 'http://schemas.android.com/apk/res/android'

def get_android_has_code(fname):
    parsed = minidom.parse(open(fname))
    application = parsed.getElementsByTagName('application')[0]
    hasCode = application.getAttributeNS(NSURI, 'hasCode')
    if not hasCode:
        return True # Default value

    if hasCode.lower() in ("yes", "true", "1"):
        return True

    if hasCode.lower() in ("no", "false", "0"):
        return False

    raise UserError("Value of hasCode is unknown")


def get_android_package(fname):
    """ Get the value of the package from <manifest package='foo'> """
    parsed = minidom.parse(open(fname))
    manifest = parsed.getElementsByTagName('manifest')[0]
    return manifest.getAttribute('package')

def get_rfile(package):
    """ Retuns the path to the R.java resource file """
    return os.path.join(package.replace('.', '/'), 'R.java')

def target_from_properties(fname):
    """ Get a target value from a properties file """
    for line in open(fname).readlines():
        line = line.strip()
        if line.startswith('#') or not line:
            continue
        key, val = line.split('=')
        if key == 'target':
            return val.split('-')[1]
    return None

def get_android_name(fname):
    """ Get the android activity name from <activity android:name='foo'> """
    parsed = minidom.parse(open(fname))
    activity = parsed.getElementsByTagName('activity')[0]
    return activity.getAttributeNS(NSURI, 'name')

def get_android_target(fname):
    """
    Get the minimum SDK version and the target SDK version.
    fname is the AndroidManifest.xml file.
    Checks the manifest and also default.properties (if it exists)
    """
    properties = os.path.join(os.path.dirname(fname), 'default.properties')
    parsed = minidom.parse(open(fname))
    uses_sdk = parsed.getElementsByTagName('uses-sdk')[0]
    min_sdk = uses_sdk.getAttributeNS(NSURI, 'minSdkVersion')
    target_sdk = uses_sdk.getAttributeNS(NSURI, 'targetSdkVersion')
    if os.path.exists(properties):
        target_sdk = target_from_properties(properties)
    return (min_sdk, target_sdk or min_sdk)

def add_gnu_tools(env, abi):
    """ Add the NDK GNU compiler tools to the Environment """
    gnu_tools = ['gcc', 'g++', 'gnulink', 'ar', 'gas']
    for tool in gnu_tools:
        env.Tool(tool)
    arm_linux = 'arm-linux-androideabi-4.4.3'
    x86 = 'x86-4.4.3'
    prebuilt = 'prebuilt/linux-x86/bin'
    arm_toolchain = os.path.join('toolchains', arm_linux, prebuilt)
    x86_toolchain = os.path.join('toolchains', x86, prebuilt)
    toolchains = {'armeabi': arm_toolchain,
                  'armeabi-v7a': arm_toolchain,
                  'x86': x86_toolchain}
    prefixes = {'armeabi': 'arm-linux-androideabi-',
                'armeabi-v7a': 'arm-linux-androideabi-',
                'x86': 'i686-android-linux-'}
    toolchain = toolchains[abi]
    prefix = prefixes[abi]

    tool_prefix = os.path.join('$ANDROID_NDK', toolchain, prefix)
    env['CC'] =  tool_prefix+'gcc'
    env['CXX'] = tool_prefix+'g++'
    env['AS'] = tool_prefix+'as'
    env['AR'] = tool_prefix+'ar'
    env['RANLIB'] = tool_prefix+'ranlib'
    env['OBJCOPY'] = tool_prefix+'objcopy'
    env['STRIP'] = tool_prefix+'strip'

def do_proguard(env, safe_name, classes, bin_classes, gen):
    original_jar_name = 'proguard/' + safe_name + 'original.jar'
    obfuscated_jar = 'proguard/' + safe_name + 'obfuscated.jar'
    original_jar = env.Command(original_jar_name,
                               [classes],
                               ['$JAR cf $TARGET -C $DIR1 .',
                                '$JAR uf $TARGET -C $DIR2 .'],
                               DIR1=gen,
                               DIR2=env.Dir(bin_classes).path)

    includes = env['PROGUARD_CONFIG'].split(os.pathsep)
    safe_includes = []
    for include in includes:
        include = str(env.File(env.subst(include)))
        if not os.path.exists(include):
            print("** warning: %s does not exist" % include)
        else:
            safe_includes.append(include)
    args = '-include ' + (' -include '.join(safe_includes))
    args += ' -libraryjars $ANDROID_JAR '
    args += ' -dump $PG_DUMP'
    args += ' -printseeds $PG_SEEDS'
    args += ' -printusage $PG_USAGE'
    args += ' -printmapping $PG_MAPPING'
    pg_sources = [original_jar]
    annotations = '$ANDROID_SDK/tools/support/annotations.jar'
    if os.path.exists(env.subst(annotations)):
        pg_sources.append(annotations)
    dex_input = env.Proguard(obfuscated_jar, pg_sources,
                             PROGUARD_ARGS=args,
                             PS=os.pathsep,
                             PG_DUMP=env.File('proguard/dump.txt'),
                             PG_SEEDS=env.File('proguard/seeds.txt'),
                             PG_USAGE=env.File('proguard/usage.txt'),
                             PG_MAPPING=env.File('proguard/mapping.txt'))
    env.Depends(dex_input, safe_includes)
    env.SideEffect(['proguard/dump.txt', 'proguard/seeds.txt',
                    'proguard/usage.txt', 'proguard/mapping.txt'], dex_input)
    return dex_input

def NdkBuild(env, library=None, inputs=None,
             manifest='#AndroidManifest.xml',
             app_abi='armeabi'):
    """ Use the NDK to build a shared library from the given inputs. """
    # ensure ANDROID_NDK is set
    get_variable(env, 'ANDROID_NDK')
    android_manifest = env.File(manifest)
    if 'ANDROID_TARGET' not in env:
        min_target, target = get_android_target(android_manifest.abspath)
        env['ANDROID_MIN_TARGET'] = min_target
        env['ANDROID_TARGET'] = target

    if type(library) == str:
        library = [library]
    if type(app_abi) == str:
        app_abis = app_abi.split()
    else:
        # Assume that app_abis is a collection already
        app_abis = app_abi

    if len(app_abis) > len(library) and len(library) > 1:
        print("Error: libraries and app_abi do not coincide")
        env.Exit(1)

    libname = library[0]
    if len(library) == 1 and libname.find(os.path.sep) == -1:
        library = [('libs/%s/' % abi) + libname for abi in app_abis]

    results = []
    android_common_cflags = ''' -Wall -Wextra -fpic -ffunction-sections -Os
                                -funwind-tables
                                -fno-short-enums -Wno-psabi
                                -fomit-frame-pointer -fno-strict-aliasing
                                -Wa,--noexecstack'''.split()

    android_abi_cflags = {'armeabi': '''-mthumb-interwork -march=armv5te
                                      -fstack-protector
                                      -mtune=xscale -msoft-float -mthumb
                                      -finline-limit=64''',

                          'armeabi-v7a': ''' -march=armv7-a -mfloat-abi=softfp
                                      -fstack-protector
                                      -mfpu=vfp -mthumb -finline-limit=64 ''',

                          'x86': '''  -finline-limit=300 '''}

    libs_len = len(library)
    for i in range(0, libs_len):
        if libs_len > 1:
            tmp_env = env.Clone()
        else:
            tmp_env = env
        library_name = library[i]
        abi = app_abis[i]
        arch = 'arch-%s' % abi[0:3]
        add_gnu_tools(tmp_env, abi)
        if abi == 'x86':
            if int(tmp_env['ANDROID_MIN_TARGET']) < 9:
                tmp_env['ANDROID_MIN_TARGET'] = '9'
        target_platform = '$ANDROID_NDK/platforms/android-$ANDROID_MIN_TARGET'
        if 'CPPPATH' not in tmp_env:
            tmp_env['CPPPATH'] = []
        tmp_env['CPPPATH'] += [target_platform + '/%s/usr/include' % arch]
        if 'CPPDEFINES' not in tmp_env:
            tmp_env['CPPDEFINES'] = []
        tmp_env['CPPDEFINES'] += ['-DANDROID']
        android_cflags = android_abi_cflags[abi].split()
        android_cflags.extend(android_common_cflags)
        android_cxxflags = '''-fno-rtti -fno-exceptions'''.split()
        tmp_env['CFLAGS'] = env.Flatten(['$CFLAGS', android_cflags])
        tmp_env['CXXFLAGS'] = env.Flatten(['$CXXFLAGS', android_cflags,
                                           android_cxxflags])
        if 'LIBPATH' not in tmp_env:
            tmp_env['LIBPATH'] = []
        tmp_env['LIBPATH'] += [target_platform + '/%s/usr/lib' % arch]
        tmp_env['SHOBJSUFFIX'] = '.'+abi+'-os'
        shflags = '''-Wl,-soname,${TARGET.file}
            -shared
            --sysroot=%s/%s
            -Wl,--no-undefined -Wl,-z,noexecstack''' % (target_platform, arch)
        tmp_env['SHLINKFLAGS'] = shflags.split()

        lib = tmp_env.SharedLibrary('local/'+library_name, inputs,
                                    LIBS=['$LIBS', 'c'])
        tmp_env.Command(library_name, lib, [Copy('$TARGET', "$SOURCE"),
                                   '$STRIP --strip-unneeded $TARGET'])
        results.append(lib)
    return results

def NdkBuildLegacy(env, library=None, inputs=None, app_root='#.',
            build_dir='.'):
    """ Use ndk-build to compile native code. """
    # ensure ANDROID_NDK is set
    get_variable(env, 'ANDROID_NDK')
    if env.GetOption('silent'):
        verbose = 0
    else:
        verbose = 1
    jobs = env.GetOption('num_jobs')
    build_path = env.Dir(build_dir).path
    app_path = env.Dir(app_root).abspath
    cmd = ('$ANDROID_NDK/ndk-build V=%s -j %s SCONS_BUILD_ROOT=%s '
           'APP_PLATFORM=android-$ANDROID_MIN_TARGET -C %s') % (
               verbose, jobs, build_path, app_path)
    lib = env.Command(os.path.join(app_root, library), env.Flatten(inputs), cmd)
    env.Clean(lib, [os.path.join(app_root, x) for x in ('libs', 'obj')])
    return lib

# monkey patch emit_java_classes to do the Right Thing
# otherwise generated classes have no package name and get rebuilt always

_DEFAULT_JAVA_EMITTER = SCons.Tool.javac.emit_java_classes

def emit_java_classes(target, source, env):
    """
    Set correct path for .class files from generated java source files
    """
    classdir = target[0]
    tlist, slist = _DEFAULT_JAVA_EMITTER(target, source, env)
    if 'APP_PACKAGE' in env:
        out = []
        for entry in slist:
            classname = env['APP_PACKAGE'] + entry.name.replace('.java', '')
            java_file = source[0].File(os.path.join(
                    env['APP_PACKAGE'].replace('.', '/'), entry.name))
            if os.path.exists(java_file.abspath):
                version = env.get('JAVAVERSION', '1.4')
                pkg_dir, classes = parse_java_file(
                                java_file.rfile().get_abspath(), version)
                for output in classes:
                    class_file = classdir.File(
                                os.path.join(pkg_dir, str(output) + '.class'))
                    class_file.attributes.java_classdir = classdir
                    class_file.attributes.java_sourcedir = entry.dir
                    class_file.attributes.java_classname = str(output)
                    out.append(class_file)
            else:
                class_file = classdir.File(os.path.join(
                        env['APP_PACKAGE'].replace('.', '/'),
                        entry.name.replace('.java', '.class')))
                class_file.attributes.java_classdir = classdir
                class_file.attributes.java_sourcedir = entry.dir
                class_file.attributes.java_classname = classname
                out.append(class_file)
        return out, slist
    else:
        return tlist, slist


SCons.Tool.javac.emit_java_classes = emit_java_classes


def AndroidApp(env, name,
               manifest='#/AndroidManifest.xml',
               source='#/src',
               resources='#/res',
               native_folder=None):
    """ Create an Android application from the given inputs. """
    android_manifest = env.File(manifest)

    if 'ANDROID_TARGET' not in env:
        min_target, target = get_android_target(android_manifest.abspath)
        if 'ANDROID_MIN_TARGET' not in env:
            env['ANDROID_MIN_TARGET'] = min_target
        env['ANDROID_TARGET'] = target

    safe_name = name.replace('-', '_')
    if 'APP_PACKAGE' not in env:
        package = get_android_package(android_manifest.abspath)
    else:
        package = env['APP_PACKAGE']
    gen_name = safe_name + '_gen'
    rfile = os.path.join(gen_name, get_rfile(package))
    gen = env.Dir(gen_name)

    # generate R.java
    resource_dirs = [env.Dir(r) for r in env.Flatten([resources])]
    abs_resources = [r.abspath for r in resource_dirs]
    res_string = ''
    aapt_args = 'package -f -m -M $MANIFEST -I $ANDROID_JAR -J $GEN'
    for tmp in range(0, len(abs_resources)):
        res_string += ' -S ${RES[%d]}' % tmp
    aapt_args += res_string
    generated_rfile = env.Aapt(rfile, resource_dirs,
             MANIFEST=android_manifest.path,
             GEN=gen, RES=abs_resources,
             AAPT_ARGS=aapt_args.split())
    env.Depends(generated_rfile, android_manifest)

    release_build = env['ANDROID_KEY_STORE'] and env['ANDROID_KEY_NAME']
    dex = []
    if get_android_has_code(android_manifest.abspath):
        # compile java to classes
        bin_classes = safe_name+'_bin/classes'
        default_cp = env.Dir(bin_classes).path
        has_cp = 'JAVACLASSPATH' in env and env['JAVACLASSPATH']
        if has_cp:
            default_cp = env['JAVACLASSPATH'] + os.pathsep + default_cp
        default_cp += os.pathsep + '$ANDROID_SDK/tools/support/annotations.jar'
        if type(source) == str:
            source = [source]
        classes = env.Java(target=bin_classes, source=source,
                           JAVABOOTCLASSPATH='$ANDROID_JAR',
                           JAVASOURCEPATH=gen.path,
                           JAVACFLAGS='-target 1.5 -source 1.5 -g -Xlint -encoding ascii'.split(),
                           JAVACLASSPATH=default_cp)
        env.Depends(classes, rfile)

        # dex file from classes
        dex_input = classes

        dx_dir = env.Dir(bin_classes).path
        has_pg = 'PROGUARD_CONFIG' in env and env['PROGUARD_CONFIG']
        if release_build and has_pg:
            dex_input = do_proguard(env, safe_name, classes, bin_classes, gen)
            dx_dir = dex_input

        if has_cp:
            env['DX_CLASSPATH'] = env['JAVACLASSPATH'].split(os.pathsep)
            dex_input.extend(env['DX_CLASSPATH'])

        dex = env.Dex(name+'classes.dex', dex_input, DX_DIR=dx_dir)
        env.Depends(dex, dex_input)

    if not dex and not native_folder:
        # assume this is a native-only project..
        native_folder = 'libs'

    # resources
    aapt_args = 'package -f -m -M $MANIFEST -I $ANDROID_JAR -F $TARGET '
    aapt_args += res_string
    tmp_package = env.Aapt(name + '.ap_', resource_dirs,
                  MANIFEST=android_manifest.path,
                  RES=abs_resources,
                  AAPT_ARGS=aapt_args.split())
    env.Depends(tmp_package, android_manifest)

    # package java -classpath jarutils.jar:androidprefs.jar:apkbuilder.jar \
    #           com.android.apkbuilder.ApkBuilder
    # >> name-debug-unaligned.apk
    outname = name + '-debug-unaligned.apk'
    finalname = name + '-debug.apk'
    if env['ANDROID_KEY_STORE']:
        unsigned_flag = '-u'
        outname = name + '-unsigned.apk'
        finalname = name + '.apk'
    else:
        unsigned_flag = ''
    apk_args = "$UNSIGNED -f $SOURCE -z $AP"
    native_path = None
    if native_folder:
        apk_args += ' -nf $NATIVE_FOLDER'
        native_path = env.Dir(native_folder).path
    unaligned = env.ApkBuilder(outname, [dex, tmp_package],
                   NATIVE_FOLDER=native_path,
                   UNSIGNED=unsigned_flag,
                   AP=tmp_package,
                   APK_ARGS=apk_args.split())
    if native_folder:
        sofiles = env.Glob(native_folder + '/armeabi/*.so')
        sofiles.extend(env.Glob(native_folder + '/armeabi-v7a/*.so'))
        sofiles.extend(env.Glob(native_folder + '/x86/*.so'))
        env.Depends(unaligned, env.Flatten([dex, tmp_package, sofiles]))
    else:
        env.Depends(unaligned, [dex, tmp_package])
    env.Depends(unaligned, env.subst('$APK_BUILDER_JAR').split())
    if release_build:
        unaligned = env.JarSigner(name + '-unaligned.apk', unaligned)

    # zipalign -f 4 unaligned aligned
    app = env.ZipAlign(finalname, unaligned)
    # installation marker
    adb = env['ANDROID_ADB']
    adb_install = env.Command(name + '-installed', app,
        [adb + ' install -r $SOURCE && date > $TARGET'])
    # do not run by default
    env.Ignore(adb_install[0].dir, adb_install)
    env.Alias('install', adb_install)

    if 'APP_ACTIVITY' not in env:
        activity = get_android_name(android_manifest.abspath)
    else:
        activity = env['APP_ACTIVITY']
    run = env.Command(name + '-run', app,
      [adb + ' shell am start -a android.intent.action.MAIN -n %s/%s%s'%(
          package, package, activity)])
    env.Depends(run, adb_install)
    env.Ignore(run[0].dir, run)
    env.Alias('run', run)

    return app

def get_variable(env, variable, do_exit=True):
    """
    Extract a variable from the environment if it exists.
    Optionally exits the run
    """
    if variable in os.environ:
        return os.environ[variable]
    elif variable in env:
        return env[variable]
    elif do_exit:
        print('Please set %s. export %s=path' % (variable, variable))
        print("or run `scons %s=path'" % variable)
        env.Exit(1)
    return None

def generate(env, **kw):
    """ SCons tool entry point """
    # ensure ANDROID_SDK is set
    get_variable(env, 'ANDROID_SDK')

    if 'ANDROID_KEY_STORE' not in env:
        env['ANDROID_KEY_STORE'] = ''

    if 'ANDROID_KEY_NAME' not in env:
        env['ANDROID_KEY_NAME'] = ''

    env.Tool('javac')
    env.Tool('jar')
    env['AAPT'] = '$ANDROID_SDK/platform-tools/aapt'
    env['DX'] = '$ANDROID_SDK/platform-tools/dx'
    env['ZIPALIGN'] = '$ANDROID_SDK/tools/zipalign'
    env['JARSIGNER'] = 'jarsigner'
    env['ANDROID_JAR'] = os.path.join('$ANDROID_SDK',
                              'platforms/android-$ANDROID_TARGET/android.jar')
    env['ANDROID_ADB'] = os.path.join('$ANDROID_SDK','platform-tools/adb')

    bld = Builder(action='$AAPT $AAPT_ARGS', suffix='.java',
                  source_scanner=DirScanner)
    env.Append(BUILDERS = { 'Aapt': bld })

    bld = Builder(action='$DX --dex --output=$TARGET $DX_DIR $DX_CLASSPATH', suffix='.dex')
    env.Append(BUILDERS = { 'Dex': bld })
    env['JAVA'] = 'java'

    cpfiles = os.pathsep.join(os.path.join('$ANDROID_SDK', 'tools/lib', jar)
                              for jar in 'androidprefs.jar sdklib.jar'.split())

    env['APK_BUILDER_CP'] = cpfiles
    base = os.path.join(os.path.dirname(__file__))
    j = env.Java(target='toolclasses',
             source=[base + '/sdklib/ApkBuilderMain.java'],
             JAVACLASSPATH='$APK_BUILDER_CP',
             JAVASOURCEPATH=env.Dir('#site_scons/site_tools').path)
    env['APK_BUILDER_JAR'] = j

    apk_builder = ('$JAVA -classpath $TOOL_CLASSES_DIR:$APK_BUILDER_CP '
                   'android.sdklib.ApkBuilderMain $TARGET $APK_ARGS')
    bld = Builder(action=apk_builder,
                  source_scanner=DirScanner,
                  TOOL_CLASSES_DIR=env.Dir('toolclasses'),
                  suffix='.apk')
    env.Append(BUILDERS = { 'ApkBuilder': bld })

    bld = Builder(action='$ZIPALIGN -f 4 $SOURCE $TARGET')
    env.Append(BUILDERS = { 'ZipAlign': bld })

    jarsigner_cmd = ('$JARSIGNER $JARSIGNER_FLAGS -keystore $ANDROID_KEY_STORE'
                     ' -signedjar $TARGET $SOURCE $ANDROID_KEY_NAME')
    env.Append(BUILDERS = { 'JarSigner': Builder(action=jarsigner_cmd) })

    proguard_cmd = ('$JAVA -jar $ANDROID_SDK/tools/proguard/lib/proguard.jar'
                    ' -injars ${PS.join([s.path for s in SOURCES])}'
                    ' -outjars $TARGET '
                    ' $PROGUARD_ARGS')
    env.Append(BUILDERS = {'Proguard': Builder(action=proguard_cmd)})

    env.AddMethod(AndroidApp)
    env.AddMethod(NdkBuild)
    env.AddMethod(NdkBuildLegacy)

def exists(env):
    """ NOOP method required by SCons """
    return 1
