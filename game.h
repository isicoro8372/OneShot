#pragma once

constexpr float CAMERA_RADIUS = 2.5f;

enum BALLSTATE
{
    BALLSTATE_WAIT,
    BALLSTATE_SHOTENDSTEP,
    BALLSTATE_SHOT,
    BALLSTATE_MOVE,
    BALLSTATE_RESULT,
    BALLSTATE_MAX
};

void InitGame();
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

XMMATRIX GetCameraViewMatrix();

BALLSTATE GetBallState();

