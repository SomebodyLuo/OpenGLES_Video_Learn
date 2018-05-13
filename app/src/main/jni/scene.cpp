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
#include "ParticleSystem.h"
#include "skeleton_animation.h"

// 矩阵默认是单位矩阵
glm::mat4 tempMatrix, viewMatrix, projectionMatrix;

// 摄像机的位置
glm::vec3 cameraPos(0.5f, 0.0f, 2.0f);

Ground ground;

Model sphere, niutou;

SkyBox skybox;

ParticleSystem ps;

skeleton_animation skeleton;

void InitGL(AAssetManager *assetManager)
{
    LOGI("------InitGL-------");
    if (nullptr == assetManager)
    {
        return;
    }

    std::vector<std::string> joint_name;
    std::vector<glm::vec3> joint_pos;
    std::vector<glm::quat> joint_rot;
    std::vector<int> joint_parent_ID;
    ParseJsonFile(assetManager, "Res/test.json", joint_name, joint_pos, joint_rot, joint_parent_ID);

    viewMatrix = glm::lookAt(cameraPos, glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // 棋盘格
    ground.Init(assetManager);
    ground.SetPosition(0.0f, -30.0f, 0.0f);
    LOGI("------InitGL2-------");
    // 球
    sphere.Init(assetManager, "Res/Sphere.obj");
    sphere.SetPosition(0.0f, 0.0f, -2.0f);
    sphere.SetTexure("Res/earth.bmp");

    // 天空盒
    skybox.Init(assetManager, "Res/");

    // 牛头
    niutou.Init(assetManager, "Res/niutou.obj");
    niutou.SetTexure("Res/niutou.bmp");
    niutou.mModelMatrix = glm::translate(5.0f, 0.0f, 6.0f) * glm::scale(0.05f, 0.05f, 0.05f) * glm::rotate(-45.0f, 0.0f, 1.0f, 0.0f);

    // 粒子系统
    ps.Init(assetManager, 0.0f, 0.0f, 0.0f);



    // 骨骼动画
    skeleton.Init(assetManager, "");

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

    // 绘制天空盒——注意必须在最前面
//    skybox.Draw(viewMatrix, projectionMatrix, cameraPos);

    // 绘制棋盘格
    ground.Draw(viewMatrix, projectionMatrix);

    // 绘制地球
//    sphere.Update(frameTime);   //光照动画
//    sphere.Draw(viewMatrix, projectionMatrix, cameraPos);

    // 绘制牛头
//    niutou.Draw(viewMatrix, projectionMatrix, cameraPos);

    // 绘制粒子
//    ps.Update(frameTime);
//    ps.Draw(viewMatrix, projectionMatrix);

    // 骨骼动画
    skeleton.Draw(viewMatrix, projectionMatrix, cameraPos, frameTime);
}


