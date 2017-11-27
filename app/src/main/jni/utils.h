//
// Created by pc on 2017/11/24.
//

#ifndef GLDEMO_UTILS_H
#define GLDEMO_UTILS_H

#include "ggl.h"

char *LoadFileContent(AAssetManager *assetManager, const char *path, int &filesize);

GLuint CompileShader(GLenum shaderType, const char *shaderCode);

// vs--Vertex Shader; fs--Fragment Shader
GLuint CreateProgram(GLuint vsShader, GLuint fsShader);

float GetFrameTime();

#endif //GLDEMO_UTILS_H
