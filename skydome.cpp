#include "skydome.h"
#include "model.h"
#include "renderer.h"

static MODEL* g_SkyDomeModel = nullptr;
static XMVECTOR g_CenterPosition{};

void InitSkyDome()
{
    g_SkyDomeModel = ModelLoad("asset/model/sky.fbx", true);
}

void UpdateSkyDome()
{

}

void DrawSkyDome()
{
    SetShader2D();
    SetDepthEnable(false);

    SetUVMatrix(XMMatrixIdentity());
    XMMATRIX translation = XMMatrixTranslationFromVector(g_CenterPosition);
    
    ModelDraw(g_SkyDomeModel, translation);

    SetShader3D();
    SetDepthEnable(true);

}

void UnInitSkyDome()
{
    ModelRelease(g_SkyDomeModel);
}

void SetSkyDomeCenterLocation(const XMVECTOR& position)
{
    g_CenterPosition = position;
}
