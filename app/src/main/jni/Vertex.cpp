#include "Vertex.h"


Vertex::Vertex(void)
{
	m_boneNum = 0;
}


Vertex::~Vertex(void)
{

}
 

void Vertex::ComputeWorldPosByBone(Bone* pBone, float& outX, float& outY, float& outZ)
{


}
 

void Vertex::BlendVertex()
{
	//do the vertex blending,get the vertex's pos in world space
    m_wX = 0;
    m_wY = 0;
    m_wZ = 0; 

    for(int i=0; i<m_boneNum; ++i)
    {

        float tx, ty, tz;

        ComputeWorldPosByBone(m_bones[i], tx, ty, tz);

        tx *= m_boneWeights[i];

        ty *= m_boneWeights[i];

        tz *= m_boneWeights[i];

 

        m_wX += tx;

        m_wY += ty;

        m_wZ += tz;

    }

} 

void Vertex::SetBoneAndWeight(int index, Bone* pBone, float weight)
{

    m_bones[index] = pBone;

    m_boneWeights[index] = weight;    

}
