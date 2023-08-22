package com.utopiarise.godot.fmod.android.plugin

import org.fmod.FMOD
import org.godotengine.godot.Godot
import org.godotengine.godot.plugin.GodotPlugin

class FmodPlugin(godot: Godot) : GodotPlugin(godot) {
    override fun getPluginName() = "Godot Fmod Android Plugin"

    init {
        FMOD.init(activity)
    }

    override fun onMainDestroy() {
        FMOD.close()
    }

    companion object {
        init {
            System.loadLibrary("java")
        }
    }
}