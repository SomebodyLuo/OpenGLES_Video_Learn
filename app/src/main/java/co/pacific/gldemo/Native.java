package co.pacific.gldemo;

// C++层没有权限去访问Android安装包里面的内容，只有java层才能；
// 通过初始化的时候将AssetManager传入C++，这时C++就可以干任何事情了。
import android.content.res.AssetManager;

public class Native {
    static {
        // OpenGL C++代码放在mygles 库里面
        System.loadLibrary("mygles");
    }

    //------------------------------下面是JNI接口----------------------------

    // 把Java层的AssetManager传入C++
    public static native void InitAssetManager(AssetManager am);

    // 初始化OpenGL
    public static native void InitOpenGL();

    // 画布大小改变
    public static native void OnViewportChanged(float w, float h);

    // 渲染一帧，会调用C++层的渲染场景的函数
    public static native void RenderOneFrame();

}



