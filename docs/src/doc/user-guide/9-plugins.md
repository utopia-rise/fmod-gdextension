# Plugins

!!! warning
    This feature is experimental. Please report any bug you find.

This addon supports FMOD plugins.  
First you'll need to add plugins libraries (`.dll`, `.dylib`, `.so`, `.a`) in a project's folder.  
In this documentation we will use `assets/plugins` as an example.  
In this directory you have to create one directory per os, like:  
```
assets
└───── plugins
       ├────── windows
       │       └────── .dll for windows goes here
       ├────── linux
       │       └────── .so for linux goes here
       ├────── macos
       │       └────── .dylib for macos goes here
       ├────── android
       │       ├────── x86_64
       │       │       └────── .so for android x86 goes here
       │       └────── arm64
       │               └────── .so for android arm64 goes here
       └────── ios
               └────── .a for ios goes here
```  
If there are some dependent libraries needed for a plugin, just place it next to plugin's library, all libraries in 
those folders are exported.  

In order to add plugins you first need to create an `FmodPluginsSettings` resource in your project:  
![plugins-create-settings-resource]  
First you need to configure the directory containing the plugins libraries. In this example `res://assets/plugins/`
Then you have to configure the plugin lists.  
There is two lists to configure:  
- `Dynamic Plugin List` is used for all platforms except `iOS`
- `Static Plugins Methods` is used for `iOS` only

For dynamic plugins you have to enter the name of the library for the plugin (without the lib prefix). As an example for 
steam plugin it will be `phonon_fmod`.  
In static plugins methods you have to place the methods from the documentation of your plugin. As and example for steam 
plugin it will be:  
- `FMOD_SteamAudio_Spatialize_GetDSPDescription`, with DSP type
- `FMOD_SteamAudio_MixerReturn_GetDSPDescription`, with DSP type
- `FMOD_SteamAudio_Reverb_GetDSPDescription`, with DSP type

Here is what the configuration looks like for this example:  
![fmod-plugins-settings-resource]

Then you have to set the path of this resource in project's settings, in the `Fmod/Plugins` section:  
![plugins-project-settings]


[plugins-create-settings-resource]: ./assets/plugins-create-settings-resource.png
[fmod-plugins-settings-resource]: ./assets/fmod-plugins-settings-resource.png
[plugins-project-settings]: ./assets/plugins-project-settings.png