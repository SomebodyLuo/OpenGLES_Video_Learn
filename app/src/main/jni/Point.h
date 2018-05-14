//
// Created by pc on 2018/5/15.
//

#include "ggl.h"
#include "vertexbuffer.h"
#include "shader.h"

#ifndef GLDEMO_POINT_H
#define GLDEMO_POINT_H


class Point {
public:
    Point();
    ~Point();

    int m_vertexNum;

    VertexBuffer *mVertexBuffer;

    glm::mat4 mModelMatrix;

    Shader *mShader;

    void Init(int pointCounts, AAssetManager *assetManager, const char *modelPath);
    void DrawStaticMesh(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos);

    void SetPointPosition(int index, float x, float y, float z);

};


#endif //GLDEMO_POINT_H
