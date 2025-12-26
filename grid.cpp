/*==============================================================================

   グリッド描画 [grid.cpp]
														 Author : 桜井優輝
														 Date   : 24/10/23
--------------------------------------------------------------------------------

==============================================================================*/
#include "main.h"
#include "renderer.h"
#include "grid.h"

static ID3D11Buffer* g_VertexBuffer = NULL;
static constexpr int GRID_X_NUM = 10;
static constexpr int GRID_Z_NUM = 10;
static constexpr int GRID_LINE_X_NUM = GRID_X_NUM + 1;
static constexpr int GRID_LINE_Z_NUM = GRID_Z_NUM + 1;
static constexpr int GRID_VERTEX_NUM = (GRID_LINE_X_NUM * 2) + (GRID_LINE_Z_NUM * 2);

void InitGrid()
{
	ID3D11Device* pDevice = GetDevice();

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * GRID_VERTEX_NUM;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	constexpr float start_x = GRID_X_NUM * -0.5f;
	constexpr float start_z = GRID_Z_NUM * -0.5f;
	constexpr float end_x = GRID_X_NUM * 0.5f;
	constexpr float end_z = GRID_Z_NUM * 0.5f;
	
	static int lineCount = 0;
	for (float x = start_x; x <= end_x; x += 1.0f)
	{
		vertex[lineCount++] = { { x,  0.0f, start_z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} };
		vertex[lineCount++] = { { x,  0.0f, end_z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} };
	}

	for (float z = start_z; z <= end_z; z += 1.0f)
	{
		vertex[lineCount++] = { { start_x,  0.0f, z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} };
		vertex[lineCount++] = { { end_x,  0.0f, z}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} };
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);
}

void UninitGrid()
{
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = nullptr;
	}
}

void DrawGrid()
{
	SetWorldViewProjection3D(XMMatrixIdentity());

	//頂点バッファ
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//プリミティブトポロジ
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//マテリアル設定
	MATERIAL material = {};
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	SetWorldMatrix(XMMatrixIdentity());

	//ポリゴン描画
	GetDeviceContext()->Draw(GRID_VERTEX_NUM, 0);
}