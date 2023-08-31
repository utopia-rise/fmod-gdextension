# Compiling from sources

Clone project with `--recurse_submodules`.

## Typical Project structure

```
└── Project root
    ├── libs
    |   └── fmod
    |       └── {platform}
    |           └── specific platform fmod api goes here
    └── fmod-gdnative (this repo, consider using it as a submodule of you GDNatives repo)
        ├── CMakeLists.txt (Here for CLion)
        ├── LICENSE
        ├── README.md
        ├── SConstruct (here to build on Windows, Linux, OSX and IOS
        ├── godot-cpp (gdextension bindings, submodule)
        └── src
```

You are supposed to put fmod libraries under `libs/fmod/{platform}`, according to the platforms you want to support.  
`libs` folder should be a brother to plugin repo in hierarchy.

`CMakeLists` is here for CLion ide, as we are used to JetBrains tools. Unfortunately, CLion does not currently support
Sconstruct.

## Compiling

In order to compile you should run `scons` command, as for all gdextension projects.

```
scons platform=<platform> target=<editor|template_debug|template_release> debug_symbols<yes|no> --jobs=<desired number of threads>
```