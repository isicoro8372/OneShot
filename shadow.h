#pragma once
#include "map.h"

void InitShadow();
void UpdateShadow();
void DrawShadow();
void UninitShadow();

void SetShadowPosition(const XMVECTOR& position, Map* map);