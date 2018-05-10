#include "Bone.h"


Bone::Bone(void)
{
    mModelMatrix = glm::mat4();
}

Bone::Bone(float x, float y, float z):
	m_pSibling(NULL), m_pFirstChild(NULL), m_pFather(NULL), m_x(x), m_y(y), m_z(z)
{
    mModelMatrix = glm::mat4();
}


Bone::~Bone(void)
{

}


void Bone::SetFirstChild(Bone* pChild) 
{ 
	m_pFirstChild = pChild; 
	m_pFirstChild->m_pFather = this; 
}

void Bone::SetSibling(Bone* pSibling) 
{ 
	m_pSibling = pSibling; 
	m_pSibling->m_pFather = m_pFather; 
} 


//give father's world pos, compute the bone's world pos
void Bone::ComputeWorldPos(float fatherWX, float fatherWY, float fatherWZ)
{
    glm::mat4 fatherMat = glm::translate(fatherWX, fatherWY, fatherWZ);
    mModelMatrix = mModelMatrix + fatherMat;

    m_wx = fatherWX + m_x;
    m_wy = fatherWY + m_y;
    m_wz = fatherWZ + m_z;

    if(m_pSibling != NULL)
        m_pSibling->ComputeWorldPos(fatherWX, fatherWY, fatherWZ);

    if(m_pFirstChild != NULL)
        m_pFirstChild->ComputeWorldPos(m_wx, m_wy, m_wz);

}


//called after compute world pos when bone loaded but not animated

void Bone::ComputeBoneOffset()
{

    m_boneOffset.m_offx = -m_wx;
    m_boneOffset.m_offy = -m_wy;
    m_boneOffset.m_offz = -m_wz;

    m_boneOffset.mOffsetMatrix = -mModelMatrix;

    if(m_pSibling!=NULL)

        m_pSibling->ComputeBoneOffset();

    if(m_pFirstChild!=NULL)

        m_pFirstChild->ComputeBoneOffset();
}

 

void Bone::Draw()
{

    if(m_pSibling != NULL)

        m_pSibling->Draw();

    if(m_pFirstChild != NULL)

        m_pFirstChild->Draw();
}
