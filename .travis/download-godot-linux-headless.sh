GODOT_VERSION=3.1.1
GODOT_BIN=Godot_v${GODOT_VERSION}-stable_linux_headless.64
GODOT_ZIP=${GODOT_BIN}.zip
GODOT_URL=https://downloads.tuxfamily.org/godotengine/${GODOT_VERSION}/${GODOT_ZIP}

curl ${GODOT_URL} --output ${GODOT_ZIP}
7z x ${GODOT_ZIP}
chmod +x ${GODOT_BIN}