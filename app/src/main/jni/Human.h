//
// Created by pc on 2018/5/15.
//

#include "Model.h"
#include "Point.h"
#include "Bone.h"


#ifndef GLDEMO_HUMAN_H
#define GLDEMO_HUMAN_H


class Human: public Model{
public:
    Human();
    ~Human();

    glm::vec3 mtopPoint, mbottomPoint, mLeftPoint, mRightPoint, mNosePoint, mJawPoint;
    float mHeight, mWidth;
    // 男性头身比是7，女性头身比是6
    const float mHeadPercentage = 1.0f / 7.0f;

    Bone *mBoneRoot;

    Point mKeyPoints;

    void Init(AAssetManager *assetManager, const char *modelPath);
    void ParseHumanBody();
    bool RightOfLine(float targetPointX, float targetPointY, float linePoint1X, float linePoint1Y, float linePoint2X, float linePoint2Y);
    void Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos);
    void ComputeWorldModelMatrix(glm::mat4 fatherModelMatrix);

    void retrieveBoneMatrices(Bone *pBone, VertexBuffer *vb);

    void UpdateMatrices();

    void animateBones();

private:
    float time1 = 0.0f;
    float angle = 0.0f;
    float df = 0.001f;

};


#endif //GLDEMO_HUMAN_H
