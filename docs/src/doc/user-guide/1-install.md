# Setup guide

## 1 - Install addon folder

We provide releases in github repository. You can download `addon.zip` from release tabs, unzip it, and copy its content
to `addons` folder of your project.

!!! warning
    It does not contains libraries from FMOD company.

## 2 - Add Fmod libraries to appropriate folder

[Download the FMOD Studio API](https://www.fmod.com/download) (You need to create an account), if you have not done it yet.
Then place fmod libraries (both `fmod` and `fmodstudio`) in the appropriate folder for each platform. A
`CopyPast_Fmod_Libs_Here.txt` file should be present where you need to copy libraries.  
Paths where you need to add shared libraries are:
- `res://addons/fmod/libs/android/arm64/`
- `res://addons/fmod/libs/macos/`
- `res://addons/fmod/libs/windows/`
- `res://addons/fmod/libs/linux/`
- `res://addons/fmod/libs/iOS/`

!!! info
    We cannot redistribute fmod libraries as it is not permitted by fmod license.

## 3 - Android specific

TODO