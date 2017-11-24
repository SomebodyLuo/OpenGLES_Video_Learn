//
// Created by pc on 2017/11/24.
//

#include <jni.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

AAssetManager *sAssetManager = nullptr;

extern "C" JNIEXPORT void Java_co_pacific_gldemo_Native_InitAssetManager(JNIEnv *env, jobject obj, jobject assetManager)
{
    sAssetManager = AAssetManager_fromJava(env, assetManager);
}

extern "C" JNIEXPORT void Java_co_pacific_gldemo_Native_InitOpenGL(JNIEnv *env, jobject obj)
{
    glClearColor(0.8f, 0.4f, 0.6f, 1.0f);
}

extern "C" JNIEXPORT void Java_co_pacific_gldemo_Native_OnViewportChanged(JNIEnv *env, jobject obj, jfloat w, jfloat h)
{
    glViewport(0.0f, 0.0f, w, h);
}

extern "C" JNIEXPORT void Java_co_pacific_gldemo_Native_RenderOneFrame(JNIEnv *env, jobject obj)
{
    glClear(GL_COLOR_BUFFER_BIT);
}
