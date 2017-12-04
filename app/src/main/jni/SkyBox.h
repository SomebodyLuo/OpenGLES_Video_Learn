//
// Created by pc on 2017/12/4.
//

#ifndef GLDEMO_SKYBOX_H
#define GLDEMO_SKYBOX_H

#include "vertexbuffer.h"
#include "shader.h"


class SkyBox {
    AAssetManager *mAssetManager;
    const char *mImageDir;

    VertexBuffer *mVertexBuffer;

    Shader *mShader;

    glm::mat4 mModelMatrix;

public:
    void Init(AAssetManager *assetManager, const char *imageDir);

    void InitFront();
    void InitBack();
    void InitLeft();
    void InitRight();
    void InitTop();
    void InitBottom();

    void Draw(glm::mat4 &V, glm::mat4 &P);

};


#endif //GLDEMO_SKYBOX_H
