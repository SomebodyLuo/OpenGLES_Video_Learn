//
// Created by pc on 2017/12/4.
//

#include "SkyBox.h"

void SkyBox::Init(AAssetManager *assetManager, const char *imageDir){
    if ((nullptr == imageDir) || (nullptr == assetManager))
    {
        return;
    }

    mAssetManager = assetManager;
    mImageDir = imageDir;

    mVertexBuffer = new VertexBuffer[6];
    mShader = new Shader[6];

    InitFront();
    InitBack();
    InitLeft();
    InitRight();
    InitTop();
    InitBottom();
}

void SkyBox::InitFront(){
    mVertexBuffer[0].SetSize(4);

    // 顶点坐标、纹理坐标
    mVertexBuffer[0].SetPosition(0, -0.5f, -0.5f, -0.5f);
    mVertexBuffer[0].SetTexcoord(0, 0.0f, 0.0f);

    mVertexBuffer[0].SetPosition(1, 0.5f, -0.5f, -0.5f);
    mVertexBuffer[0].SetTexcoord(1, 1.0f, 0.0f);

    mVertexBuffer[0].SetPosition(2, -0.5f, 0.5f, -0.5f);
    mVertexBuffer[0].SetTexcoord(2, 0.0f, 1.0f);

    mVertexBuffer[0].SetPosition(3, 0.5f, 0.5f, -0.5f);
    mVertexBuffer[0].SetTexcoord(3, 1.0f, 1.0f);

    // Shader
    mShader[0].Init(mAssetManager, "Res/skybox.vs", "Res/skybox.fs");
    char temp[256];
    memset(temp, 0, 256);
    strcpy(temp, mImageDir);
    strcat(temp, "front.bmp");
    mShader[0].SetTexture("U_Texture", temp);
}

void SkyBox::InitBack(){
    mVertexBuffer[1].SetSize(4);

    mVertexBuffer[1].SetPosition(0, 0.5f, -0.5f, 0.5f);
    mVertexBuffer[1].SetTexcoord(0, 0.0f, 0.0f);

    mVertexBuffer[1].SetPosition(1, -0.5f, -0.5f, 0.5f);
    mVertexBuffer[1].SetTexcoord(1, 1.0f, 0.0f);

    mVertexBuffer[1].SetPosition(2, 0.5f, 0.5f, 0.5f);
    mVertexBuffer[1].SetTexcoord(2, 0.0f, 1.0f);

    mVertexBuffer[1].SetPosition(3, -0.5f, 0.5f, 0.5f);
    mVertexBuffer[1].SetTexcoord(3, 1.0f, 1.0f);

    // Shader
    mShader[1].Init(mAssetManager, "Res/skybox.vs", "Res/skybox.fs");
    char temp[256];
    memset(temp, 0, 256);
    strcpy(temp, mImageDir);
    strcat(temp, "back.bmp");
    mShader[1].SetTexture("U_Texture", temp);
}

void SkyBox::InitLeft(){
    mVertexBuffer[2].SetSize(4);

    mVertexBuffer[2].SetPosition(0, -0.5f, -0.5f, 0.5f);
    mVertexBuffer[2].SetTexcoord(0, 0.0f, 0.0f);

    mVertexBuffer[2].SetPosition(1, -0.5f, -0.5f, -0.5f);
    mVertexBuffer[2].SetTexcoord(1, 1.0f, 0.0f);

    mVertexBuffer[2].SetPosition(2, -0.5f, 0.5f, 0.5f);
    mVertexBuffer[2].SetTexcoord(2, 0.0f, 1.0f);

    mVertexBuffer[2].SetPosition(3, -0.5f, 0.5f, -0.5f);
    mVertexBuffer[2].SetTexcoord(3, 1.0f, 1.0f);

    // Shader
    mShader[2].Init(mAssetManager, "Res/skybox.vs", "Res/skybox.fs");
    char temp[256];
    memset(temp, 0, 256);
    strcpy(temp, mImageDir);
    strcat(temp, "left.bmp");
    mShader[2].SetTexture("U_Texture", temp);
}

void SkyBox::InitRight(){
    mVertexBuffer[3].SetSize(4);

    mVertexBuffer[3].SetPosition(0, 0.5f, -0.5f, -0.5f);
    mVertexBuffer[3].SetTexcoord(0, 0.0f, 0.0f);

    mVertexBuffer[3].SetPosition(1, 0.5f, -0.5f, 0.5f);
    mVertexBuffer[3].SetTexcoord(1, 1.0f, 0.0f);

    mVertexBuffer[3].SetPosition(2, 0.5f, 0.5f, -0.5f);
    mVertexBuffer[3].SetTexcoord(2, 0.0f, 1.0f);

    mVertexBuffer[3].SetPosition(3, 0.5f, 0.5f, 0.5f);
    mVertexBuffer[3].SetTexcoord(3, 1.0f, 1.0f);

    // Shader
    mShader[3].Init(mAssetManager, "Res/skybox.vs", "Res/skybox.fs");
    char temp[256];
    memset(temp, 0, 256);
    strcpy(temp, mImageDir);
    strcat(temp, "right.bmp");
    mShader[3].SetTexture("U_Texture", temp);
}

void SkyBox::InitTop(){
    mVertexBuffer[4].SetSize(4);

    mVertexBuffer[4].SetPosition(0, -0.5f, 0.5f, -0.5f);
    mVertexBuffer[4].SetTexcoord(0, 0.0f, 0.0f);

    mVertexBuffer[4].SetPosition(1, 0.5f, 0.5f, -0.5f);
    mVertexBuffer[4].SetTexcoord(1, 1.0f, 0.0f);

    mVertexBuffer[4].SetPosition(2, -0.5f, 0.5f, 0.5f);
    mVertexBuffer[4].SetTexcoord(2, 0.0f, 1.0f);

    mVertexBuffer[4].SetPosition(3, 0.5f, 0.5f, 0.5f);
    mVertexBuffer[4].SetTexcoord(3, 1.0f, 1.0f);

    // Shader
    mShader[4].Init(mAssetManager, "Res/skybox.vs", "Res/skybox.fs");
    char temp[256];
    memset(temp, 0, 256);
    strcpy(temp, mImageDir);
    strcat(temp, "top.bmp");
    mShader[4].SetTexture("U_Texture", temp);
}

void SkyBox::InitBottom(){
    mVertexBuffer[5].SetSize(4);

    mVertexBuffer[5].SetPosition(0, -0.5f, -0.5f, 0.5f);
    mVertexBuffer[5].SetTexcoord(0, 0.0f, 0.0f);

    mVertexBuffer[5].SetPosition(1, 0.5f, -0.5f, 0.5f);
    mVertexBuffer[5].SetTexcoord(1, 1.0f, 0.0f);

    mVertexBuffer[5].SetPosition(2, -0.5f, -0.5f, -0.5f);
    mVertexBuffer[5].SetTexcoord(2, 0.0f, 1.0f);

    mVertexBuffer[5].SetPosition(3, 0.5f, -0.5f, -0.5f);
    mVertexBuffer[5].SetTexcoord(3, 1.0f, 1.0f);

    // Shader
    mShader[5].Init(mAssetManager, "Res/skybox.vs", "Res/skybox.fs");
    char temp[256];
    memset(temp, 0, 256);
    strcpy(temp, mImageDir);
    strcat(temp, "bottom.bmp");
    mShader[5].SetTexture("U_Texture", temp);
}

void SkyBox::Draw(glm::mat4 &V, glm::mat4 &P, glm::vec3 &cameraPos)
{
    glDisable(GL_DEPTH_TEST);   //天空盒需要禁止深度测试

    // 让天空盒跟随camera移动
    mModelMatrix = glm::translate(cameraPos);

    for (int i = 0; i < 6; ++i) {
        mVertexBuffer[i].Bind();

        mShader[i].BindMVP(glm::value_ptr(mModelMatrix), glm::value_ptr(V), glm::value_ptr(P));
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        mVertexBuffer[i].Unbind();
    }
}
