---
name: check-cpp
description: Verify a C++ change before committing - clang-format the touched files and build the editor, template_debug and template_release targets, which compile different #ifdef branches. Use after any edit under src/.
---

# Check a C++ change

1. **Format.** Run `clang-format -i` with the repo's `.clang-format` on every `.cpp` and `.h` you touched under `src/`. Format only what you touched: most of `src/` predates the current `.clang-format`, and reformatting untouched files buries the change in noise.
   ```bash
   git diff --name-only HEAD -- 'src/*.cpp' 'src/*.h' | xargs clang-format -i
   ```
2. **Build the three targets** for the host platform, in this order, each one to completion (`build-native` skill):
   ```bash
   scons platform=<host> target=editor -j<cores>
   scons platform=<host> target=template_debug -j<cores>
   scons platform=<host> target=template_release -j<cores>
   ```
   `TOOLS_ENABLED` and `DEBUG_ENABLED` code only compiles in some of them, so an include or a symbol missing from one branch only shows in that target. `template_release` also links the non-logging FMOD libraries.
3. **Bound API.** When a bound method, property, signal or class changed, update its XML in `doc_classes/` and the matching page in `docs/`.
4. **Review against the rules** in the root `CLAUDE.md` (Engineering Principles): includes are exactly what the file uses, the namespace conventions, the initialization style, every FMOD result checked, no comment that restates the code.

Report each target's result. A warning introduced by the change counts as a problem to fix.
