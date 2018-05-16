#pragma once
#include "ggl.h"

class BoneOffset
{

public:

    //BoneOffset transform a vertex from mesh space to bone space.

    //In other words, it is the offset from mesh space to a bone's space.

    //For each bone, there is a BoneOffest.

    //If we add the offset to the vertex's pos (in mesh space), we get the vertex's pos in bone space

    //For example: if a vertex's pos in mesh space is (100,0,0), the bone offset is (-20,0,0), so the vertex's pos in bone space is (80,0,0)

    //Actually, BoneOffset is the invert transform of that we place a bone in mesh space, that is (-20,0,0) means the bone is at (20,0,0) in mesh space

    glm::mat4 mOffsetMatrix;

};

class Bone
{
public:
	Bone();
    void setPosition(float x, float y, float z);
    void setPosition(glm::vec3 pos);
    void setRotation(float angle, float x, float y, float z);
    void setScale(float x, float y, float z);

    ~Bone(); 

    int mBoneIndex;
    Bone* m_pSibling;
    Bone* m_pFirstChild;
    Bone* m_pFather; //only for draw bone 

    void SetFirstChild(Bone* pChild);
    void SetSibling(Bone* pSibling); 

    glm::mat4 mLocalTranslateMatrix;
//    glm::mat4 mWorldTranslateMatrix;

    glm::quat mRotation;
    glm::mat4 mLocalRotationMatrix;
//    glm::mat4 mWorldRotationMatrix;

    glm::mat4 mLocalScaleMatrix;

    glm::mat4 mLocalModelMatrix;
    glm::mat4 mWorldModelMatrix;

    //give father's world model matrix, compute the bone's model matrix
    void ComputeWorldModelMatrix(glm::mat4 fatherT, glm::mat4 fatherR);
    void ComputeWorldModelMatrix(glm::mat4 &fatherModelMatrix);
    glm::mat4 &GetLocalModelMatrix();

    BoneOffset m_boneOffset;

    //called after compute world pos when bone loaded but not animated

    void ComputeBoneOffset(); 

    void Draw();
};

