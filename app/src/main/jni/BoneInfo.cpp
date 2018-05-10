#include "BoneInfo.h"

BoneInfo::BoneInfo()
{

}

void BoneInfo::SetBoneAndWeight(int index, Bone* pBone, float weight)
{

    m_bones[index] = pBone;

    m_boneWeights[index] = weight;
}
