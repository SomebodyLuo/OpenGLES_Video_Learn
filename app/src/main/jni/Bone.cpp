#include "Bone.h"
#include "utils.h"


Bone::Bone(void):
        m_pSibling(NULL), m_pFirstChild(NULL), m_pFather(NULL)
{
}

void Bone::setPosition(float x, float y, float z)
{
    mLocalTranslateMatrix = glm::translate(x, y, z);
}

void Bone::setPosition(glm::vec3 pos)
{
    mLocalTranslateMatrix = glm::translate(pos);
}

void Bone::setRotation(float angle, float x, float y, float z)
{
    mLocalRotationMatrix = glm::rotate(angle, x, y, z);
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

void Bone::ComputeWorldModelMatrix(glm::mat4 fatherT, glm::mat4 fatherR)
{

}

//give father's world model matrix, compute the bone's model matrix
void Bone::ComputeWorldModelMatrix(glm::mat4 &fatherModelMatrix)
{
    mWorldModelMatrix = fatherModelMatrix * GetLocalModelMatrix();

    if(m_pSibling != NULL)
        m_pSibling->ComputeWorldModelMatrix(fatherModelMatrix);

    if(m_pFirstChild != NULL)
        m_pFirstChild->ComputeWorldModelMatrix(mWorldModelMatrix);
}

glm::mat4 &Bone::GetLocalModelMatrix()
{
    mLocalModelMatrix = mLocalTranslateMatrix * mLocalRotationMatrix;

    return mLocalModelMatrix;
}



//called after compute world pos when bone loaded but not animated
void Bone::ComputeBoneOffset()
{
    m_boneOffset.mOffsetMatrix = glm::inverse(mWorldModelMatrix);

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
