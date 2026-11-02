#!/usr/bin/env bash
# Usage: run.sh <Godot editor binary>
# Runs the editor addon monkey test in a headless Godot editor on the demo project, which must have been imported
# (run_tests.sh imports it). It passes on "[MONKEY] DONE with 0 failure(s)" with no script error in monkey.log.
if [ -z "$1" ]; then
  echo "ERROR: Please provide the path to the Godot editor executable as the first argument."
  exit 1
fi
here="$(cd "$(dirname "$0")" && pwd)"
project="$(cd "$here/.." && pwd)"
log="$here/monkey.log"
"$1" --headless -e --path "$project" -s res://editor_monkey/monkey.gd > "$log" 2>&1
grep -E "^\[MONKEY\] (FAIL|DONE)|SCRIPT ERROR|Error calling" "$log"
if ! grep -q "\[MONKEY\] DONE with 0 failure" "$log" || grep -qE "SCRIPT ERROR|Error calling" "$log"; then
  echo "editor monkey FAILED, see $log"
  exit 1
fi
echo "editor monkey passed"
