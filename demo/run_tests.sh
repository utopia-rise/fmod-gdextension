if [ -z "$1" ]; then
  echo "ERROR: Please provide the path to the Godot executable as the first argument."
  exit 1
fi

GODOT_BIN="$1"
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


"$GODOT_BIN" -s --headless --path "$PROJECT_PATH" addons/gut/gut_cmdln.gd | (
    tests=0
    passing=0

    while read -r line
    do
        echo "$line"

        # Match line that starts with "Tests"
        if echo "$line" | grep -q "^Tests"; then
            tests=$(echo "$line" | awk '{print $NF}')
        fi

        # Match line that starts with "Passing Tests"
        if echo "$line" | grep -q "^Passing Tests"; then
            passing=$(echo "$line" | awk '{print $NF}')
        fi
    done

    echo "Retrieved values: tests=$tests passing=$passing"

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
