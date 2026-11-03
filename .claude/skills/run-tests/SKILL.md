---
name: run-tests
description: Every test of the repository - the gdUnit4 suites in the editor, in a desktop export and in a web export run in a headless browser, the editor monkey - and the demos played in a browser for a human to check. Use to verify a change to src/ or to the addon end to end, after building a platform's libraries, or whenever the user asks to test or play a demo, for example "I want to test the web demo for sound" or "open the 3D demo in Firefox".
argument-hint: "[editor | export-release | export-debug | web-release | web-debug | web-demo | monkey] [browser] [nothreads | threads | scene]"
---

# Test FMOD GDExtension

Every mode runs from `demo/` with Git Bash on Windows and takes the Godot editor binary first. Pick the mode from what
was built or asked:

| Mode | Command | Checks |
|---|---|---|
| Editor | `./run_tests.sh <Godot>` | the gdUnit4 suites in a headless editor, against the host `editor` library |
| Desktop export | `./run_tests.sh <Godot> export-release` (or `export-debug`) | the suites in an exported game, with that template |
| Web export | `./run_tests.sh <Godot> web-release <browser> [nothreads\|threads]` (or `web-debug`) | the suites in a web export, in a headless browser |
| Web demo | `./run_tests.sh <Godot> web-demo <browser> [scene]` | a demo in a browser window, for a person to play and listen to |
| Editor monkey | `editor_monkey/run.sh <Godot>` | the editor addon and FMOD nodes driven like a user |

Mobile exports have no test yet.

Run one Godot at a time on `demo/`: two runs share `export/` and the import cache and corrupt each other.

## Before

- **Godot editor:** any official editor from `compatibility_minimum` in `demo/addons/fmod/fmod.gdextension` onwards
  (GDExtensions are forward compatible); CI uses `GODOT_VERSION` from `.github/workflows/trigger_ci.yml`. On Windows use
  the console build so output reaches the terminal. gdUnit4 (`demo/addons/gdUnit4/`) must support that Godot version:
  its README lists them.
- **Libraries** (`build-native` skill): every mode needs the host `editor` library (the export plugin packs the banks
  into exports); desktop export modes also the host's `template_release` or `template_debug` library; web modes the web
  libraries of that template, with and without threads (`web-demo` uses the release ones without threads).
- **Export templates:** the official template of the mode (`web_dlink_nothreads_<mode>.zip` and `web_dlink_<mode>.zip`
  for the web) under `editor_data/export_templates/<version>/` of a self-contained editor, or the user's templates.
- **Browser** for web modes: Chrome, another Chromium (Brave, Edge) or Firefox, given by its executable. Default to
  Chrome; use the one the user names.
- **Self-contained editor:** the scripts open the project, which applies the user's editor settings and may resave
  scripts and `project.godot` (indentation, the `config/features` of a newer Godot). Run with a self-contained editor
  (an empty `._sc_` file next to its binary), or check `git status` afterwards and revert what the run changed under
  `demo/`. Close any editor open on `demo/`: it holds the library (Windows) and the import cache.

## Suites: editor and desktop export

`run_tests.sh` imports the project headless (three attempts, then a headless editor as fallback). In editor mode it
runs `res://addons/gdUnit4/bin/GdUnitCmdTool.gd -a res://test -c --ignoreHeadlessMode` and returns its exit code: 0 on
success, 100 on a failure or error, 101 when tests leave orphan nodes. A single suite runs with
`-a res://test/unit/<suite>.gd` on the same command line.

`export-release` and `export-debug` export the host's `tests_<platform>` preset to `demo/export/tests/` and run the
game headless: the preset's `export-tests` feature makes `test_runner/ExportTestMain.gd` the main loop and an empty
scene the main scene, and `test_runner/ExportTestRunner.gd` runs every suite with gdUnit4's export-safe core, exiting
with 0 or 100.

## Suites: web export

`web-release` and `web-debug` export `tests_web` (`tests_web_threads` with `threads`) and run it through
`test_runner/run_in_browser.py`: it serves the export with the cross-origin isolation headers threaded exports need,
injects a script forwarding the page's console and simulating key presses (FMOD starts its audio on a user gesture),
launches the browser headless on a throwaway profile allowing audio, and returns the code of the
`Export tests exit code` line `ExportTestRunner.gd` prints, or 1 after ten minutes without it. Headless is the default:
a browser window can be hidden by the desktop, which stops Godot's web main loop. Pass `window` after the threading
argument to show it (CI does, in a virtual display).

## Web demo

For "test the web demo", "listen to the 2D demo in Firefox" and the like. `web-demo` exports the `Web` preset
(release, no threads) to `demo/export/web/` with the given scene as its main scene, for the time of the export
(official templates ignore `--scene`), then opens it in a window of the browser, on a throwaway profile allowing audio,
and serves it until that window is closed. Scenes:

| Demo | Scene |
|---|---|
| 2D, low level (the default) | `res://low_level_2D/FmodScriptTest.tscn` |
| 2D, FMOD nodes | `res://high_level_2D/FmodNodesTest.tscn` |
| 3D | `res://high_level_3D/World.tscn` |

Run it in the background, since it lasts as long as the window, and tell the user the browser it opened, to click or
press a key first (browsers start audio on a user gesture), and what to listen to. Several demos or browsers run one
after the other's export, each with its own command. Stop the command when the user is done; it stops the browser it
opened.

## Editor monkey

```bash
cd demo && editor_monkey/run.sh <Godot>
```

`editor_monkey/monkey.gd` drives a headless editor on the demo project like a user: scripted `step_*` methods check the
Fmod Explorer (tree, search, refresh, selection, event audition, parameter display), the emitters' event picker in the
Inspector (through the explorer, a typed path or GUID) and the `fmod_parameters/*` properties it creates, the bank
loader's bank list, the FMOD nodes' properties and the undo history; `step_random_user_actions` then calls `action_*`
methods picked from `SEED` and checks after each that no engine or script error or warning appeared, no event instance
leaked and the scene changed only through the undo history. Undoing everything must empty the scene, redoing everything
must restore it, and the saved scene must reopen unchanged. It passes on `[MONKEY] DONE with 0 failure(s)` with no
`SCRIPT ERROR` in `editor_monkey/monkey.log`. The script runs before the extension's classes are registered: it names
none of them, nor the addon's classes built on them, as types, and reaches them through `ClassDB`,
`Engine.get_singleton()`, `is_class()` and script names. Run it after any change to `src/nodes/`, `src/tools/` or
`demo/addons/fmod/`, and add a `step_*` (and, for an action a user repeats, an `action_*`) for any new editor feature.

## Write a test

Suites live in `demo/test/unit/`, extend `GdUnitTestSuite`, load the banks they need in `before()` and release what
they created in `after()`; test methods start with `test_`. Free every node a test creates (`auto_free(...)` for nodes
local to a test), or the run ends with 101. Wait for FMOD with `await await_millis(...)`. A suite runs in every mode,
web included: it must not depend on threads or on files outside `res://`.

## Read the result

- One `Statistics:` line per suite, then `Overall Summary: <n> test cases | <e> errors | <f> failures | … | <o> orphans`
  and `Executed test cases : (<run>/<total>)`.
- `FMOD Sound System: Successfully initialized` must appear; without it, the library did not load and every FMOD call
  fails.
- `SCRIPT ERROR` lines, and `ERROR:` lines from FMOD, even when the run passes. Two are expected: the user property
  tests read properties the car engine event does not have (`Cannot get user property …`).
- The headless editor reports leaks at exit (`RID allocations … leaked`, `resources still in use`) and the dummy remote
  debugger port (`Unable to connect to host '127.0.0.1:0'`); neither comes from the extension.
- A web run that stops printing before its summary has hung: `ERROR: no test result after 600 seconds.` follows.
- HTML and XML reports of the editor mode are written to `demo/reports/` (ignored by git).

Report the overall summary and any error line; for a demo, what the user heard.
