#include "skeleton_animation.h"


skeleton_animation::skeleton_animation(void)
{

}


skeleton_animation::~skeleton_animation(void)
{

}

void skeleton_animation::Init(AAssetManager *assetManager, const char *modelPath)
{
    mSkinMesh.Init(assetManager, modelPath);
}


void skeleton_animation::Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos, float frameTime)
{
    //draw original mesh
    mSkinMesh.DrawStaticMesh(viewMatrix, projectionMatrix, cameraPos);

    //move bones
    mSkinMesh.animateBones();
 //   mSkinMesh.Update(frameTime);

    //update all bone's pos in bone tree
    mSkinMesh.g_boneRoot->ComputeWorldPos(glm::mat4());

    //update vertex pos by bones, using vertex blending
    mSkinMesh.UpdateVertices();

    //draw deformed mesh
    mSkinMesh.Draw(viewMatrix, projectionMatrix, cameraPos);
#if 0
    //draw bone
    mSkinMesh.g_boneRoot->Draw();
#endif
}
