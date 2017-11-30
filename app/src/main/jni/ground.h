//
// Created by pc on 2017/11/30.
//

#ifndef GLDEMO_GROUND_H
#define GLDEMO_GROUND_H

#include "utils.h"
#include "vertexbuffer.h"

class Ground
{
    VertexBuffer *mVertexBuffer;
    GLuint mVBO;

public:
    void Init();

};


#endif //GLDEMO_GROUND_H
