#include "main.h"
#include "renderer.h"
#include "line.h"

//==============================================
//マクロ定義
//==============================================
#define NUM_LINE_VERTEX (4)

//==============================================
//プロトタイプ宣言
//==============================================

//==============================================
//グローバル関数
//==============================================
static ID3D11Buffer* g_VertexBuffer = NULL;

//==============================================
//初期化関数
//==============================================
HRESULT InitLine()
{
	ID3D11Device* pDevice = GetDevice();

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_LINE_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	return S_OK;
}

void UninitLine()
{
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

//==============================================
//描画処理
//ラインの場合の引数	始点座標、終点座標、色
//						スプライトのように中心からの座標という基準はない
//
//トライアングルの場合	画面上の頂点座標を3つ与える(時計回り)、あと色も
// 
//座標はXMFLOAT3型で与える。色はXMFLOAT4型で与える。
//==============================================
void DrawLine(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//左上
	vertex[0].Position = pos1;
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	//右上
	vertex[1].Position = pos2;
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(0.0f, 0.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	//頂点バッファ
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//プリミティブトポロジ
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//ポリゴン描画
	GetDeviceContext()->Draw(NUM_LINE_VERTEX, 0);
}