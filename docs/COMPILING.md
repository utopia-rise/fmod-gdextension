# Compiling from sources

## Typical Project structure

```
└── Project root
    ├── libs
    |   └── fmod
    |       └── {platform}
    |           └── specific platform fmod api goes here
    ├── godot-cpp (gdnative bindings, consider using a git submodule)
    └── fmod-gdnative (this repo, consider using it as a submodule of you GDNatives repo)
        ├── CMakeLists.txt (Here for CLion)
        ├── LICENSE
        ├── README.md
        ├── SConstruct (here to build on Windows, Linux, OSX and IOS
        └── src
```

If you look at [Sconstruct](SConstruct) script, you'll see that it refers to libraries that are in path relatives to
parent folder. This is because Utopia-Rise team chose to make this repo integrated to a unique repository, storing all
the GDNative for our project.

So, you are supposed to put fmod libraries under `libs/fmod/{platform}`, according to the platforms you want to support.

Otherwise, you can use [our example project](https://github.com/utopia-rise/GDNative-example-repo), which already contains the appropriate structure.

Feel free to modify SConstruct according to your project structure.

`CMakeLists` is here for CLion ide, as we are used to JetBrains tools. Unfortunately, CLion does not currently support
Sconstruct.

## Building Fmod plugin

### Building GDNative API bindings

To Build GDNative bindings you can follow [this tutorial from godot official documentation](https://docs.godotengine.org/en/3.1/tutorials/plugins/gdnative/gdnative-cpp-example.html#building-the-c-bindings).
If you want to regenerate api you can add the following argument to your building command :
```
godotbinpath="path to your godot binary"
```
When you're done with that part you should have `libgodot-cpp.<platform>.<target>.<bits|android_arch|ios_arch>.<a|lib>` in godot-cpp/bin` folder.

#### iOS

To build bindings for iOS, execute:
```
scons platform=ios generate_bindings=True ios_arch=armv7/arm64 target=release
```

#### Android

To build bindings for android, we provide our [godot-cpp](https://github.com/utopia-rise/godot-cpp) version.
Checkout `utopia-3.2` branch. This is also provided with our [GDNative example project](https://github.com/utopia-rise/GDNative-example-repo).  
First, you should set `ANDROID_NDK_ROOT` environment variable by typing :
```
export ANDROID_NDK_ROOT="pathToYourAndroidNDK"
```  
To build on Android, you should type :
```
scons platform=android generate_bindings=True android_arch=armv7/arm64v8 target=release
```

### Building the GDNative plugin

[Download the FMOD Studio API](https://www.fmod.com/download) (You need to create an account) and place it in the
appropriate platform folder into lib folder (see project structure).

For each platforms, if your project structure is different from the one proposed here, you can overload `cpp_bindings_dir`
and `headers_dir` parameters.

To load fmod dynamic libraries on app or engine loading, fmod GDNative will look in subfolder `libs` by default, as in OSX
part. you can overload this relative path adding this parameter to the command `fmod_lib_dir="path to fmod dll"`.

#### OSX

To build the GDNative for OSX, you should use this command in `fmod-gdnative` folder :

```
scons platform=osx target=release bits=64
```

This will generate `libGodotFmod.osx.release.64.dylib` in `fmod-gdnative/bin` folder.


#### Linux

To build the GDNative for Linux, you should use this command in `fmod-gdnative` folder :
```
scons platform=linux use_llvm=yes target=release bits=64
```
This will generate a `libGodotFmod.linux.release.64.so` in `fmod-gdnative/bin` folder.

#### Windows

To build the GDNative for Windows, you should use this command in `fmod-gdnative` folder :
```
scons platform=windows target=release bits=64
```
This will generate `libGodotFmod.windows.release.64.dll` in `fmod-gdnative/bin` folder.

#### Android

To build the GDNative for Android, we currently use NDKBuild. So you should use this command in `fmod-gdnative` folder :
```
$ANDROID_NDK_ROOT/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk  APP_PLATFORM=android-21
```
This will generate `libGodotFmod.android.<target>.<android_arch>.so` for each supported architectures in `libs` folder.

#### iOS

To build the GDNative for iOS, you should use this command in `fmod-gdnative` folder :
```
scons platform=ios target=release ios-arch=armv7/arm64
```
This will generate `libGodotFmod.ios.release.arm64.a` in `fmod-gdnative/bin` folder.

Those libraries are statics, so you need to add fmod static librairies and godot-cpp static library for ios in your
project, with `libGodotFmod.ios.release.arm64.a`.