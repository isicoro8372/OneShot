#ifndef TRAIL_H
#define TRAIL_H

#include "main.h"
#include "renderer.h"

void TrailInitialize(void);
void TrailFinalize(void);
void TrailUpdate(void);
void TrailDraw(XMVECTOR cameraPosition, float width = 0.2f);

void SetTrailPosition(const XMVECTOR& Position);
void ResetTrailPosition(const XMVECTOR& Position);
void SetTrailTargetCamera(const XMVECTOR& position);

#endif