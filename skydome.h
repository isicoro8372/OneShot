#pragma once

#include <DirectXMath.h>
using namespace DirectX;

void InitSkyDome();
void UpdateSkyDome(); 
void DrawSkyDome();
void UnInitSkyDome();

void SetSkyDomeCenterLocation(const XMVECTOR& position);