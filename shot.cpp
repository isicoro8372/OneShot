#include "shot.h"
#include "keylogger.hpp"

static constexpr XMVECTOR DEFAULT_DIRECTION = { 0.0f, 0.0f, 1.0f };
static Ball* g_Ball = nullptr;
static MODEL* g_DirectionArrow = nullptr;
static int g_DirectionArrowTexture;
static XMVECTOR g_Velocity = { 0.0f, 0.0f, 0.0f };
static XMVECTOR g_Direction = DEFAULT_DIRECTION;
static XMMATRIX g_Matrix = XMMatrixIdentity();

void InitShot()
{
    g_DirectionArrow = ModelLoad("asset/model/Arrow.fbx");
    g_DirectionArrowTexture = LoadTexture(L"asset/texture/blank.png");
}

void UpdateShot()
{
    XMFLOAT4X4 matrix;
    XMStoreFloat4x4(&matrix, g_Matrix);
    matrix._41 = matrix._42 = matrix._43 = 0.0f; //平行移動成分をカット
    g_Matrix = XMLoadFloat4x4(&matrix);

    g_Direction = XMVector3TransformNormal(DEFAULT_DIRECTION, g_Matrix);
    g_Velocity = XMVector3Normalize(g_Direction) * 0.2f;
}

void DrawShot()
{
    XMMATRIX translation = XMMatrixTranslationFromVector(g_Ball->GetPosition());
    XMMATRIX world = XMMatrixTranslation(0.0f, 0.0f, 0.35f) * g_Matrix * translation;
    SetLight(world);
    SetTexture(g_DirectionArrowTexture);
    ModelDraw(g_DirectionArrow, world);
}

void UninitShot()
{
    if (g_DirectionArrow)
    {
        ::ModelRelease(g_DirectionArrow);
        g_DirectionArrow = nullptr;
    }
}

/*-------------------------------------------------------------------------------*/

void SetBallObject(Ball* ball)
{
    g_Ball = ball;
}

void SetShotMatrix(const XMMATRIX& matrix)
{
    g_Matrix = matrix;
}

XMVECTOR GetShotVelocity(float chargePercentage)
{
    return g_Velocity * chargePercentage;
}
