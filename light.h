#pragma once

#include "main.h"
#include "renderer.h"

void InitLight();
void UpdateLight();
void DrawLight();
void UninitLight();

void SetDirectionalLight(const XMVECTOR& dir, const XMFLOAT4 color);
void SetAmbient(const XMFLOAT4& color);
void SetLight(const XMMATRIX& world);
