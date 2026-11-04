# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What This Project Is

**FMOD GDExtension** integrates the FMOD Studio and FMOD Core APIs into Godot 4 as a GDExtension built against [godot-cpp](https://github.com/godotengine/godot-cpp). It is a C++ extension plus a GDScript editor addon: the native side exposes an `FmodServer` engine singleton, wrappers of FMOD Studio objects (banks, events, buses, VCAs), scene nodes (emitters, listeners, bank loader) and settings resources; the addon (`demo/addons/fmod/`) adds the bank explorer, Inspector editors, the `FmodManager` autoload and the Android export plugin.

The minimum Godot version is `compatibility_minimum` in `demo/addons/fmod/fmod.gdextension` (4.6, with `compatibility_maximum = "4"`: GDExtensions are forward compatible, so any later 4.x loads the addon), mirrored by `api_version` in `SConstruct`, `config/features` in `demo/project.godot` and the `org.godotengine:godot` dependency of `android-plugin/library/build.gradle.kts`; CI runs the latest patch of that minor version, `GODOT_VERSION` in `.github/workflows/trigger_ci.yml` and `trigger_on_tag.yml`. The FMOD version is `FMOD_VERSION` in the same two workflows, mirrored by the versioned Linux library names in `fmod.gdextension`. `EMSCRIPTEN_VERSION` there is the Emscripten that built the official web export templates of `GODOT_VERSION` (`godotengine/build-containers`, `Dockerfile.web` of that branch): web libraries are built with exactly that version. Change each set together.

## Specs

[`specs/`](specs/) is the guidance of the project: what FMOD GDExtension offers its users, and the global rules (the constitution) every implementation follows. Those product rules live there, not in this file. Read `specs/CLAUDE.md` before implementing or changing a feature; it explains how the directory works and how to keep it in sync with the code. `python specs/check_specs.py` validates it, and CI runs it on every pull request.

Run the `spec-review` skill, unasked, before committing or opening a PR for any change to `src/`, `demo/addons/fmod/`, `android-plugin/`, `doc_classes/` or `specs/`, including spec edits you made yourself. Report every conflict to the maintainer with the ways to resolve it (change the code, change the spec, or record the gap as `partial` with a `breaks` note) and your recommendation, and wait for their decision: never settle a conflict by editing a spec on your own.

## Skills

Recurring procedures are Claude Code skills under `.claude/skills/`: `build-native`, `check-cpp`, `run-tests` and `spec-review`. Use them instead of re-deriving the steps from this file.

## godot-cpp

The `godot-cpp` submodule tracks upstream [`godotengine/godot-cpp`](https://github.com/godotengine/godot-cpp) at its latest release (`10.0.0-stable`). Since 10.0, godot-cpp is versioned on its own and builds against any supported Godot API (4.3 to 4.7) through `api_version`, which SConstruct sets to the minimum Godot version. After switching branches, run `git submodule update godot-cpp` and delete `godot-cpp/gen/` before building.

godot-cpp 10 no longer pulls headers in transitively the way the `godot-4.x-stable` tags did (`classes/ref.hpp` among them), and its core `Mutex` (`templates/mutex.hpp`, with `MutexLock`) replaces the engine `Mutex` class for locking in C++.

`build_profile.json` is an allowlist of the engine classes godot-cpp generates and compiles (parents are added automatically); SConstruct passes it by default. A missing class shows up as a missing `classes/*.hpp` header, or as a godot-cpp method that does not exist because one of its argument types is excluded. Add the class there; do not drop the profile. godot-cpp regenerates its bindings only when `godot-cpp/gen/` is absent or with `generate_bindings=yes`: after editing the profile, delete `godot-cpp/gen/`, or stale headers of excluded classes hide the error until link time.

## Engineering Principles

### General

These apply to every language in the repository (C++, GDScript, Python).

- **Code is documentation** — Names and structure carry the meaning, so code has no comments except: a header on a class or struct; a very complex method; a piece of code that is not self-explanatory, such as a non-trivial algorithm that needs a step-by-step guide, a non-obvious invariant, a cross-thread protocol or an engine or FMOD quirk being worked around. Properties and plain methods get none, and nothing restates what the code does.
- **Avoid needless helpers** — Keep trivial single-use code inline. Extract a helper only when it has multiple call sites or its body is substantial enough to make the caller clearer.
- **KISS** — Prefer the simplest design that clearly solves the current problem.
- **YAGNI** — Do not add behavior or abstractions for requirements that do not exist yet.
- **DRY** — Keep each piece of knowledge and behavior in one authoritative place.

### C++

- **Lambdas** — Not banned, but use one only when strictly necessary (a callback API) or when the code would be markedly more complex without it. Prefer a plain loop, a file-local function, or a data table over a local lambda that merely groups a few lines.
- **Namespaces** — Headers wrap their declarations in a `namespace godot { }` block and `.cpp` files start with `using namespace godot;` after their includes, so no `godot::` prefixes appear; the FMOD callbacks live in `namespace Callbacks` instead. No anonymous namespaces: file-local helpers are `static` free functions in the `.cpp` (or private members when they belong to the class). Macro bodies stay fully qualified because they expand at the use site.
- **Includes** — Every file includes what it uses directly and nothing it does not; do not rely on a header reaching you through another include (the build profile turns most such reliance into a build error). When a header only includes something for its `.cpp`, move the include to the `.cpp`. godot-cpp headers are included without the `godot_cpp/` prefix (`<classes/node3d.hpp>`). Build `editor`, `template_debug` and `template_release` after touching includes: the three targets compile different `#ifdef` branches.
- **clang-format** — Run `clang-format -i` with the repo `.clang-format` over every `.cpp` and `.h` you touch under `src/`, and only those: most of `src/` predates the current configuration.
- **Initialization** — Use `=` for scalar values and expressions already of the declared type (`int count = 0;`, `String name = get_name();`). Use direct `()` initialization whenever an object is constructed from something of another type, whether its constructor is explicit or implicit: `Type value(expr);`, never `Type value = Type(expr);`. Use `()` for constructor calls with several arguments (`Vector3 pos(1, 2, 3);`), `{}` only for arrays, aggregates, `std::initializer_list` parameters, and returned/passed temporary aggregates (`return {ptr, size};`). Member initializer lists use `()`, default member initializers use `=`. Never `Type value{expr};`, never `Type value();`, never `Foo f{};` on a class type (write `Foo f;`); zero PODs with `= {}`.
- **Flags** — Several `bool` flags of one struct or class are a bit-field, one bit each (`bool dirty : 1;`), declared next to each other so they share bytes. C++17 gives bit-fields no default member initializer: a constructor sets them, in its initializer list.
- **FMOD results** — Check every FMOD call: `ERROR_CHECK_WITH_REASON(result, reason)` pushes a Godot error naming the operation and returns `false` on failure, `ERROR_CHECK(result)` only tests it. Log with `GODOT_LOG_INFO` / `GODOT_LOG_VERBOSE` / `GODOT_LOG_WARNING` / `GODOT_LOG_ERROR` (`src/helpers/common.h`).

### GDExtension

- **Registration** — Every `GDCLASS` (and `FMODCLASS`) must be registered in `register_types.cpp` (editor classes at the editor level, under `TOOLS_ENABLED`), declare its own `static void _bind_methods()`, even empty, and have its entry in `doc_classes/`.
- **FMOD enums** — An FMOD enum or flag set the API needs is declared in the class it belongs to, the godot-cpp way: a class `enum` (`: uint32_t` for flags) whose values are FMOD's constants, `VARIANT_ENUM_CAST` or `VARIANT_BITFIELD_CAST`, and `BIND_ENUM_CONSTANT` or `BIND_BITFIELD_FLAG`; `fmod_enum_cast` converts to and from FMOD's types. The enum is named in PascalCase without `Fmod` and its values without FMOD's prefix (`FmodEvent.StudioStopMode.IMMEDIATE`), keeping one more word of the prefix only where a value would clash with another enum of its class or start with a digit; FMOD's `FORCEINT` and `MAX` placeholders are never bound. It is registered only while a bound method, property, signal or value handed to scripts uses it: every change to the public API, new, modified or removed, registers the enums it starts using and unregisters those nothing uses anymore, keeping their C++ declaration.
- **Callables** — Prefer `callable_mp(this, &Class::method)` over string-named `Callable`s; bind a method only when the engine or a script calls it by name.
- **StringNames** — A constant `StringName` used on a hot path is built once, as a member of `FmodStringNames`, not on every call.
- **Engine singletons** — Server singletons (`RenderingServer`…) are registered after the extension's scene level: reach them from deferred calls or later, never from `initialize_fmod_module`.

## Prerequisites

- **Python 3** with **SCons** and a C++ compiler: MSVC on Windows, GCC or Clang on Linux, Xcode on macOS and iOS.
- **The FMOD Studio API SDK** of version `FMOD_VERSION` (`20400` is 2.04.00), for each platform you build. `python get_fmod.py <FMOD account> <FMOD password> <platform> <version>` downloads one platform's SDK and installs it into `libs/` (in CI, only the `fmod_sdk.yml` workflow runs it); it needs the user's FMOD account, so ask the user to run it when an SDK is missing, and never handle their password. It lays the SDK out as `libs/<sdk platform>/{core,studio}/{inc,lib}` at the repository root (ignored by git, which keeps only the `libs/PUT_FMOD_LIBS_HERE_TO_BUILD` placeholder), where `<sdk platform>` is `windows`, `linux`, `osx`, `ios` or `android` and each folder is the `api/` directory of the FMOD installer renamed; or pass `fmod_lib_dir=<dir>/`. `#define FMOD_VERSION` in `core/inc/fmod_common.h` tells what is installed.
- **Android:** the NDK godot-cpp defaults to (`ndk_version` in `godot-cpp/tools/android.py`, 28.1.13356709) under `ANDROID_HOME`, and JDK 17 for the Android plugin (its Gradle 8.0 does not run on newer JDKs).
- **Web:** the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html) of version `EMSCRIPTEN_VERSION`, activated so that `emcc` is on the `PATH`.
- **clang-format** for C++ changes.

## Build Commands

### Native GDExtension (SCons)

SCons, run from the repository root, builds the library straight into `demo/addons/fmod/libs/<platform>/` (Android: `libs/android/<arch>/`) and copies the FMOD runtime libraries next to it. Platforms use godot-cpp naming (`windows`, `linux`, `macos`, `android`, `ios`). Commands and options: `build-native` skill.

```bash
scons platform=windows arch=x86_64 target=editor -j<cores>
scons platform=windows arch=x86_64 target=template_debug -j<cores>
scons platform=windows arch=x86_64 target=template_release -j<cores>
```

`editor` and `template_debug` link FMOD's logging libraries (`fmodL`, `fmodstudioL`), `template_release` the plain ones. `editor` and `template_debug` also compile `doc_classes/*.xml` into the library (`src/gen/doc_data.gen.cpp`, generated). `demo/addons/fmod/fmod.gdextension` lists every library and FMOD dependency file name; keep it in sync with SConstruct and the FMOD version.

iOS builds a static library, `libGodotFmod.ios.<target>.a` with godot-cpp merged in by `libtool`, and `ios_simulator=yes` a simulator one per architecture, `libGodotFmod.ios.<target>.simulator.<arch>.a`. The addon ships static xcframeworks, `libGodotFmod.ios.<target>.xcframework` (the device library and the simulator ones merged with `lipo`) and FMOD's `fmod[L].xcframework` and `fmodstudio[L].xcframework` (the `.gdextension` dependencies, from FMOD's `_iphoneos` and `_iphonesimulator` libraries), which `assemble_ios.yml` assembles with `xcodebuild -create-xcframework`; the app links them. Never ship a dynamic iOS library, xcframework-wrapped or not: Godot's iOS export embeds it as a framework signed under an identifier derived from its file name, which App Store Connect rejects with ITMS-90334 (issue #438).

`CMakeLists.txt` is for IDEs (CLion) only; SCons is the build.
The web builds a side module, `libGodotFmod.web.<target>.wasm32[.nothreads].wasm`, loaded by Godot's `dlink` templates (`Extensions Support` in the preset), with FMOD's static Studio library linked in: the single-threaded one with `threads=no` (the default on the web), the pthread one with `threads=yes`; the single-threaded objects have no atomics, so they cannot go into a threaded module. FMOD's web libraries assume they are linked into the main module, and three constraints follow, under `WEB_ENABLED`: no `-sASYNCIFY` (a side module built with it fails to load, so FMOD's asynchronous file reads cannot be used and files are read synchronously, without the file runner thread), a JavaScript shim in `FmodServer::init` exposing FMOD's mixer functions on `Module` for its audio worklet (and starting that worklet on a key press, where FMOD only waits for a click or touch), and, with threads, the `AUDIOWORKLET` output instead of the default `AUDIOWORKLETP`, which needs Emscripten Wasm Audio Worklets Godot's runtime lacks. FMOD has no plugin loading, live update or networking on the web. FMOD 2.04.00's web libraries mix a stereo sound played through the Core API as a mono one once a Studio event was stopped: `FmodServer::create_sound_instance` sets its mix matrix explicitly; remove that once a fixed FMOD release is used.

### Android plugin (Gradle)

```bash
cd android-plugin && ./gradlew :library:assemble
```

Build the four Android libraries first (`template_debug` and `template_release`, `arch=arm64` and `arch=x86_64`). The extension is an Android AAR plugin (`android_aar_plugin = true` in `fmod.gdextension`): Godot's Android export skips its `.gdextension` and libraries, and each AAR carries them instead. `fmod-debug.aar` holds `libGodotFmod.android.template_debug.<arch>.so` with `libfmodL`/`libfmodstudioL` for both ABIs, `fmod-release.aar` the release library with `libfmod`/`libfmodstudio`, and both hold `fmod.gdextension` under `assets/addons/fmod/` and FMOD's Java side. Gradle reads the libraries from `demo/addons/fmod/libs/android/<arch>/` (or `-PnativeLibrariesDir=`) and fails when one is missing. It needs FMOD's `fmod.jar` (from the Android SDK's `core/lib/`) in `android-plugin/library/libraries/`, and copies both AARs into `demo/addons/fmod/libs/android/`, where `FmodAndroidExportPlugin.gd` hands the export's variant to Gradle exports.

## Testing

`demo/test/unit/` holds the [gdUnit4](https://github.com/godot-gdunit-labs/gdUnit4) suites (addon in `demo/addons/gdUnit4/`, kept on a release that supports `GODOT_VERSION`), run in the editor, in desktop exports and in web exports by `demo/run_tests.sh`; `demo/editor_monkey/` drives the editor addon like a user; `run_tests.sh web-demo` plays a demo in a browser. How to run each, and what they need: `run-tests` skill. Every new feature or change in the editor experience (an FMOD node, the editor plugin, the explorer, an Inspector editor) is added to the editor monkey in the same change. The export plugin has no automated test: run an export after changing it.

### Debugging native code

Build `target=editor` with `debug_symbols=yes` (SConstruct always adds `/Zi` on Windows debug targets), then launch the Godot editor binary on `demo/` under a native debugger (Visual Studio, CLion, lldb or gdb) and break in `src/`. FMOD's own warnings and errors go to Godot's output when the logging settings select it (`Fmod/Logging` in Project Settings), and live update lets FMOD Studio connect to a running game (never to the editor).

## Architecture

### C++ layer (`src/`)

- **`register_types.cpp`** — entry point `fmod_library_init`. Core level: `FmodStringNames` and the `GodotFileRunner`. Scene level: runtime classes, the `FmodServer` singleton, and FMOD initialization from Project Settings when `auto_initialize` is on (always in the editor). Editor level: `FmodEditorPlugin` and `FmodEditorExportPlugin`.
- **`fmod_server.*`** — `FmodServer`: FMOD system lifecycle (`init`, `update`, `shutdown`), banks, event instances, one-shots, listeners, buses/VCAs, global parameters, files and sounds, plugins, drivers and performance data. Its `update()` runs every frame from the `FmodManager.gd` autoload.
- **`fmod_cache.*`** — loaded files and banks, and the event descriptions, buses and VCAs they hold by GUID; banks loaded without blocking are finished in `update_pending()`.
- **`studio/`** — `RefCounted` wrappers of FMOD Studio objects, declared with `FMODCLASS` / `FMODCLASSWITHPATH` (`helpers/common.h`); `FmodBank` unloads its bank when its last reference goes.
- **`core/`** — FMOD Core wrappers: `FmodFile` (a loaded file), `FmodSound` (a playing channel), `FmodChannelGroup` (DSP clock).
- **`nodes/`** — `FmodEventEmitter2D/3D` and `FmodListener2D/3D`, thin classes over the CRTP templates `FmodEventEmitter<Derived, NodeType>` and `FmodListener<Derived, NodeType>` that hold the shared logic; `FmodBankLoader`. None of them runs FMOD in the editor.
- **`resources/`** — settings resources (`FmodGeneralSettings`, software format, DSP, 3D, logging, plugins), each read from Project Settings with `get_from_project_settings()`, and `FmodAudioStream`, the Godot audio stream that plays FMOD's output on a Godot bus.
- **`callback/`** — namespace `Callbacks`: FMOD event callbacks, and the file system callbacks that route FMOD's file reads through Godot's `FileAccess`, so banks load from `res://` and from exported packs alike.
- **`tools/`** — editor only (`TOOLS_ENABLED`): `FmodEditorPlugin`; `FmodEditorExportPlugin` packs banks and audio files into exports (per-preset `fmod/auto_export_banks` option) and FMOD plugin libraries per platform, generating the static registration code on iOS.
- **`plugins/`** — loading FMOD plugin libraries, dynamic or static (iOS).
- **`helpers/`** — logging and FMOD error macros, `FMODCLASS`, `fmod_enum_cast` (converts between the classes' enums, declared with FMOD's constants as values, and FMOD's own types), transform conversions (`maths.h`) and file listing.

### Editor addon (`demo/addons/fmod/`)

GDScript, `@tool`: `FmodPlugin.gd` (toolbar button, bank explorer window, Inspector plugins, export plugins, the `FmodManager` autoload), `FmodManager.gd` (calls `FmodServer.update()` every frame, forwards notifications, suspends the mixer on mobile focus loss, registers the performance monitors), `tool/FmodBankDatabase.gd` (loads the master banks from `Fmod/General/banks_path`), `tool/ui/` (bank explorer, event audition, parameter display), `tool/inspectors/` and `tool/property_editors/`. The released addon is this folder, `README.md` and `LICENSE`, with every desktop and iOS library and the two Android AARs in `libs/` (no bare Android library).

### Export presets (`demo/export_presets.cfg`)

One preset per platform for the demo (exports go to `demo/export/`, ignored), leaving the tests out, and `tests_linux`, `tests_macos`, `tests_windows`, `tests_web`, `tests_web_threads`, `tests_ios` (exporting the Xcode project only) and `tests_android` (a Gradle build for arm64 and x86_64, started with the Compatibility renderer, since the emulator's software Vulkan cannot present) for the export tests: `export-tests` custom feature, text scripts (gdUnit4 discovers tests from their source) and gdUnit4's editor scenes left out (packing them crashes the headless editor on shutdown, gdUnit4 #1151). The two web presets also leave out `test_godot_output.gd`, the Godot bus output being unavailable on the web. Options a preset omits take Godot's defaults.

### Threading

FMOD calls its event and file callbacks from its own threads, and the scene tree must not be touched from there: event callbacks are queued under `FmodServer::callback_mutex` and invoked from `update()` on the main thread, dropping those whose target was freed. File reads are served by `Callbacks::GodotFileRunner`, a dedicated thread with its own request queue, started and stopped at the core initialization level.

### Data flow (FMOD Studio → game)

```
Sound designer builds banks in FMOD Studio
  → banks placed under res:// (Fmod/General/banks_path for the editor addon)
  → FmodBankLoader or FmodServer.load_bank() loads them through Godot's FileAccess
  → FmodCache indexes their events, buses and VCAs by path and GUID
  → emitters / FmodServer create event instances, positioned from nodes every frame
  → FmodManager calls FmodServer.update(): pending banks, callbacks, one-shots, listeners, FMOD update
  → export: FmodEditorExportPlugin packs banks, audio files and plugin libraries
```

## Key Gotchas

- **Master banks first** — `Master.strings.bank` then `Master.bank` must be loaded before any other bank; the bank explorer reads them from `Fmod/General/banks_path`.
- **2D distances** — 2D positions are in pixels: the `distance_factor` 3D setting must be the number of pixels per meter, or 2D attenuation is wrong.
- **Editor resaves demo scripts** — Opening `demo/` with an editor that applies personal settings can rewrite the indentation of its scripts: run tests with a self-contained editor (an empty `._sc_` file next to its binary), or revert what a run changed under `demo/` before committing.
- **godot-cpp after a branch switch** — A godot-cpp checked out at another commit, or bindings generated for another one, build a library the editor rejects or crashes on: `git submodule update godot-cpp`, then delete `godot-cpp/gen/`.
- **Locked libraries on Windows** — An editor open on `demo/` holds the DLLs, and the link fails with LNK1104; close it first.
- **A custom main loop still loads the main scene** — Hence the empty main scene of test exports: the demo's would play events and load files of its own while the suites count them.

## CI/CD

Structured like godot-jvm's: `trigger_ci.yml` (pull requests, manual runs, and a weekly run of `master`, on Mondays) and `trigger_on_tag.yml` (tags `x.y.z-a.b.c`) call reusable workflows, and the versions live at their top (`GODOT_VERSION`, `FMOD_VERSION`, passed down through YAML anchors).

- **FMOD SDK** — `fmod_sdk.yml`, run by hand on `master` with an FMOD version, is the only workflow with the `FMODUSER`/`FMODPASS` secrets: it downloads every platform's SDK on macOS (`hdiutil`), keeps only the headers, the libraries SConstruct links and copies, and `fmod.jar`, and caches them as `fmod-sdk-<version>` (`fmod-sdk-cache` action), which every pull request and tag can read. Several versions can be cached at once, for pull requests on either side of an FMOD upgrade: run it with the new version before changing `FMOD_VERSION`. The first job of `trigger_ci.yml` and `trigger_on_tag.yml` fails when `FMOD_VERSION` is not cached, and so does a version unused for 7 days, which GitHub evicts. Pull requests, external ones included, run on `pull_request` without secrets.
- **Build** — `build_<platform>.yml`: Linux, Windows (x86_64) and macOS (universal) build `editor`, `template_debug` and `template_release`; Android (arm64, x86_64) and iOS (arm64, and arm64 and x86_64 for the simulator) build the two templates, and the web builds them without and with threads (`_threads` artifacts) with Emscripten `EMSCRIPTEN_VERSION`. Each target is an artifact named `<target>_<platform>[_<arch>]`, holding `demo/addons/fmod/libs/<platform>/`. Shared steps are the `gdextension-build` and `fmod-sdk-cache` actions.
- **Assemble** — one job per platform, each producing the `fmod-gdextension-<platform>` artifact the tests and the addon use: `assemble_libraries.yml` merges the targets of Linux, Windows, macOS and the web, `assemble_ios.yml` wraps the iOS libraries into xcframeworks, and `assemble_android.yml` builds the two AARs from the Android artifacts and FMOD's `fmod.jar`, failing on anything else in the package. The graph keeps godot-jvm's four columns: build, assemble, test, package.
- **Test** — one job per platform. `test_desktop.yml`, called for Linux, macOS and Windows: `run_tests.sh` in editor mode and in `export-release` and `export-debug` modes, against the official editor and export templates of `GODOT_VERSION` (`download-godot` action), as godot-jvm does. Every other platform tests a debug and a release export too. `test_ios.yml`, for a debug and a release export, exports the `tests_ios` Xcode project, builds it unsigned for a device and fails when the extension is embedded in the app instead of linked into its binary (issue #438), then builds it for the x86_64 simulator (Godot's official simulator template has no other slice; Apple Silicon runs it under Rosetta) and runs the tests on a booted iPhone simulator, reading the runner's `Export tests exit code` line from its log. `test_android.yml`, aligned on godot-jvm's, exports `tests_android` in debug and in release with the matching AAR and runs the tests on an x86_64 emulator (`reactivecircus/android-emulator-runner`); `.github/scripts/test_android_emulator.sh` reads the same line from logcat and also requires as many test cases as `func test_` in `demo/test/`. `test_web.yml` runs the web tests without and with threads, in Chrome and Firefox under Xvfb, one job per browser and threading mode running the release export then the debug export. `test_editor_monkey.yml` runs the editor monkey on Linux against the Linux editor library, uploading `monkey.log` when it fails.
- **Package addon** — `assemble_addon.yml` runs once every assemble and test job passes, and uploads `fmod-addon` (`addons/fmod/`), after checking that every library and dependency `fmod.gdextension` names is there. On a tag, the release job zips it as `addons.zip`, zips the demo project without its tests as `demo.zip`, and creates the GitHub release.
- **Caches** — SCons' object cache per build job (`scons-cache`, `SCONS_CACHE`, which godot-cpp's SConstruct reads) and the Android plugin's Gradle dependencies (`gradle-deps-cache`), through `master-cache`: every run restores them by exact key, and only `master` runs, the weekly scheduled one, save them back, replacing the previous entry, so branches and pull requests only read.
- `check-specs` validates `specs/`.

## Documentation

- User guide (MkDocs, published on readthedocs): `docs/src/doc/`; serve locally with `cd docs && ./run.sh`, build with `./build.sh`. Update the pages with the code they describe.
- Compiling from source: `docs/src/doc/advanced/1-compiling.md`
- Class reference: `doc_classes/*.xml`, shown in the editor's help; update it with the bound API.
