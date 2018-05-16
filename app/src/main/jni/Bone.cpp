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
#if 0
    mLocalTranslateMatrix = glm::translate(pos);
#else

    mLocalTranslateMatrix = glm::translate(glm::mat4(), pos);
#endif

}

void Bone::setRotation(float angle, float x, float y, float z)
{
    mLocalRotationMatrix = glm::rotate(angle, x, y, z);
}

void Bone::setScale(float x, float y, float z)
{
    mLocalScaleMatrix = glm::scale(x, y, z);
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
    mLocalModelMatrix = mLocalTranslateMatrix * mLocalScaleMatrix * mLocalRotationMatrix;

    return mLocalModelMatrix;
}



//called after compute world pos when bone loaded but not animated
void Bone::ComputeBoneOffset()
{

#if 0
    m_boneOffset.mOffsetMatrix = glm::inverse(mWorldModelMatrix);

#else
    //-----------------------------------------------------------
    m_boneOffset.mOffsetMatrix = mWorldModelMatrix;

    // transpose 重要！
    glm::mat3 R_Transpose;
    glm::vec3 pos;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            R_Transpose[i][j]=m_boneOffset.mOffsetMatrix[j][i];

    for(int i=0;i<3;i++)
        pos[i]=m_boneOffset.mOffsetMatrix[3][i];

    pos = -R_Transpose * pos;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            m_boneOffset.mOffsetMatrix[i][j] = R_Transpose[i][j];

    for(int i=0;i<3;i++)
    {
        m_boneOffset.mOffsetMatrix[3][i] = pos[i];
        m_boneOffset.mOffsetMatrix[i][3] = 0;
    }

    m_boneOffset.mOffsetMatrix[3][3] = 1;
    //-----------------------------------------------------------
#endif

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
