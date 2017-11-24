//
// Created by pc on 2017/11/24.
//

#include "ggl.h"
#include "scene.h"

AAssetManager *sAssetManager = nullptr;

extern "C" JNIEXPORT void Java_co_pacific_gldemo_Native_InitAssetManager(JNIEnv *env, jobject obj, jobject assetManager)
{
    sAssetManager = AAssetManager_fromJava(env, assetManager);
}

extern "C" JNIEXPORT void Java_co_pacific_gldemo_Native_InitOpenGL(JNIEnv *env, jobject obj)
{
//    glClearColor(0.8f, 0.4f, 0.6f, 1.0f);
    InitGL();
}

extern "C" JNIEXPORT void Java_co_pacific_gldemo_Native_OnViewportChanged(JNIEnv *env, jobject obj, jfloat w, jfloat h)
{
//    glViewport(0.0f, 0.0f, w, h);
    SetViewportSize(w, h);
}

extern "C" JNIEXPORT void Java_co_pacific_gldemo_Native_RenderOneFrame(JNIEnv *env, jobject obj)
{
//    glClear(GL_COLOR_BUFFER_BIT);
    DrawGL();
}
