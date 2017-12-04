//
// Created by pc on 2017/11/24.
//

#include "scene.h"
#include "ggl.h"
#include "utils.h"
#include "ground.h"
#include "shader.h"
#include "Model.h"
#include "SkyBox.h"

// 矩阵默认是单位矩阵
glm::mat4 tempMatrix, viewMatrix, projectionMatrix;

Ground ground;

Model sphere;

SkyBox skybox;

void InitGL(AAssetManager *assetManager)
{
    LOGI("------InitGL-------");
    if (nullptr == assetManager)
    {
        return;
    }

    // 棋盘格
    ground.Init(assetManager);
//    ground.SetPosition(-10.0f, -1.0f, -10.0f);
    LOGI("------InitGL2-------");
    // 球
    sphere.Init(assetManager, "Res/Sphere.obj");
    sphere.SetPosition(0.0f, 0.0f, -6.5f);
    sphere.SetTexure("Res/earth.bmp");

    // 天空盒
    skybox.Init(assetManager, "Res/");

}

void SetViewportSize(float width, float height)
{
    //------SetViewportSize-------width = 720.000000; height = 1132.000000
    LOGI("------SetViewportSize-------width = %f; height = %f", width, height);
    // 设置投影矩阵
    projectionMatrix = glm::perspective(60.0f, width / height, 0.1f, 1000.0f);
}

void DrawGL()
{
    float frameTime = GetFrameTime();

    LOGI("------DrawGL------- %f s", frameTime);
    // 擦除背景颜色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //设置颜色缓冲区和深度缓冲区
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skybox.Draw(viewMatrix, projectionMatrix);

    // 绘制棋盘格
    ground.Draw(viewMatrix, projectionMatrix);

    // 绘制地球
    sphere.Draw(viewMatrix, projectionMatrix);

}


