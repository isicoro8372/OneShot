#define NOMINMAX

#include <time.h>

#include "main.h"
#include "scenemanager.h"
#include "Audio.h"
#include "keylogger.hpp"
#include "renderer.h"
#include "texture.h"
#include "sprite.h"
#include "game.h"
#include "light.h"
#include "collision.h"
#include "billboard.h"

#include "cube.hpp"
#include "model.h"

#include "camera.h"
#include "camera_move.h"
#include "camera_tracking.h"
#include "camera_pov.h"

#include "background.h"
#include "fade.hpp"

#include "ball.h"
#include "shot.h"
#include "trail.h"

#include "skydome.h"

#include <string>

//マップデータ
#include "mapdata.h"

constexpr int UI_GAUGE_SIZE_X = 40;
constexpr int UI_GAUGE_SIZE_Y = 300;

static int g_GaugeCount = 0;
static float g_ChargePercentage = 0.0f;

static int g_TileTextureID[TILE_ID_MAX];
static int g_BillBoardTextureID;

static int g_TimerTextureID;
static int g_ShotGaugeTextureID;

static int g_TextureID = -1;
static XMFLOAT3 position = {};
static XMFLOAT3 rotate = {};
static XMFLOAT3 size = { 1.0f, 1.0f, 1.0f };

static float g_Offset = 0.0f;
static float g_Angle = 0.0f;

static Camera* g_Camera[DEFAULT_CAMERA_MAX];
static int g_CameraID = 0;

static MODEL* g_BallModel = nullptr;

static Ball* g_Ball = nullptr;
static XMVECTOR g_CubePosition = { 0.0f, 2.5f, 10.0f };

static XMFLOAT3 g_ShootDirection = { 0.0f, 2.5f, 10.0f };
static XMFLOAT3 g_LastShotPosition = {};
static float g_Xangle, g_Yangle = 0.0f;

static Map* g_Map;

static unsigned int g_bgmID;
static int g_TitleTexture;

static BALLSTATE g_BallState;
static unsigned int g_FrameCounter;

void InitTitle()
{
	g_TitleTexture = LoadTexture(L"asset/texture/Title.png");
	SetFade(60, XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FADE_IN);
	
    //g_bgmID = LoadAudio("asset\\Audio\\title.wav");
    //PlayAudio(g_bgmID, true);

    InitCube();
    InitBillBoard();
    InitShot();
    TrailInitialize();
    InitSkyDome();

    srand((unsigned)time(NULL));

    switch (rand() % 3)
    {
    case 0:
        g_Map = new Map1();
        break;
    case 1:
        g_Map = new Map2();
        break;
    case 2:
        g_Map = new Map3();
        break;
    }
    //ボール関係
    g_Ball = new Ball({ 5.0f, 5.0f, 1.0f });
    g_Xangle = XMConvertToRadians(-15.0f);
    g_BallState = BALLSTATE_MOVE;
    g_FrameCounter = 0;
    ResetTrailPosition(g_Ball->GetPosition());

    g_TimerTextureID = LoadTexture(L"asset/texture/blank.png");
    g_ShotGaugeTextureID = LoadTexture(L"asset/texture/blank.png");

    for (int i = 0; i < TILE_ID_MAX; i++)
    {
        switch (i)
        {
        case 10:
            g_TileTextureID[i] = LoadTexture(L"asset/texture/blank.png");
        default:
            g_TileTextureID[i] = LoadTexture(L"asset/texture/ground.png");
            break;
        }
    }

    //カメラ初期化
    g_Camera[0] = new PovCamera;
    for (int i = 0; i < g_Map->GetMapCameraCount(); i++)
    {
        //ステージカメラの生成と初期化
        g_Camera[i + 1] = new TrackingCamera;
        static_cast<TrackingCamera*>(g_Camera[i + 1])->SetTarget(g_Ball);
        XMVECTOR cameraPosition = XMLoadFloat3(&g_Map->GetMapCameraPositions()[i]);
        static_cast<TrackingCamera*>(g_Camera[i + 1])->SetPosition(cameraPosition);
    }

    //デバッグ用カメラ
    g_Camera[DEFAULT_CAMERA_MAX - 1] = new MovingCamera;

    g_BallModel = ModelLoad("asset/model/ball.fbx");

    g_Ball->LoadModel();

    SetDirectionalLight(GetWorldLightVector(), GetLocalLightVector());
    SetAmbient({ 0.5f, 0.5f, 0.5f, 0.0f });

    //ボールの登録
    SetBallObject(g_Ball);
    SetShotMatrix(XMMatrixInverse(nullptr, g_Camera[2]->GetViewMatrix()));
}
void UninitTitle()
{
	//StopAudio(g_bgmID);
	//UnloadAudio(g_bgmID);

    g_Ball->ModelRelease();

    UninitCube();
    UninitBillBoard();
    UninitShot();
    TrailFinalize();
    UnInitSkyDome();

    for (int i = 0; i < DEFAULT_CAMERA_MAX - 1; i++)
    {
        delete g_Camera[i];
        g_Camera[i] = nullptr;
    }

    delete g_BallModel;
    g_BallModel = nullptr;

    XMFLOAT3 position = {};
    XMFLOAT3 rotate = {};
    XMFLOAT3 size = { 1.0f, 1.0f, 1.0f };

    float g_Offset = 0.0f;
    float g_Angle = 0.0f;

    g_CameraID = 0;

    delete g_Ball;
    Ball* g_Ball = nullptr;

    XMVECTOR g_CubePosition = { 0.0f, 2.5f, 10.0f };

    XMFLOAT3 g_ShootDirection = { 0.0f, 2.5f, 10.0f };
    XMFLOAT3 g_LastShotPosition = {};

}
void UpdateTitle()
{
	if (GetKeyTrigger(VK_SPACE) && GetFadeState() == FADE_NONE)
	{
		SetFade(30, XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), FADE_OUT);
		SetScene(SCENE_GAME);
        return;
	}

    SetShotMatrix(XMMatrixRotationX(g_Xangle) * XMMatrixRotationY(g_Yangle));
    UpdateShot();

    g_ChargePercentage = g_GaugeCount % SHOT_POWER_MAX / 100.0f;

    for (int i = '0'; i <= '0' + DEFAULT_CAMERA_MAX; i++)
    {
        if (GetKeyTrigger(i))
        {
            g_CameraID = i - '0';
        }
    }

    switch (g_BallState)
    {
    case BALLSTATE_WAIT:
        g_FrameCounter++;
        if (g_FrameCounter > 60)
        {
            g_GaugeCount = 0;
            g_BallState = BALLSTATE_SHOT;
        }
        break;

    case BALLSTATE_SHOT:

        g_GaugeCount++;

        //if (GetKeyTrigger(VK_SPACE)) //トリガーを時間に
        //{
        //    XMStoreFloat3(&g_LastShotPosition, g_Ball->GetPosition());
        //    g_Ball->AddForce(GetShotVelocity(g_ChargePercentage));
        //    g_FrameCounter = 0;
        //    g_BallState = BALLSTATE_SHOTENDSTEP;
        //}
        break;

    case BALLSTATE_SHOTENDSTEP:
        g_FrameCounter++;
        if (g_FrameCounter > 30)
        {
            g_GaugeCount = 0;
            g_BallState = BALLSTATE_MOVE;
        }
        break;

    case BALLSTATE_MOVE:

        g_Ball->Update();

        //フィールドカメラ
        if (g_CameraID != DEFAULT_CAMERA_MAX - 1)
        {
            float minLength = 1000.0f;
            for (int i = 0; i < g_Map->GetMapCameraCount(); i++)
            {
                float cameraToBallLength = XMVectorGetX(XMVector3Length(g_Ball->GetPosition() - g_Camera[i + 1]->GetPosition()));
                if (minLength > cameraToBallLength)
                {
                    minLength = cameraToBallLength;
                    g_CameraID = i + 1;
                }
            }
        }

        //落下の検知
        XMFLOAT3 pos;
        XMStoreFloat3(&pos, g_Ball->GetPosition());
        if (pos.y < -2.0f)
        {
            g_Ball->SetPosition(XMLoadFloat3(&g_LastShotPosition));
            g_Ball->StopMove();
        }

        //当たり判定
        {
            bool isHit = g_Map->GenerateCollider(*g_Ball);

            //地面に当たった
            if (g_Ball->IsStopped(isHit))
            {
                g_BallState = BALLSTATE_WAIT;
                g_FrameCounter = 0;
            }

            //ゴールに触れた
            if (isHit && g_Map->GetTileType() == 10)
            {
                SetScene(SCENE_GAME);
                return;
            }
        }
        break;
    }

    SetTrailPosition(g_Ball->GetPosition());

    //カメラの更新
    g_Camera[g_CameraID]->Update();
    SetTrailTargetCamera(g_Camera[g_CameraID]->GetPosition());

}
void DrawTitle()
{
    SetSkyDomeCenterLocation(g_Camera[g_CameraID]->GetPosition());

    SetShader3D();
    SetDepthEnable(true);

    SetViewMatrix(g_Camera[g_CameraID]->GetViewMatrix());
    SetProjectionMatrix(g_Camera[g_CameraID]->GetPerspectiveMatrix());

    XMMATRIX worldMatrix = XMMatrixTranslation(0.0f, 5.0f, 0.0f);
    SetWorldMatrix(worldMatrix);

    DrawSkyDome();

    //ModelDraw(g_Model3, XMMatrixTranslation(-5.5f, 3.5f, 5.0f), L"asset/texture/models/tv_tex.png");

    g_Map->Draw(g_TileTextureID);

    g_Ball->Draw();

    if (g_BallState == BALLSTATE_SHOT || g_BallState == BALLSTATE_SHOTENDSTEP)
    {
        DrawShot();
    }

    TrailDraw(g_Camera[g_CameraID]->GetPosition());

    XMFLOAT3 cubepos;
    XMStoreFloat3(&cubepos, g_CubePosition);
    XMMATRIX cubeMatrix = XMMatrixTranslation(cubepos.x, cubepos.y, cubepos.z);

    SetShader2D();

    SetDepthEnable(false);

    SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    XMFLOAT3 position = XMFLOAT3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
    XMFLOAT2 size = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);
    XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

    SetTexture(g_TitleTexture);
    DrawSquare(position, size, color);
}