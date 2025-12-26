#pragma once

#include <vector>
#include <string>
#include "ball.h"
#include "model.h"

constexpr int STAGE_NUM = 3;
constexpr int TILE_ID_MAX = 10;
constexpr float POLE_HEIGHT = 4.0f;
constexpr float POLE_RADIUS = 0.0625f;

void DrawMap(int textureID);
const std::vector<std::string> modelPath =
{
	{"asset/model/slope0.fbx"},
	{"asset/model/slope1.fbx"},
	{"asset/model/slope2.fbx"},
	{"asset/model/slope3.fbx"},
};

class Map
{
	enum MODEL_ID
	{
		SLOPE_0,
		SLOPE_1,
		SLOPE_2,
		SLOPE_3,

		MODEL_MAX
	};
	MODEL* m_ArrowModel = nullptr;
	MODEL* m_PoleModel = nullptr;
	int m_ArrowModelTexture;
	int m_UpdateFrame = 0;

	std::vector<MODEL*> m_Models;

protected:
	int m_HitTileType = -1;

public:
	Map();
	virtual ~Map();

	virtual const XMFLOAT3* GetMapCameraPositions() = 0;
	virtual int GetMapCameraCount() = 0;

	virtual int GetMapX() = 0;
	virtual int GetMapZ() = 0;

	virtual const int* GetMapTile(int columns) = 0;
	virtual const float* GetMapHeight(int columns) = 0;

	int GetTileType();
	void Draw(int* textureID);
	bool GenerateCollider(Ball& ball);
};