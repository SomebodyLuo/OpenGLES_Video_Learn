package co.pacific.gldemo;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.opengl.GLSurfaceView;        //OpenGL专用View
import javax.microedition.khronos.egl.EGLConfig;    //OpenGL相关的包
import javax.microedition.khronos.opengles.GL10;    //OpenGL相关的包

//每一个View都需要一个Render，GLSurfaceView也需要！
class MyGLRender implements GLSurfaceView.Renderer{

    //视口被创建时
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        gl.glClearColor(0.8f, 0.4f, 0.6f, 1.0f);
    }

    //视口改变大小时，比如左右旋转屏幕时
    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

        gl.glViewport(0, 0, width, height); //修改视口映射
    }

    //刷新View
    @Override
    public void onDrawFrame(GL10 gl) {
        gl.glClear(gl.GL_COLOR_BUFFER_BIT);
    }
}

//新建一个GLSurfaceView类
class MyGLSurfaceView extends GLSurfaceView {
    public MyGLRender mRender;
    public MyGLSurfaceView(Context context){
        super(context);

        setEGLContextClientVersion(2);  //使用OpenGL 2.0的渲染环境

        mRender = new MyGLRender();
        setRenderer(mRender);           //将渲染器Render关联给SurfaceView
    }

}


public class MainActivity extends AppCompatActivity {

    MyGLSurfaceView mGLView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mGLView = new MyGLSurfaceView(getApplication());

        setContentView(mGLView);
    }
}
