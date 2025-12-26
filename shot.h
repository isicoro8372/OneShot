#pragma once
#include "ball.h"

constexpr int SHOT_POWER_MAX = 50;

void InitShot();
void UpdateShot();
void DrawShot();
void UninitShot();

void SetBallObject(Ball* ball);
void SetShotMatrix(const XMMATRIX& matrix);
XMVECTOR GetShotVelocity(float g_ChargePercentage);