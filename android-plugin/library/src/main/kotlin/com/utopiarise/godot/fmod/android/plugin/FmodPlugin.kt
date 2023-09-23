package com.utopiarise.godot.fmod.android.plugin

import android.app.Activity
import android.view.View
import org.fmod.FMOD
import org.godotengine.godot.Godot
import org.godotengine.godot.plugin.GodotPlugin

class FmodPlugin(godot: Godot) : GodotPlugin(godot) {
    override fun getPluginName() = "Godot Fmod Android Plugin"

    override fun onMainCreate(activity: Activity?): View? {
        FMOD.init(activity)
        return super.onMainCreate(activity)
    }

    override fun onMainDestroy() {
        FMOD.close()
    }

    companion object {
        init {
            if (BuildConfig.DEBUG) {
                System.loadLibrary("fmodL")
                System.loadLibrary("fmodstudioL")
            } else {
                System.loadLibrary("fmod")
                System.loadLibrary("fmodstudio")
            }
        }
    }
}