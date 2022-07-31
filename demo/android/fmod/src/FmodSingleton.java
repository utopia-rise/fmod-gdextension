package org.godotengine.godot;

import android.app.Activity;
import android.content.Intent;
import android.content.Context;
import com.godot.game.R;
import javax.microedition.khronos.opengles.GL10;
import org.fmod.FMOD;



public class FmodSingleton extends Godot.SingletonBase {

    protected Activity appActivity;
    protected Context appContext;

    static
    {
        System.loadLibrary("fmod");
    }

    static public Godot.SingletonBase initialize(Activity p_activity) {
        return new FmodSingleton(p_activity);
    }

    public FmodSingleton(Activity p_activity) {
        this.appActivity = p_activity;
        this.appContext = appActivity.getApplicationContext();
        // You might want to try initializing your singleton here, but android
        // threads are weird and this runs in another thread, so to interact with Godot you usually have to do.
        FMOD.init(p_activity);
    }

    // Forwarded callbacks you can reimplement, as SDKs often need them.
    protected void onMainActivityResult(int requestCode, int resultCode, Intent data) {}
    protected void onMainRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {}

    protected void onMainPause() {}
    protected void onMainResume() {}
    protected void onMainDestroy() {
        FMOD.close();
    }

    protected void onGLDrawFrame(GL10 gl) {}
    protected void onGLSurfaceChanged(GL10 gl, int width, int height) {} // Singletons will always miss first 'onGLSurfaceChanged' call.

}