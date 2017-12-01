//
// Created by pc on 2017/12/1.
//

#ifndef GLDEMO_MODEL_H
#define GLDEMO_MODEL_H


#include "ggl.h"
#include "vertexbuffer.h"

class Model {

public:

    VertexBuffer *mVertexBuffer;

    Model();
    void Init(AAssetManager *assetManager, const char *modelPath);

};


#endif //GLDEMO_MODEL_H
