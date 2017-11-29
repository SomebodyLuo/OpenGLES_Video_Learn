package co.pacific.gldemo;

import android.content.res.AssetManager;

public class Native {
    static {
        System.loadLibrary("mygles");
    }

    public static native void InitAssetManager(AssetManager am);

    public static native void InitOpenGL();

    public static native void OnViewportChanged(float width,float height);

    public static native void RenderOneFrame();

}



