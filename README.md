![logo](.README/fmod-gdextension-logo.png)

Godot C++ GDExtension for Godot 4 that provides an integration for the FMOD Studio API.

FMOD is an audio engine and middleware solution for interactive audio in games. It has been the audio engine behind many
titles such as Transistor, Into the Breach and Celeste. [More on FMOD's website](https://www.fmod.com/).

This GDExtension exposes most of the Studio API functions to Godot's GDScript and also provides helpers for performing
common functions like attaching Studio events to Godot nodes and playing 3D/positional audio. _It is still very much a
work in progress and some API functions are not yet exposed._ Feel free to tweak/extend it based on your project's needs.

**Note:** FMOD also provides a C# wrapper for their API which is used in the Unity integration and it is possible to use the
same wrapper to build an integration for Godot in C#. However do note that this would only work on a Mono build of Godot
as C# support is required and performance might not be on the same level as a C++ integration. 

**Note:** This project is a fork of [godot-fmod-integration](https://github.com/alexfonseka/godot-fmod-integration)
which uses godot module to integrate fmod in the engine. We thank [alexfonseka](https://github.com/alexfonseka) for the work he did.
This fork was designed to be able to use Fmod without building Godot Engine !

**Note:** The video tutorial below will guide you through the integration on Windows, including the basics of FMOD Studio and basic Godot usage. The tutorial uses driver 3.1.1, is for Godot 3.3.4 and 3.4.4, and uses FMOD 2.02.07.

[![Integration Tutorial Video Thumbnail](https://img.youtube.com/vi/Zhh7B2Ggr_g/0.jpg)](https://www.youtube.com/watch?v=Zhh7B2Ggr_g)

![wowmeme]

### Getting started

in order to get started, take a look at [documentation](./docs/src/doc/user-guide)

### Continuous delivery

This project uses github actions to continuously deploy released drivers. If you do not want to use those releases, you
can compile from sources by looking to [compile from sources section](./docs/src/doc/advanced/1-compiling.md).  
This project uses [SEMVER](https://semver.org/).

## Contributing

In order to be able to PR this repo from a fork, you need to add `FMODUSER` and `FMODPASS` secrets to your fork repo.  
This enables CI to download FMOD api.

## Thanks

This project is a forked from [godot-fmod-integration](https://github.com/alexfonseka/godot-fmod-integration)
from [alexfonseka](https://github.com/alexfonseka). We'd like to thank him for the work he did, we simply adapted his
work to GDNative.  
Feel free to propose any modification using github's *pull request*. We hope you'll enjoy this driver.


[wowmeme]: .README/wowmeme.png
[fmodsingleton]: .README/fmodsingleton.png
[usecustombuild]: .README/usecustombuild.png
