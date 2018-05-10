#pragma once
#include "ggl.h"
#include "Bone.h"

class Vertex
{
public:
	Vertex(void);
	~Vertex(void);

    void ComputeWorldPosByBone(Bone* pBone, float& outX, float& outY, float& outZ); 

    void BlendVertex(); 

    float m_x, m_y, m_z;		//local pos in mesh space

    float m_wX, m_wY, m_wZ;	//blended vertex pos, in world space
	glm::mat4 mModelMatrix;

    //skin info
    int m_boneNum;

	static const int MAX_BONE_PER_VERTEX = 4;
    Bone* m_bones[MAX_BONE_PER_VERTEX];
    float m_boneWeights[MAX_BONE_PER_VERTEX]; 

    void SetBoneAndWeight(int index, Bone* pBone, float weight);
};

