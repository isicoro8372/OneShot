/*==============================================================================

   定型3D図形描画 [cube.cpp]
														 Author : 桜井優輝
														 Date   : 24/10/16
--------------------------------------------------------------------------------

==============================================================================*/
#include "main.h"
#include "renderer.h"
#include "cube.hpp"
#include "light.h"
#include "texture.h"

static ID3D11Buffer* g_VertexBuffer = NULL;
static ID3D11Buffer* g_IndexBuffer = NULL;
static const int CUBE_VERTEX_NUM = 36;

const VERTEX_3D cubeVertex[] = {
	{ {-0.5f,   0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
	{ { 0.5f,   0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.25f, 0.0f} },
	{ {-0.5f,  -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.25f} },
	{ { 0.5f,  -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.25f, 0.25f} },

	{ { 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {0.25f, 0.0f} },
	{ { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.0f} },
	{ { 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {0.25f, 0.25f} },
	{ { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.25f} },

	{ { 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}, {0.5f, 0.0f} },
	{ {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}, {0.75f, 0.0f} },
	{ { 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}, {0.5f, 0.25f} },
	{ {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}, {0.75f, 0.25f} },

	{ {-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.75f, 0.0f} },
	{ {-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {1.0f,  0.0f} },
	{ {-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {0.75f, 0.25f} },
	{ {-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f}, {1.0f,  0.25f} },

	{ {-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f,  0.25f} },
	{ { 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.25f, 0.25f} },
	{ {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.0f,  0.5f} },
	{ { 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.25f, 0.5f} },

	{ { 0.5f,  -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.25f, 0.25f} },
	{ {-0.5f,  -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.5f,  0.25f} },
	{ { 0.5f,  -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.25f, 0.5f} },
	{ {-0.5f,  -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}, {0.5f,  0.5f} },
};

void InitCube()
{
	ID3D11Device* pDevice = GetDevice();

	//頂点取得
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = cubeVertex;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 24; //(CUBE_VERTEX_NUM - CUBE_VERTEX_NUM / 3);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	GetDevice()->CreateBuffer(&bd, &data, &g_VertexBuffer);

	//インデックスバッファ生成
	unsigned short indexes[] = {
	 0,  1,  2,  1,  3,  2,
	 4,  5,  6,  5,  7,  6,
	 8,  9, 10,  9, 11, 10,
	12, 13, 14, 13, 15, 14,
	16, 17, 18, 17, 19, 18,
	20, 21, 22, 21, 23, 22
	};

	data.pSysMem = indexes;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned short) * CUBE_VERTEX_NUM;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, &data, &g_IndexBuffer);
}

void UninitCube()
{
	if (g_IndexBuffer)
	{
		g_IndexBuffer->Release();
		g_IndexBuffer = nullptr;
	}

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = nullptr;
	}
}

void DrawCube(const XMMATRIX& world, int textureID, int patternNo, int xPatternCount, int yPatternCount)
{
	SetTexture(textureID);

	SetWorldMatrix(world);

	SetShader3D();

	//頂点バッファ
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//インデックスバッファ設定
	GetDeviceContext()->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	//プリミティブトポロジ
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアル設定
	MATERIAL material = {};
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);
	
	SetLight(world);

	//ポリゴン描画
	GetDeviceContext()->DrawIndexed(CUBE_VERTEX_NUM, 0, 0);
}
