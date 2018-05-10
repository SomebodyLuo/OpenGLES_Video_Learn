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
    Bone(float x, float y, float z); 

    ~Bone(); 

    Bone* m_pSibling;
    Bone* m_pFirstChild;
    Bone* m_pFather; //only for draw bone 

    void SetFirstChild(Bone* pChild);
    void SetSibling(Bone* pSibling); 

    float m_wx, m_wy, m_wz; //pos in world space
    glm::mat4 mLocalMatrix;
    glm::mat4 mWorldMatrix;

    //give father's world pos, compute the bone's world pos
    void ComputeWorldPos(glm::mat4 mat4);

    BoneOffset m_boneOffset;   

    //called after compute world pos when bone loaded but not animated

    void ComputeBoneOffset(); 

    void Draw();
};

