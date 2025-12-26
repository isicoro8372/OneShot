#include "map.h"
#include "light.h"
#include "cube.hpp"
#include "texture.h"
#include "model.h"
#include "billboard.h"
#include "game.h"

Map::Map()
{
	m_ArrowModel = ModelLoad("asset/model/Arrow.fbx");
	m_PoleModel = ModelLoad("asset/model/Pole.fbx");
	m_ArrowModelTexture = LoadTexture(L"asset/texture/blank.png");

	for (int i = 0; i < MODEL_ID::MODEL_MAX; i++)
	{
		m_Models.push_back(ModelLoad(modelPath[i].c_str(), true));
	}
}

Map::~Map()
{
	ModelRelease(m_ArrowModel);
	ModelRelease(m_PoleModel);

	for (int i = 0; i < MODEL_ID::MODEL_MAX; i++)
	{
		ModelRelease(m_Models[i]);
	}
	m_Models.empty();
}

void Map::Draw(int* TextureID)
{
	m_UpdateFrame++;
	int tileID = 0;
	XMMATRIX world = XMMatrixIdentity();
	SetLight(world);

	for (int z = 0; z < GetMapZ(); z++)
	{
		for (int x = 0; x < GetMapX(); x++)
		{
			tileID = GetMapTile(z)[x];
			if(tileID != 0)
			{
				XMMATRIX translation = XMMatrixTranslation(x, GetMapHeight(z)[x], z);
				switch (tileID)
				{
				case 10:
					XMMATRIX goalPosition = translation;
					XMMATRIX arrowTranslation = XMMatrixTranslation(x, GetMapHeight(z)[x] + cos((float)m_UpdateFrame / 20.0f) + 3.5f + POLE_HEIGHT, z);
					XMMATRIX arrowRotate = XMMatrixRotationX(XMConvertToRadians(90.0f));
					XMMATRIX arrowScale = XMMatrixScaling(5.0f, 5.0f, 5.0f);
					SetTexture(m_ArrowModelTexture);
					ModelDraw(m_ArrowModel, arrowScale * arrowRotate * arrowTranslation);

					XMMATRIX poleScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
					SetTexture(m_ArrowModelTexture);
					ModelDraw(m_PoleModel, poleScale * goalPosition);
					DrawCube(translation, TextureID[tileID - 1]);
					break;
				case 21:
					ModelDraw(m_Models[0], translation);
					break;
				case 22:
					ModelDraw(m_Models[1], translation);
					break;
				case 23:
					ModelDraw(m_Models[2], translation);
					break;
				case 24:
					ModelDraw(m_Models[3], translation);
					break;
				default:
					DrawCube(translation, TextureID[tileID - 1]);
					break;
				}
			}
		}
	}
}

bool Map::GenerateCollider(Ball& ball)
{
	bool isHit = false;
	m_HitTileType = -1;
	XMFLOAT3 ballPosition;
	XMStoreFloat3(&ballPosition, ball.GetPosition());

	for (int z = std::round(ballPosition.z - COLLISION_RADIUS); z < std::round(ballPosition.z + COLLISION_RADIUS); z++)
	{
		if (z >= 0 && z < GetMapZ())
		{
			for (int x = std::round(ballPosition.x - COLLISION_RADIUS); x < std::round(ballPosition.x + COLLISION_RADIUS); x++)
			{
				if (x >= 0 && x < GetMapX())
				{
					if (GetMapTile(z)[x])
					{
						Collision::AABB cubeLocalAABB({ -0.5f, -0.5f, -0.5f }, { 0.5f, 0.5f, 0.5f });
						Collision::AABB cubeWorldAABB = cubeLocalAABB.Translation({ (float)x, GetMapHeight(z)[x], (float)z });

						if (GetMapTile(z)[x] == 10)
						{
							XMFLOAT3 ballPosition;
							XMStoreFloat3(&ballPosition, ball.GetPosition());
							Collision::Sphere ballCollision(ballPosition, 0.2f);

							//ƒS[ƒ‹ƒ|[ƒ‹
							//XMFLOAT3 polePosition = cubeWorldAABB.GetCenter();
							//polePosition.y += POLE_HEIGHT / 2.0f;
							//Collision::Capsule poleCollisition(polePosition, POLE_HEIGHT, POLE_RADIUS);
							//if (poleCollisition.isOverlap(ballCollision))
							//{
							//	m_HitTileType = GetMapTile(z)[x];
							//	isHit = true;
							//	ball.StopMove();
							//}

						}

						if (ball.GetAABB().isOverlap(cubeWorldAABB))
						{
							m_HitTileType = GetMapTile(z)[x];
							isHit = true;
							ball.OnHit(cubeWorldAABB, m_HitTileType - 21);
						}
					}
				}
			}
		}
	}

	return isHit;
}


int Map::GetTileType()
{
	return m_HitTileType;
}
