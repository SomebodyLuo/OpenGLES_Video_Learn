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

unsigned char*DecodeBMP(unsigned char *bmpFileData, int &width, int &height);

GLuint CreateTexture2D(unsigned char*pixelData, int width, int height, GLenum type);

GLuint CreateTexture2DFromBMP(AAssetManager *assetManager, const char*bmpPath);

GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void *data /*= nullptr*/);

GLuint CreateProcedureTexture(int size);

void print_mat(glm::mat4 &mat);
void print_array(float array[16]);

#endif //GLDEMO_UTILS_H
