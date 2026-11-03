# Setup guide

## 1 - Install addon folder

We provide releases in github repository. 

You can download `addons.zip` from this [page](https://github.com/utopia-rise/fmod-gdextension/releases), then unzip it
at the root of your Godot project: it contains the `addons/fmod` folder.

## 2 - Activate the plugin in Godot

Open your Godot project, go to your project settings and select the `Plugins` tab.
You should see the line `FMOD GDExtension` and a checkbox. Enable it.
After this, you should be able to create Fmod nodes, use the bank explorer and write scripts using `FmodServer`

## 3 - Android specific

Android exports require a Godot Android plugin. The addon ships it, for debug and release exports, and adds it to the
export by itself.  

Download android build template from project menu:  
![install-android-build-template]  

Make sure the `Use Gradle Build` option is activated in your Android export:  
![android-extension-export-enable]

Android exports support the `arm64-v8a` and `x86_64` architectures.

## 4 - Web specific

Web exports need Godot's GDExtension support: enable `Extensions Support` in your Web export preset. Both values of
`Thread Support` work:

- **Without threads** (Godot's default), FMOD mixes on the browser's main thread, and the game runs on any web host.
- **With threads**, FMOD streams and loads files on its own threads. Like any threaded Godot web export, the game must
  be served with the `Cross-Origin-Opener-Policy: same-origin` and `Cross-Origin-Embedder-Policy: require-corp` headers.
  FMOD starts 9 threads of its own: set the preset's `Emscripten Pool Size` to at least 17 (Godot's default 8 plus 9),
  or the game freezes when FMOD starts. The preset shows a warning below that.

Browsers only start audio after the player interacts with the page: FMOD starts playing on the first click, touch or
key press.

FMOD's web runtime has no live update and cannot load FMOD plugins (see [Plugins](9-plugins.md)).

FMOD 2.04.00's web libraries have a bug with sounds played through the low level API (`FmodServer.load_file_as_sound`,
`load_file_as_music` and `create_sound_instance`): once any Studio event has been stopped, they mix a stereo sound as a
mono one, which sounds garbled. Studio events and mono sounds are not affected. The addon works around the bug, so
these sounds play correctly, until a fixed FMOD release.

[android-extension-export-enable]: ./assets/android-export-enable-extension.png
[install-android-build-template]: ./assets/install-android-build-template.png