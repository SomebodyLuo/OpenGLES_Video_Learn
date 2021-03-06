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

    mBox.Init(assetManager, "");
    mBox.SetPosition(0.0f, 0.0f, 0.0f);



#if 1
    mMan.Init(assetManager, "Res/human_body.obj");
//    man.mModelMatrix = glm::translate(3.0f, -8.0f, 2.5f) * glm::scale(0.5f, 0.5f, 0.5f) * glm::rotate(-20.0f, 0.0f, 1.0f, 0.0f);
    mMan.mModelMatrix = glm::translate(3.0f, -8.0f, 2.5f) * glm::scale(0.5f, 0.5f, 0.5f) * glm::rotate(-20.0f, 0.0f, 1.0f, 0.0f);
    mMan.ParseHumanBody();
#endif
}


void skeleton_animation::Draw(glm::mat4 &viewMatrix, glm::mat4 &projectionMatrix, glm::vec3 &cameraPos, float frameTime)
{
    //draw original mesh
    mSkinMesh.DrawStaticMesh(viewMatrix, projectionMatrix, cameraPos);

    //move bones
    mSkinMesh.animateBones();
    //update all bone's pos in bone tree
    mSkinMesh.ComputeWorldModelMatrix(glm::mat4());
    //update vertex pos by bones, using vertex blending
    mSkinMesh.UpdateMatrices();
    //draw deformed mesh
    mSkinMesh.Draw(viewMatrix, projectionMatrix, cameraPos);

#if 1
    mBox.animateBones();
    mBox.ComputeWorldModelMatrix(glm::mat4());
    mBox.UpdateMatrices();
    mBox.Draw(viewMatrix, projectionMatrix, cameraPos);
#endif


#if 1
    mMan.animateBones();
    mMan.ComputeWorldModelMatrix(glm::mat4());
    mMan.UpdateMatrices();
    mMan.Draw(viewMatrix, projectionMatrix, cameraPos);
#endif

#if 0
    //draw bone
    mSkinMesh.g_boneRoot->Draw();
#endif
}
