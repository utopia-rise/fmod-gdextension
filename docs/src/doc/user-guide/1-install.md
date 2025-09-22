# Setup guide

## 1 - Install addon folder

We provide releases in github repository. 

You can download `addon.zip` from this [page](https://github.com/utopia-rise/fmod-gdextension/releases), then unzip it 
and copy its content to the `addons` directory of your Godot project.

## 2 - Activate the plugin in Godot

Open your Godot project, go to your project settings and select the `Plugins` tab.
You should see the line `FMOD GDExtension` and a checkbox. Enable it.
After this, you should be able to create Fmod nodes, use the bank explorer and write scripts using `FmodServer`

## 3 - Android specific

Android exports require a Godot Android plugin.  

Download android build template from project menu:  
![install-android-build-template]  

Make sure the `Use Gradle Build` option is activated in your Android export:  
![android-extension-export-enable]

!!! warning
    We curently only support armv8 architecture for android export.  
    In order to get support for x86, you should build plugin on your own. 

[android-extension-export-enable]: ./assets/android-export-enable-extension.png
[install-android-build-template]: ./assets/install-android-build-template.png