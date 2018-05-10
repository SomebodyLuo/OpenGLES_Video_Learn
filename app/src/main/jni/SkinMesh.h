#pragma once
#include "ggl.h"
#include "Vertex.h"
#include "vertexbuffer.h"
#include "shader.h"

class SkinMesh
{
public:
	SkinMesh(void);
	SkinMesh(int vertexNum);

	~SkinMesh(void); 



    int m_vertexNum;

    Vertex* m_vertexs; //array of vertices in mesh

	VertexBuffer *mVertexBuffer;

	Shader *mShader;


	void Init(AAssetManager *assetManager, const char *modelPath);

	void UpdateVertices();

	void DrawStaticMesh(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos);
	void Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos);

	void SetAmbientMaterial(float r, float g, float b, float a);

	void SetDiffuseMaterial(float r, float g, float b, float a);

	void SetSpecularMaterial(float r, float g, float b, float a);


	Bone* g_boneRoot;
	Bone* g_bone1, *g_bone2, *g_bone31, *g_bone32;

    void Update(float deltaTime);
	void animateBones();
	int dir, dir2;
};

