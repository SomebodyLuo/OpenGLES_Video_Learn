//
// Created by pc on 2018/5/15.
//

#include "Model.h"
#include "Point.h"

#ifndef GLDEMO_HUMAN_H
#define GLDEMO_HUMAN_H


class Human: public Model{
public:
    Human();

    glm::vec3 mtopPoint, mbottomPoint, mLeftPoint, mRightPoint, mJawPoint;
    float mHeight, mWidth;
    // 男性头身比是7，女性头身比是6
    const float mHeadPercentage = 1.0f / 7.0f;

    Point mKeyPoints;

    void Init(AAssetManager *assetManager, const char *modelPath);
    void ParseHumanBody();
    void Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos);

};


#endif //GLDEMO_HUMAN_H
