#pragma once
#include "ggl.h"
#include "Bone.h"
#include "SkinMesh.h"

class skeleton_animation
{
public:
	skeleton_animation(void);
	~skeleton_animation(void);

	SkinMesh mSkinMesh;

	void Init(AAssetManager *assetManager, const char *modelPath);
	void Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos, float frameTime);
};

