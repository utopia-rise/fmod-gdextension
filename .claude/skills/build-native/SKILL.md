---
name: build-native
description: Build the native GDExtension with SCons for a platform and target, into the demo project or another one. Use whenever C++ under src/, SConstruct or build_profile.json changed and the extension must be rebuilt.
argument-hint: "[platform] [target] [target_path]; default: host platform, editor"
---

# Build the native GDExtension

Run SCons from the repository root. Platforms are `windows`, `linux`, `macos`, `android`, `ios`, `web` (godot-cpp naming). Targets are `editor`, `template_debug`, `template_release`.

```bash
scons platform=<platform> target=<target> -j<cores>
```

- **FMOD SDK:** SCons reads it from `libs/<sdk platform>/{core,studio}/{inc,lib}` at the repository root (ignored by git) (SDK platform folders: `windows`, `linux`, `osx`, `ios`, `android`, `web`), or from `fmod_lib_dir=<dir>/` (keep the trailing `/`). Each folder is the `api/` directory of the FMOD installer for that platform, renamed. The SDK version must match `FMOD_VERSION` in `.github/workflows/trigger_ci.yml` (`20400` is 2.04.00; `FMOD_VERSION` in `core/inc/fmod_common.h` shows what is installed). `python get_fmod.py <FMOD account> <FMOD password> <platform> <FMOD_VERSION>` downloads and installs one platform's SDK there (7-Zip needed for Windows, macOS for the macOS and iOS ones); it needs the user's FMOD account, so ask the user to run it, and never handle their password.
- **Arch:** Windows needs `arch=x86_64` (or `x86_32`), Android `arch=arm64` or `arch=x86_64` with `ANDROID_HOME` set and the NDK godot-cpp defaults to (28.1.13356709) installed; Android, iOS and the web have no `editor` target. The web needs the Emscripten SDK of version `EMSCRIPTEN_VERSION` (`.github/workflows/trigger_ci.yml`) activated, so that `emcc` is on the `PATH`, and builds `threads=no` by default: build `threads=yes` too, the two libraries `fmod.gdextension` lists (`libGodotFmod.web.<target>.wasm32.nothreads.wasm` and `libGodotFmod.web.<target>.wasm32.wasm`), with FMOD linked in and no runtime library copied. macOS builds a framework; iOS needs Xcode and builds a static library (`libGodotFmod.ios.<target>.a`, godot-cpp merged in with `libtool`), since a dynamic one gets embedded and signed under an identifier App Store Connect rejects.
- **Output:** libraries land in `demo/addons/fmod/libs/<platform>/` (Android: `libs/android/<arch>/`), where the demo picks them up, and SConstruct copies the FMOD runtime libraries next to them: `fmodL`/`fmodstudioL` for `editor` and `template_debug`, `fmod`/`fmodstudio` for `template_release`. To build into another project, add `target_path=/absolute/path/to/project/addons/fmod/libs/` (keep the trailing `/`).
- **Build profile:** `build_profile.json` is an allowlist of the engine classes godot-cpp generates and compiles. A class missing from it shows up as a missing `classes/*.hpp` header (C1083 / "file not found"), or as a godot-cpp method that does not exist because one of its argument types is excluded. Add the class there; do not drop the profile. godot-cpp only regenerates its bindings when `godot-cpp/gen/` is absent or with `generate_bindings=yes`: after editing the profile, delete `godot-cpp/gen/` (or pass the flag), or stale headers of excluded classes hide the error until link time.
- **godot-cpp submodule:** after switching branches, `git submodule update godot-cpp` and delete `godot-cpp/gen/` before building; bindings from another godot-cpp commit produce a library the editor refuses or crashes on. The Godot API it builds against is `api_version` in SConstruct.
- **Android AARs:** after the four Android builds, `cd android-plugin && ./gradlew :library:assemble` (JDK 17, `fmod.jar` in `library/libraries/`) packs them into `fmod-debug.aar` and `fmod-release.aar`, which Android exports use instead of the bare libraries.
- **A locked library** (LNK1104 / "Access is denied" on Windows): a Godot process still holds it. Find it; kill only a process you started, and ask the user to close their own editor.

After changing includes or `#ifdef` code, build all three targets: see the `check-cpp` skill.
