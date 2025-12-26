#pragma once

#include <vector>
#include "main.h"

const std::vector<VERTEX_3D> DEFAULT_TRIAGLE =
{
	{ XMFLOAT3{-0.5f, 0.0f, 0.0f}, XMFLOAT3{0.0f, 0.0f, -1.0f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 0.0f} },
	{ XMFLOAT3{ 0.0f, 1.0f, 0.0f}, XMFLOAT3{0.0f, 0.0f, -1.0f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.25f, 0.0f} },
	{ XMFLOAT3{ 0.5f, 0.0f, 0.0f}, XMFLOAT3{0.0f, 0.0f, -1.0f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 0.25f} }
};

void InitCustom3D();
void UninitCustom3D();

void DrawCustom3D(const XMMATRIX& world, int textureID = NULL, const std::vector<VERTEX_3D> vertex = DEFAULT_TRIAGLE, int patternNo = 0, int xPatternCount = 0, int yPatternCount = 0);