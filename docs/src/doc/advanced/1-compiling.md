# Compiling from sources

Clone project with `--recurse-submodules`.

## Typical Project structure

```
└── fmod-gdextension (this repo)
    ├── CMakeLists.txt (Here for CLion)
    ├── SConstruct
    ├── godot-cpp (gdextension bindings, submodule)
    ├── libs
    |   └── {platform}
    |       ├── core
    |       └── studio
    └── src
```

Put the FMOD Studio API of each platform you build under `libs/{platform}`, where `{platform}` is `windows`, `linux`,
`osx`, `ios`, `android` or `web`. The FMOD Studio API needs an FMOD account to download, so it is never committed. `get_fmod.py`
downloads one platform's API with your account and installs it there:

```
python get_fmod.py <FMOD account> <FMOD password> <windows|linux|macos|ios|android|web> 20400
```

Unpacking the Windows installer needs [7-Zip](https://www.7-zip.org/), and the macOS and iOS disk images need macOS.
You can also copy the `api` folder of an FMOD installation to `libs/{platform}` yourself.

`CMakeLists` is here for CLion ide, as we are used to JetBrains tools. Unfortunately, CLion does not currently support
Sconstruct.

## Compiling

In order to compile you should run `scons` command, as for all gdextension projects.

```
scons platform=<macos|linux|windows|ios|android|web> target=<editor|template_debug|template_release> --jobs=<desired number of threads>
```

If `target` is `editor` or `template_debug`, debug symbols will be generated.

### Additional Instructions

On Windows, you need to pass the `arch` argument with values `x86_64` or `x86_32`.

Example:
```
scons platform=windows arch=x86_64 target=editor
```

You can also take the FMOD Studio API from another directory with `fmod_lib_dir` (keep the trailing `/`).

Example:
```
scons platform=windows arch=x86_64 target=editor fmod_lib_dir=../my-fmod-sdks/
```

### Web

Web builds need the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) of the version that
built Godot's web export templates (`EMSCRIPTEN_VERSION` in `.github/workflows/trigger_ci.yml`): a library built with
another version can fail to load. Activate it so that `emcc` is on the `PATH`, then build each target without and with
threads, which give the two libraries `fmod.gdextension` lists for the web:

```
scons platform=web target=template_release
scons platform=web target=template_release threads=yes
```

The web has no `editor` target.
