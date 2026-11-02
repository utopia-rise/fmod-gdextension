---
name: run-tests
description: Run the gdUnit4 test suites of the demo project against the current native build, in the editor or in a release export, as CI does. Use to verify a change to src/ or to the addon end to end.
argument-hint: "[path to the Godot editor binary] [editor | export-release | export-debug]"
---

# Run the gdUnit4 tests

## Before

- **Native library:** build the host `editor` target in this checkout (`build-native` skill). The tests load `demo/addons/fmod/libs/<platform>/libGodotFmod.<platform>.editor.*` and the FMOD logging libraries SCons copied next to it.
- **Godot editor:** any official editor from `compatibility_minimum` in `demo/addons/fmod/fmod.gdextension` onwards (GDExtensions are forward compatible); CI uses `GODOT_VERSION` from `.github/workflows/trigger_ci.yml`. On Windows use the console build so output reaches the terminal. gdUnit4 (`demo/addons/gdUnit4/`) must support that Godot version: its README lists them.
- **Self-contained editor:** the script opens the project, which applies the user's editor settings and may resave scripts (indentation, for one). Copy the editor binaries to a scratch folder and create an empty `._sc_` file next to them, so it runs with default settings, or check `git status` afterwards and revert what the run changed under `demo/`.
- Close any editor open on `demo/`: it holds the library (Windows) and the import cache.
- **Export modes:** also build the host's `template_release` (or `template_debug`) library and install the matching official export template (`editor_data/export_templates/<version>/` for a self-contained editor). Godot needs only the template of the mode used.

## Run

```bash
cd demo && ./run_tests.sh <path to the Godot editor binary> [editor|export-release|export-debug]
```

On Windows run it from Git Bash. It imports the project headless (three attempts, then a headless editor as fallback). In editor mode it then runs `res://addons/gdUnit4/bin/GdUnitCmdTool.gd -a res://test -c --ignoreHeadlessMode`, whose exit code it returns: 0 on success, 100 on a failure or error, 101 when tests leave orphan nodes.

`export-release` and `export-debug` then export the host's `tests_<platform>` preset to `demo/export/tests/` and run the exported game headless: `test_runner/ExportTestMain.gd` becomes the main loop (the preset's `export-tests` feature) and runs every suite under `res://test` with gdUnit4's export-safe core, exiting with 0 or 100. An export without templates fails before running anything.

A single suite runs in the editor with `-a res://test/unit/<suite>.gd` instead of `-a res://test`, on the same command line as in the script.

## Write a test

Suites live in `demo/test/unit/`, extend `GdUnitTestSuite`, load the banks they need in `before()` and release what they created in `after()`; test methods start with `test_`. Free every node a test creates (`auto_free(...)` for nodes local to a test), or the run ends with 101. Wait for FMOD with `await await_millis(...)`.

## Read the result

- One `Statistics:` line per suite, then `Overall Summary: <n> test cases | <e> errors | <f> failures | … | <o> orphans` and `Executed test cases : (<run>/<total>)`.
- `FMOD Sound System: Successfully initialized` must appear; without it, the library did not load and every FMOD call fails.
- `SCRIPT ERROR` lines, and `ERROR:` lines from FMOD, even when the run passes. Two are expected: the user property tests read properties the car engine event does not have (`Cannot get user property …`).
- The headless editor reports leaks at exit (`RID allocations … leaked`, `resources still in use`) and the dummy remote debugger port (`Unable to connect to host '127.0.0.1:0'`); neither comes from the extension.
- HTML and XML reports are written to `demo/reports/` (ignored by git).

Report the overall summary and any error line.
