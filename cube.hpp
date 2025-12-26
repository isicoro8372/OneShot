/*==============================================================================

   ’èŒ^3D}Œ`•`‰æ [cube.hpp]
														 Author : ÷ˆä—D‹P
														 Date   : 24/10/16
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once

#include "main.h"

void InitCube();
void UninitCube();

void DrawCube(const XMMATRIX& world, int textureID = NULL, int patternNo = 0, int xPatternCount = 0, int yPatternCount = 0);
