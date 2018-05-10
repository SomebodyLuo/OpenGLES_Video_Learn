#pragma once
#include "ggl.h"
#include "Bone.h"

class BoneInfo
{
public:
	//bone info
	int m_boneNum;
	BoneInfo();

	static const int MAX_BONE_PER_VERTEX = 4;
	Bone* m_bones[MAX_BONE_PER_VERTEX];
	float m_boneWeights[MAX_BONE_PER_VERTEX];

	void SetBoneAndWeight(int index, Bone* pBone, float weight);
};

