#define NOMINMAX

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
#include "custom3d.h"
#include "model.h"

#include "camera.h"
#include "camera_move.h"
#include "camera_tracking.h"
#include "camera_pov.h"
#include "camera_easing.h"

#include "background.h"
#include "fade.hpp"

#include "ball.h"
#include "shot.h"
#include "trail.h"
#include "skydome.h"
#include "shadow.h"

//マップデータ
#include "MapData.h"

//マクロ定義
constexpr int UI_GAUGE_SIZE_X = 40;
constexpr int UI_GAUGE_SIZE_Y = 300;

constexpr int SHOT_STROKE_SEGMENTS = 3;
constexpr int SHOT_TIME_SEGMENTS = 5;
constexpr int SHOT_SCORE_SEGMENTS = 8;

constexpr int NUMBER_SIZE_X = 80;
constexpr int NUMBER_SIZE_Y = 80;
constexpr int NUMBER_SEG_SPACE = -10;

//テクスチャ
static int g_TileTextureID[TILE_ID_MAX];
static int g_NumberTextureID;

static int g_TimerTextureID;
static int g_ShotGaugeTextureID;

static int g_StageResultTextureID;
static int g_TotalResultTextureID;

//サウンド
static int g_BoundSE;
static int g_ShotSE;
static int g_CupInSE;

//座標
static XMFLOAT3 position = {};
static XMFLOAT3 rotate = {};
static XMFLOAT3 size = {1.0f, 1.0f, 1.0f};

//カメラ
static Camera* g_Camera[DEFAULT_CAMERA_MAX];
static int g_CameraID = 0;

//ボール関係
static MODEL* g_BallModel = nullptr;

static Ball* g_Ball = nullptr;

static XMFLOAT3 g_ShootDirection = { 0.0f, 2.5f, 10.0f };
static XMFLOAT3 g_LastShotPosition = {};
static float g_Xangle, g_Yangle = 0.0f;

static int g_GaugeCount = 0;
static float g_ChargePercentage = 0.0f;

//スコア関連
static int g_Strokes = 0;
static int g_ThisStrokes = 0;
static int g_Gametime = 0;
static bool IsScoreReset = true;


//マップ
static int g_StageID = 1;

static Map* g_Map;

static BALLSTATE g_BallState;
static unsigned int g_FrameCounter;

void InitGame()
{
    InitCube();
    InitCustom3D();
    InitBillBoard();
    InitShot();
    InitSkyDome();
    TrailInitialize();
    InitShadow();

    //スコア
    g_ThisStrokes = 0;
    if (IsScoreReset)
    {
        g_Strokes = 0;
        g_Gametime = 0;
        g_StageID = 1;
        IsScoreReset = false;
    }

    //マップ
    switch (g_StageID)
    {
    case 1:
        g_Map = new Map1();
        break;
    case 2:
        g_Map = new Map2();
        break;
    case 3:
        g_Map = new Map3();
        break;
    default:
        g_Map = nullptr;
        break;
    }
    assert(g_Map);

    //ボール関係
    g_Ball = new Ball({ 5.0f, 5.0f, 1.0f });
    g_Xangle = XMConvertToRadians(-15.0f);
    g_Yangle = 0.0f;
    g_BallState = BALLSTATE_MOVE;
    g_FrameCounter = 0;
    ResetTrailPosition(g_Ball->GetPosition());

    //テクスチャ
    g_TimerTextureID = LoadTexture(L"asset/texture/blank.png");
    g_ShotGaugeTextureID = LoadTexture(L"asset/texture/blank.png");
    g_NumberTextureID = LoadTexture(L"asset/texture/number.png");
    g_StageResultTextureID = LoadTexture(L"asset/texture/stageresult.png");
    g_TotalResultTextureID = LoadTexture(L"asset/texture/totalresult.png");

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

    //サウンド
    g_BoundSE = LoadAudio("asset/audio/bound.wav");
    g_ShotSE = LoadAudio("asset/audio/shot.wav");
    g_CupInSE = LoadAudio("asset/audio/cupin.wav");

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
    g_Camera[DEFAULT_CAMERA_MAX - 2] = new EasingCamera({ 0.0f, 1.0f, -10.0f }, { -2.0f, 2.0f, -5.0f }, 60);

    g_BallModel = ModelLoad("asset/model/ball.fbx");

    g_Ball->LoadModel();

    SetDirectionalLight(GetWorldLightVector(), GetLocalLightVector());
    SetAmbient({ 0.5f, 0.5f, 0.5f, 0.0f});

    //ボールの登録
    SetBallObject(g_Ball);
    SetShotMatrix(XMMatrixInverse(nullptr, g_Camera[2]->GetViewMatrix()));
}


void UninitGame()
{
    g_Ball->ModelRelease();

    UninitCube();
    UninitCustom3D();
    UninitBillBoard();
    UninitShot();
    UnInitSkyDome();
    TrailFinalize();
    UninitShadow();

    UnloadAudio(g_BoundSE);
    UnloadAudio(g_ShotSE);
    UnloadAudio(g_CupInSE);

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

void UpdateGame()
{
    SetShotMatrix(XMMatrixRotationX(g_Xangle) * XMMatrixRotationY(g_Yangle));
    UpdateShot();
    
    if (g_BallState != BALLSTATE_RESULT)
    {
        g_Gametime++;
    }

    g_ChargePercentage = g_GaugeCount % SHOT_POWER_MAX / ((float)SHOT_POWER_MAX);

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

        //カメラ位置設定
        if (g_CameraID != DEFAULT_CAMERA_MAX - 1)
        {
            g_CameraID = 0;
        }
        XMFLOAT3 cameraPosition_float;
        XMStoreFloat3(&cameraPosition_float, g_Ball->GetPosition());
        cameraPosition_float = XMFLOAT3{ cameraPosition_float.x + (-sin(g_Yangle) * CAMERA_RADIUS), cameraPosition_float.y + 0.5f, cameraPosition_float.z + (-cos(g_Yangle) * CAMERA_RADIUS) };
        g_Camera[0]->SetPosition(XMLoadFloat3(&cameraPosition_float));
        static_cast<PovCamera*>(g_Camera[0])->SetBallPosition(g_Ball->GetPosition());

        if (GetKeyDown('W'))
        {
            g_Xangle -= 0.02f;
            g_Xangle = std::max(g_Xangle, XMConvertToRadians(-45.0f));
        }
        if (GetKeyDown('S'))
        {
            g_Xangle += 0.02f;
            g_Xangle = std::min(g_Xangle, XMConvertToRadians(-15.0f));
        }
        if (GetKeyDown('A'))
        {
            g_Yangle -= 0.02f;
        }
        if (GetKeyDown('D'))
        {
            g_Yangle += 0.02f;
        }


        if (GetKeyTrigger(VK_SPACE))
        {
            XMStoreFloat3(&g_LastShotPosition, g_Ball->GetPosition());
            g_Ball->AddForce(GetShotVelocity(g_ChargePercentage));
            g_FrameCounter = 0;
            g_Strokes++;
            g_ThisStrokes++;
            g_BallState = BALLSTATE_SHOTENDSTEP;
        }
        break;

    case BALLSTATE_SHOTENDSTEP:
        g_FrameCounter++;
        if (g_FrameCounter > 30)
        {
            PlayAudio(g_ShotSE);
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

        //XMFLOAT3 ballPosition;
        //XMStoreFloat3(&ballPosition, g_Ball->GetPosition());
        //Collision::Sphere ballCollision(ballPosition, 0.2f);
        //XMFLOAT3 polePosition;
        //XMStoreFloat3(&polePosition, /*ゴールポジ*/);
        //Collision::Capsule poleCollisition(polePosition, 1.0f, 0.25f);

        //if (poleCollisition.isOverlap(ballCollision))
        //{
        //    g_Ball->OnOverlap();
        //    PlayAudio(g_CupInSE);
        //    g_StageID++;
        //    if (g_StageID > STAGE_NUM)
        //    {
        //        IsScoreReset = true;
        //    }
        //    g_BallState = BALLSTATE_RESULT;
        //    g_FrameCounter = 0;
        //    return;
        //}

        //落下の検知
        XMFLOAT3 pos;
        XMStoreFloat3(&pos, g_Ball->GetPosition());
        if (pos.y < -2.0f)
        {
            g_Ball->SetPosition(XMLoadFloat3(&g_LastShotPosition));
            ResetTrailPosition(g_Ball->GetPosition());
            g_Ball->StopMove();
        }

        //当たり判定
        {
            bool isHit = g_Map->GenerateCollider(*g_Ball);

            if (isHit)
            {
                PlayAudio(g_BoundSE);
            }
            //地面で止まった
            if (g_Ball->IsStopped(isHit))
            {
                static_cast<EasingCamera*>(g_Camera[DEFAULT_CAMERA_MAX - 2])->SetTarget(g_Ball);
                static_cast<EasingCamera*>(g_Camera[DEFAULT_CAMERA_MAX - 2])->MoveStart(g_Camera[g_CameraID]->GetPosition(), g_Ball->GetPosition() - XMVECTOR{ 3.0f, -3.0f, 0.0f });
                g_CameraID = DEFAULT_CAMERA_MAX - 2;
                g_BallState = BALLSTATE_WAIT;
                g_FrameCounter = 0;
            }

            //ゴールに触れた
            if (isHit && g_Map->GetTileType() == 10)
            {
                PlayAudio(g_CupInSE);
                g_StageID++;
                if (g_StageID > STAGE_NUM)
                {
                    IsScoreReset = true;
                }
                g_BallState = BALLSTATE_RESULT;
                g_FrameCounter = 0;
                return;
            }
        }
        break;
    case BALLSTATE_RESULT:
        g_FrameCounter++;
        if (g_FrameCounter > 120)
        {
            if (g_StageID <= STAGE_NUM)
            {
                SetScene(SCENE_GAME);
                return;
            }
            else if (GetKeyTrigger(VK_SPACE))
            {
                SetScene(SCENE_TITLE);
                return;
            }
        }
        break;

    }

    SetTrailPosition(g_Ball->GetPosition());

    //カメラの更新
    g_Camera[g_CameraID]->Update();
    SetTrailTargetCamera(g_Camera[g_CameraID]->GetPosition());
    SetShadowPosition(g_Ball->GetPosition(), g_Map);
}

void DrawGame()
{
    SetSkyDomeCenterLocation(g_Camera[g_CameraID]->GetPosition());

    SetShader3D();
    SetDepthEnable(true);
    
    SetViewMatrix(g_Camera[g_CameraID]->GetViewMatrix());
    SetProjectionMatrix(g_Camera[g_CameraID]->GetPerspectiveMatrix());

    DrawSkyDome();

    XMMATRIX worldMatrix = XMMatrixTranslation(0.0f, 5.0f, 0.0f);
    SetWorldMatrix(worldMatrix);

    g_Map->Draw(g_TileTextureID);

    g_Ball->Draw();
    DrawShadow();

    DrawCustom3D(worldMatrix, g_ShotGaugeTextureID);

    if (g_BallState == BALLSTATE_SHOT || g_BallState == BALLSTATE_SHOTENDSTEP)
    {
        DrawShot();
    }

    TrailDraw(g_Camera[g_CameraID]->GetPosition());

    //UI
    {
        XMFLOAT3 position = {};
        XMFLOAT2 size = {};
        XMFLOAT4 color = {};

        if (g_BallState == BALLSTATE_SHOT || g_BallState == BALLSTATE_SHOTENDSTEP)
        {
            //ゲージ
            position = XMFLOAT3(SCREEN_WIDTH - SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2.6f + UI_GAUGE_SIZE_Y / 2.0f * (1.0f - g_ChargePercentage), 0);
            size = XMFLOAT2(UI_GAUGE_SIZE_X, UI_GAUGE_SIZE_Y * g_ChargePercentage);
            color = XMFLOAT4(g_ChargePercentage, 1.35f - g_ChargePercentage, 0.0f, 1.0f);
            DrawSquare(position, size, color);

            position = XMFLOAT3(SCREEN_WIDTH - SCREEN_WIDTH / 3, SCREEN_HEIGHT / 2.6f, 0);
            size = XMFLOAT2(UI_GAUGE_SIZE_X, UI_GAUGE_SIZE_Y);
            if (g_ChargePercentage < 0.01f)
            {
                color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
            }
            else
            {
                color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
            }
            SetTexture(g_ShotGaugeTextureID);
            DrawSquare(position, size, color);
        }

        //打数
        int value = g_Strokes;
        position = XMFLOAT3(SCREEN_WIDTH - NUMBER_SIZE_X / 1.5f, NUMBER_SIZE_Y / 1.5f, 0);
        size = XMFLOAT2(NUMBER_SIZE_X, NUMBER_SIZE_Y);
        color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        SetTexture(g_NumberTextureID);
        for (int i = 0; i < SHOT_STROKE_SEGMENTS; i++)
        {
            int num = value % 10;
            value /= 10;
            if (i == 0 || num != 0)
            {
                DrawSquareRotateUV(position, size, color, 0.0f, num, 10, 1);
            }
            position.x -= (NUMBER_SIZE_X + NUMBER_SEG_SPACE);
        }

        //スコア
        if (g_BallState == BALLSTATE_RESULT) //中間
        {
            value = g_FrameCounter;
            if (g_StageID <= STAGE_NUM)
            {
                //Strokes表示
                if (((float)value) / 30.0f >= 1.0f)
                {
                    int strokeValue = g_ThisStrokes;

                    //桁数の計算
                    unsigned digit = 0;
                    do
                    {
                        strokeValue /= 10;
                        digit++;
                    } while (strokeValue != 0);
                    //描画
                    strokeValue = g_ThisStrokes;
                    position = XMFLOAT3(SCREEN_WIDTH / 2 + (NUMBER_SIZE_X / 2 * (digit - 1)), SCREEN_HEIGHT / 2, 0);
                    size = XMFLOAT2(NUMBER_SIZE_X, NUMBER_SIZE_Y);
                    color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                    SetTexture(g_NumberTextureID);
                    for (int i = 0; i < SHOT_STROKE_SEGMENTS; i++)
                    {
                        int num = strokeValue % 10;
                        strokeValue /= 10;
                        if (i == 0 || digit > i)
                        {
                            DrawSquareRotateUV(position, size, color, 0.0f, num, 10, 1);
                        }
                        position.x -= (NUMBER_SIZE_X + NUMBER_SEG_SPACE);
                    }
                }

                //テキスト表示
                if (((float)value) / 10.0f >= 1.0f)
                {
                    position = XMFLOAT3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
                    size = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);
                    color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                    SetTexture(g_StageResultTextureID);
                    DrawSquare(position, size, color);
                }
            }
            else //最終スコア
            {
                //Strokes表示
                if (((float)value) / 50.0f >= 1.0f)
                {
                    int strokeValue = g_Strokes;

                    //桁数の計算
                    unsigned digit = 0;
                    do
                    {
                        strokeValue /= 10;
                        digit++;
                    } while (strokeValue != 0);
                    //描画
                    strokeValue = g_Strokes;
                    position = XMFLOAT3(SCREEN_WIDTH / 2 + (NUMBER_SIZE_X / 2 * (digit - 1)), SCREEN_HEIGHT / 4, 0);
                    size = XMFLOAT2(NUMBER_SIZE_X, NUMBER_SIZE_Y);
                    color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                    SetTexture(g_NumberTextureID);
                    for (int i = 0; i < SHOT_STROKE_SEGMENTS; i++)
                    {
                        int num = strokeValue % 10;
                        strokeValue /= 10;
                        if (i == 0 || digit > i)
                        {
                            DrawSquareRotateUV(position, size, color, 0.0f, num, 10, 1);
                        }
                        position.x -= (NUMBER_SIZE_X + NUMBER_SEG_SPACE);
                    }
                }

                //Time表示
                if (((float)value) / 90.0f >= 1.0f)
                {
                    int timeValue = g_Gametime / 60;

                    //桁数の計算
                    unsigned digit = 0;
                    do
                    {
                        timeValue /= 10;
                        digit++;
                    } while (timeValue != 0);
                    //描画
                    timeValue = g_Gametime / 60;
                    position = XMFLOAT3(SCREEN_WIDTH / 2 + (NUMBER_SIZE_X / 2 * (digit - 1)), SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 16, 0);
                    size = XMFLOAT2(NUMBER_SIZE_X, NUMBER_SIZE_Y);
                    color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                    SetTexture(g_NumberTextureID);
                    for (int i = 0; i < SHOT_TIME_SEGMENTS; i++)
                    {
                        int num = timeValue % 10;
                        timeValue /= 10;
                        if (i == 0 || digit > i)
                        {
                            DrawSquareRotateUV(position, size, color, 0.0f, num, 10, 1);
                        }
                        position.x -= (NUMBER_SIZE_X + NUMBER_SEG_SPACE);
                    }
                }

                //Score表示
                if (((float)value) / 120.0f >= 1.0f)
                {
                    int scoreValue = (5000000.0f * STAGE_NUM / ((g_Strokes + 1) * g_Gametime));

                    //桁数の計算
                    unsigned digit = 0;
                    do
                    {
                        scoreValue /= 10;
                        digit++;
                    } while (scoreValue != 0);
                    //描画
                    scoreValue = (5000000.0f * STAGE_NUM / ((g_Strokes + 1) * g_Gametime));
                    position = XMFLOAT3(SCREEN_WIDTH / 2 + (NUMBER_SIZE_X / 2 * (digit - 1)), SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 3, 0);
                    size = XMFLOAT2(NUMBER_SIZE_X, NUMBER_SIZE_Y);
                    color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                    SetTexture(g_NumberTextureID);
                    for (int i = 0; i < SHOT_SCORE_SEGMENTS; i++)
                    {
                        int num = scoreValue % 10;
                        scoreValue /= 10;
                        if (i == 0 || digit > i)
                        {
                            DrawSquareRotateUV(position, size, color, 0.0f, num, 10, 1);
                        }
                        position.x -= (NUMBER_SIZE_X + NUMBER_SEG_SPACE);
                    }
                }

                //テキスト表示
                if (((float)value) / 10.0f >= 1.0f)
                {
                    if (value > 120)
                    {
                        value = 120;
                    }
                    int seg = value / 15.0f;
                    position = XMFLOAT3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
                    size = XMFLOAT2(SCREEN_WIDTH , SCREEN_HEIGHT);
                    color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
                    SetTexture(g_TotalResultTextureID);
                    DrawSquare(position, size, color);
                }
            }
        }
    }
}

XMMATRIX GetCameraViewMatrix()
{
    if (g_Camera[g_CameraID])
    {
        return g_Camera[g_CameraID]->GetViewMatrix();
    }

    return XMMatrixIdentity();
}

BALLSTATE GetBallState()
{
    return g_BallState;
}