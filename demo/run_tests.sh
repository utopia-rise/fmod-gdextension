if [ -z "$1" ]; then
  echo "ERROR: Please provide the path to the Godot executable as the first argument."
  exit 1
fi

GODOT_BIN="$1"
PROJECT_PATH="$PWD"

# Pre-import the project (headless). Try --import first, then fall back to headless editor.
if ! "$GODOT_BIN" --headless --path "$PROJECT_PATH" --import; then
  echo "INFO: '--import' failed or unsupported. Falling back to headless editor import..."
  if ! "$GODOT_BIN" --headless --path "$PROJECT_PATH" --editor --quit; then
    echo "WARNING: Pre-import step failed. Continuing anyway..."
  fi
fi

"$GODOT_BIN" -s --headless --path "$PROJECT_PATH" addons/gut/gut_cmdln.gd | (
    tests=0
    passing=0

    while read -r line
    do
        echo "$line"

        # Capture the number of tests
        if echo "$line" | grep -q "Tests"; then
            tests=$(echo "$line" | awk '{print $2}')
        fi

        # Capture the number of passing tests
        if echo "$line" | grep -q "Passing"; then
            passing=$(echo "$line" | awk '{print $2}')
        fi
    done

    if [[ "$tests" -eq 0 ]]; then
       echo "ERROR: No tests were found."
       exit 1
    fi

    if [[ "$tests" -eq "$passing" ]]; then
        exit 0
    else
        echo "ERROR: Some assertions failed!"
        exit 1
    fi
)

result=$?

# Exit with the result of the subshell
exit $result
