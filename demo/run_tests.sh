#!/usr/bin/env bash
# Usage: run_tests.sh <Godot editor binary> [editor|export-release|export-debug]
#        run_tests.sh <Godot editor binary> <web-release|web-debug> <browser executable> [nothreads|threads] [window]
#        run_tests.sh <Godot editor binary> web-demo <browser executable> [scene]
# editor (the default) runs the gdUnit4 suites in the editor; export-release and export-debug export the tests_<host>
# preset with that template and run the suites in the exported game (test_runner/), which exits with 0 or 100.
# web-release and web-debug export tests_web (tests_web_threads with threads) and run it in a headless browser, or in a
# window with window. web-demo exports the Web preset with the given scene (the project's main scene by default) as
# its main scene and opens it in the browser, until its window is closed.
if [ -z "$1" ]; then
  echo "ERROR: Please provide the path to the Godot executable as the first argument."
  exit 1
fi

GODOT_BIN="$1"
MODE="${2:-editor}"
PROJECT_PATH="$PWD"
DELAY_SEC=5
MAX_ATTEMPTS=3

retry_import() {
  for (( attempt=1; attempt<=MAX_ATTEMPTS; attempt++ )); do
    echo "INFO: Attempt $attempt/$MAX_ATTEMPTS: waiting $DELAY_SEC before import…"
    sleep "$DELAY_SEC"

    if "$GODOT_BIN" --headless --path "$PROJECT_PATH" --import; then
      echo "INFO: Import succeeded on attempt $attempt."
      return 0
    else
      echo "WARN: Import failed on attempt $attempt. Retrying…"
    fi
  done
  return 1
}


# Pre-import the project (headless). Try --import first, then fall back to headless editor.
if ! retry_import; then
  echo "INFO: All --import attempts failed. Falling back to headless editor import…"
  sleep "$DELAY_SEC"
  if ! "$GODOT_BIN" --headless --path "$PROJECT_PATH" --editor --quit; then
    echo "WARNING: Fallback import (--editor --quit) failed. Continuing anyway…"
  fi
fi

if [ "$MODE" = "editor" ]; then
  # --remote-debug to a port nothing listens on keeps a script error from dropping into Godot's interactive debugger.
  # GdUnit exits with 0 on success, 100 on failures or errors and 101 when tests leave orphan nodes.
  exec "$GODOT_BIN" --headless --path "$PROJECT_PATH" -s -d --remote-debug tcp://127.0.0.1:0 \
    res://addons/gdUnit4/bin/GdUnitCmdTool.gd -a res://test -c --ignoreHeadlessMode
fi

if [ "$MODE" = "web-release" ] || [ "$MODE" = "web-debug" ]; then
  BROWSER="$3"
  PRESET=tests_web
  [ "$4" = "threads" ] && PRESET=tests_web_threads
  EXPORT_DIR="$PROJECT_PATH/export/tests"
  rm -rf "$EXPORT_DIR"
  mkdir -p "$EXPORT_DIR"
  "$GODOT_BIN" --headless --path "$PROJECT_PATH" "--export-${MODE#web-}" "$PRESET" "$EXPORT_DIR/index.html" || exit 1
  HEADLESS=--headless
  [ "$5" = "window" ] && HEADLESS=
  exec python test_runner/run_in_browser.py "$EXPORT_DIR" "$BROWSER" $HEADLESS
fi

if [ "$MODE" = "web-demo" ]; then
  BROWSER="$3"
  EXPORT_DIR="$PROJECT_PATH/export/web"
  rm -rf "$EXPORT_DIR"
  mkdir -p "$EXPORT_DIR"
  # Official export templates ignore --scene: the scene is the project's main scene while it exports.
  if [ -n "$4" ]; then
    cp project.godot "$EXPORT_DIR/project.godot.saved"
    sed -i "s#^run/main_scene=.*#run/main_scene=\"$4\"#" project.godot
  fi
  "$GODOT_BIN" --headless --path "$PROJECT_PATH" --export-release Web "$EXPORT_DIR/index.html"
  EXPORTED=$?
  if [ -n "$4" ]; then
    mv "$EXPORT_DIR/project.godot.saved" project.godot
  fi
  [ $EXPORTED = 0 ] || exit 1
  exec python test_runner/run_in_browser.py "$EXPORT_DIR" "$BROWSER" --demo
fi

case "$(uname -s)" in
  Linux*) PLATFORM=linux; EXPORT_FILE=tests.x86_64; EXECUTABLE=tests.x86_64 ;;
  Darwin*) PLATFORM=macos; EXPORT_FILE=tests.zip ;;
  MINGW*|MSYS*|CYGWIN*) PLATFORM=windows; EXPORT_FILE=tests.exe; EXECUTABLE=tests.console.exe ;;
  *) echo "ERROR: Unsupported host $(uname -s)."; exit 1 ;;
esac

EXPORT_DIR="$PROJECT_PATH/export/tests"
rm -rf "$EXPORT_DIR"
mkdir -p "$EXPORT_DIR"
"$GODOT_BIN" --headless --path "$PROJECT_PATH" "--$MODE" "tests_$PLATFORM" "$EXPORT_DIR/$EXPORT_FILE" || exit 1

if [ "$PLATFORM" = "macos" ]; then
  unzip -q "$EXPORT_DIR/$EXPORT_FILE" -d "$EXPORT_DIR"
  EXECUTABLE="$(find "$EXPORT_DIR"/*.app/Contents/MacOS -type f -perm -u+x | head -n 1)"
  EXECUTABLE="${EXECUTABLE#"$EXPORT_DIR/"}"
fi

if [ ! -f "$EXPORT_DIR/$EXECUTABLE" ]; then
  echo "ERROR: The export produced no $EXECUTABLE:"
  ls -R "$EXPORT_DIR"
  exit 1
fi
exec "$EXPORT_DIR/$EXECUTABLE" --headless

