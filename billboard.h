/*==============================================================================

   ビルボード [billboard.h]
														 Author : 桜井優輝
														 Date   : 24/12/17
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once

#include "main.h"
#include "renderer.h"

void InitBillBoard();
void UpdateBillBoard();
void DrawBillBoard(const XMMATRIX& worldMatrix, const XMMATRIX& cameraViewMatrix, int textureID, const XMFLOAT2& texturePatternSize, const XMFLOAT3& scaleFloat3 = { 1.0f, 1.0f, 1.0f }, int patternNo = 0);
void UninitBillBoard();

