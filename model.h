#pragma once

#include <unordered_map>
#include <assert.h>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"

#include "texture.h"
#include "light.h"
#include "collision.h"

#pragma comment (lib, "assimp-vc143-mt.lib")



struct MODEL
{
	const aiScene* AiScene = nullptr;

	ID3D11Buffer** VertexBuffer = nullptr;
	ID3D11Buffer** IndexBuffer = nullptr;

	bool m_BlenderFlag = false;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> Texture;

	Collision::AABB aabb;

	MODEL(bool blenderFlag) : m_BlenderFlag(blenderFlag) {};
};


MODEL* ModelLoad(const char* FileName, bool frag = false);
void ModelDraw(MODEL* model, const XMMATRIX& world, std::wstring forceTexture = L"null");
void ModelRelease(MODEL* model);

