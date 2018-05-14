#include "Bone.h"
#include "utils.h"


Bone::Bone(void)
{
}

Bone::Bone(float x, float y, float z):
	m_pSibling(NULL), m_pFirstChild(NULL), m_pFather(NULL)
{
    mLocalMatrix = glm::translate(x, y, z);
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
void Bone::ComputeWorldPos(glm::mat4 father)
{
    mWorldMatrix = glm::mul(father,mLocalMatrix) ;

//    print_mat(mWorldMatrix);

    if(m_pSibling != NULL)
        m_pSibling->ComputeWorldPos(father);

    if(m_pFirstChild != NULL)
        //   m_pFirstChild->ComputeWorldPos(m_wx, m_wy, m_wz);
        m_pFirstChild->ComputeWorldPos(mWorldMatrix);


}

//called after compute world pos when bone loaded but not animated
void Bone::ComputeBoneOffset()
{
    m_boneOffset.mOffsetMatrix = glm::inverse(mWorldMatrix);

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
