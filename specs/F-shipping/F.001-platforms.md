# F.1: Platforms

The addon runs the same game on the desktop, mobile and web platforms Godot exports to, from the editor and from debug and release exports.

Each platform needs its native library for the editor (desktop only) and for both export templates, with the matching FMOD runtime, and mobile platforms need their own packaging: an Android plugin bringing FMOD's Java side, a static xcframework on iOS. The web needs two libraries with FMOD linked in, one for exports with threads and one for exports without. The addon ships all of it (§4), and every feature works on each of them (§3).

Exporting the game's audio content is F.2.

## Capabilities
- **F.1.1**: Game developers use FMOD in the Godot editor on Windows x86_64, Linux x86_64 and macOS.
- **F.1.2**: Games export with FMOD to Windows x86_64, Linux x86_64 and macOS, in debug and release.
- **F.1.3**: Games export with FMOD to Android arm64, the Android plugin being added to the Gradle build without game developer action.
- **F.1.4**: Games export with FMOD to Android x86_64.
- **F.1.5**: Games export with FMOD to iOS.
- **F.1.6**: Games export with FMOD to the web.
- **F.1.7**: Games built for iOS run in the iOS simulator.

## Rules
- **F.1.R1**: A debug export and the editor use FMOD's logging libraries, and a release export FMOD's release libraries.
  - Why: The logging libraries report FMOD's warnings and errors while developing; players get the leaner build.
