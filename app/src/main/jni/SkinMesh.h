#pragma once
#include "ggl.h"
#include "vertexbuffer.h"
#include "shader.h"


class SkinMesh
{
public:
	SkinMesh(void);
	SkinMesh(int vertexNum);

	~SkinMesh(void); 

    int m_vertexNum;

	VertexBuffer *mVertexBuffer;

	Shader *mShader;

	Shader *mShaderDynamic;

	void Init(AAssetManager *assetManager, const char *modelPath);

    void ComputeWorldModelMatrix(glm::mat4 fatherModelMatrix);

	void retrieveBoneMatrices(Bone *pBone, VertexBuffer *vb);
	void UpdateMatrices();

	void DrawStaticMesh(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos);
	void Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos);

	void SetAmbientMaterial(float r, float g, float b, float a);

	void SetDiffuseMaterial(float r, float g, float b, float a);

	void SetSpecularMaterial(float r, float g, float b, float a);


	Bone* g_boneRoot;
	Bone* g_bone1, *g_bone2, *g_bone31, *g_bone32;

	void animateBones();
	int dir, dir2;
};

