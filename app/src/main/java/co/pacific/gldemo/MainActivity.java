package co.pacific.gldemo;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


//每一个View都需要一个Render，GLSurfaceView也需要！
class MyGLRenderer implements GLSurfaceView.Renderer{

    //视口被创建时
    
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Native.InitOpenGL();    //替换成C++函数
    }

    //视口改变大小时，比如左右旋转屏幕时
    
    public void onSurfaceChanged(GL10 gl, int width, int height) {

        Native.OnViewportChanged(width, height);    //替换成C++函数
    }

    //刷新View
    
    public void onDrawFrame(GL10 gl) {
        Native.RenderOneFrame();    //替换成C++函数

    }
}

//新建一个GLSurfaceView类
class MyGLSurface extends GLSurfaceView {
    MyGLRenderer mRenderer;
    public MyGLSurface(Context context){
        super(context);

        setEGLContextClientVersion(2);  //使用OpenGL 2.0的渲染环境

        mRenderer = new MyGLRenderer();
        setRenderer(mRenderer);           //将渲染器Render关联给SurfaceView
    }

}


public class MainActivity extends AppCompatActivity {

    MyGLSurface mView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
        // 将资源读取器传入C++层
        Native.InitAssetManager(getAssets());

        mView = new MyGLSurface(getApplication());

        setContentView(mView);
    }
}
