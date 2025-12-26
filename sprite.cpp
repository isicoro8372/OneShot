/*==============================================================================

   定型図形描画 [sprite.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/

#include "main.h"
#include "renderer.h"
#include "sprite.h"

//==============================================
//マクロ定義
//==============================================
#define NUM_SPRITE_VERTEX (4)

#define SCROLL_ADUST_X (/*PIECE_SIZE_X * */9.8f)
#define SCROLL_ADUST_Y (/*PIECE_SIZE_Y * */0.7f)

//==============================================
//プロトタイプ宣言
//==============================================

//==============================================
//グローバル関数
//==============================================
static ID3D11Buffer* g_VertexBuffer = NULL;
static XMFLOAT2 scrollOffset = { SCROLL_ADUST_X, SCROLL_ADUST_Y };

//==============================================
//初期化関数
//==============================================
HRESULT InitSprite()
{
	ID3D11Device* pDevice = GetDevice();

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * NUM_SPRITE_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

	return S_OK;
}

void UninitSprite()
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
void DrawSquare(XMFLOAT3 pos, XMFLOAT2 size, XMFLOAT4 color, bool scroll)
{
	SetShader2D();

	SetWorldViewProjection2D();

	if (scroll)
	{
		pos.x += scrollOffset.x;
		pos.y += scrollOffset.y;
	}

	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	FLOAT size_W = size.x * 0.5f;
	FLOAT size_H = size.y * 0.5f;

	//四角形の中心を(0,0)と考えた4つの座標を作る。

	//左上
	vertex[0].Position = XMFLOAT3(-size_W, -size_H, 0);
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	//右上
	vertex[1].Position = XMFLOAT3(size_W, -size_H, 0);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	//左下
	vertex[2].Position = XMFLOAT3(-size_W, size_H, 0);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	//右下
	vertex[3].Position = XMFLOAT3(size_W, size_H, 0);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


	//平行移動処理
	for (int i = 0; i < 4; i++)
	{
		vertex[i].Position.x += pos.x;
		vertex[i].Position.y += pos.y;
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);


	//頂点バッファ
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//プリミティブトポロジ
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}

//行列使用
void DrawSquare(XMFLOAT2 size, XMFLOAT4 color, XMMATRIX worldMatrix, int patternNo, int xPatternCount, int yPatternCount, bool reverseTexture)
{
	SetWorldMatrix(worldMatrix);

	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float size_W = size.x * 0.5f;
	float size_H = size.y * 0.5f;

	//四角形の中心を(0,0)と考えた4つの座標を作る。

	//左上
	vertex[0].Position = XMFLOAT3(-size_W, -size_H, 0);
	vertex[0].Diffuse = color;
	//右上
	vertex[1].Position = XMFLOAT3(size_W, -size_H, 0);
	vertex[1].Diffuse = color;
	//左下
	vertex[2].Position = XMFLOAT3(-size_W, size_H, 0);
	vertex[2].Diffuse = color;
	//右下
	vertex[3].Position = XMFLOAT3(size_W, size_H, 0);
	vertex[3].Diffuse = color;

	//テクスチャ設定
	if (xPatternCount > 0 && yPatternCount > 0)
	{
		float texPt_W = 1.0f / xPatternCount;
		float texPt_H = 1.0f / yPatternCount;

		float texPt_U = (patternNo % xPatternCount) * texPt_W;
		float texPt_V = (patternNo / xPatternCount) * texPt_H;

		if (reverseTexture)
		{
			//反転
			vertex[0].TexCoord = XMFLOAT2(texPt_U + texPt_W, texPt_V);
			vertex[1].TexCoord = XMFLOAT2(texPt_U, texPt_V);
			vertex[2].TexCoord = XMFLOAT2(texPt_U + texPt_W, texPt_V + texPt_H);
			vertex[3].TexCoord = XMFLOAT2(texPt_U, texPt_V + texPt_H);
		}
		else
		{
			vertex[0].TexCoord = XMFLOAT2(texPt_U, texPt_V);
			vertex[1].TexCoord = XMFLOAT2(texPt_U + texPt_W, texPt_V);
			vertex[2].TexCoord = XMFLOAT2(texPt_U, texPt_V + texPt_H);
			vertex[3].TexCoord = XMFLOAT2(texPt_U + texPt_W, texPt_V + texPt_H);
		}
	}
	else
	{
		vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
		vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
		vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
		vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);


	//頂点バッファ
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//プリミティブトポロジ
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//ポリゴン描画
	GetDeviceContext()->Draw(4, 0);
}

//三角形
void DrawTriangle(XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT4 color)
{
	SetWorldViewProjection2D();

	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;


	vertex[0].Position = v0;
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = v1;
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = v2;
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	//頂点バッファ
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//プリミティブトポロジ
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//ポリゴン描画
	GetDeviceContext()->Draw(3, 0);
}



void DrawSquareRotate(XMFLOAT3 position, XMFLOAT2 size, XMFLOAT4 color, FLOAT radian)
{
	SetWorldViewProjection2D();

	//頂点の作成
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float size_W = size.x * 0.5f;
	float size_H = size.y * 0.5f;

	//左上 中心からサイズの半分の場所で座標を作る
	vertex[0].Position = XMFLOAT3(-size_W, -size_H, 0.0f);
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	//右上 中心からサイズの半分の場所で座標を作る
	vertex[1].Position = XMFLOAT3(size_W, -size_H, 0.0f);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	//左下 中心からサイズの半分の場所で座標を作る
	vertex[2].Position = XMFLOAT3(-size_W, size_H, 0.0f);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	//右下 中心からサイズの半分の場所で座標を作る
	vertex[3].Position = XMFLOAT3(size_W, size_H, 0.0f);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//回転処理
	float co = cosf(radian);
	float si = sinf(radian);

	for (int i = 0; i < 4; i++)
	{
		float x = vertex[i].Position.x;
		float y = vertex[i].Position.y;
		vertex[i].Position.x = (x * co - y * si) + position.x;
		vertex[i].Position.y = (x * si + y * co) + position.y;
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// ポリゴン描画
	GetDeviceContext()->Draw(NUM_SPRITE_VERTEX, 0);
}

void DrawSquareRotateUV(XMFLOAT3 position, XMFLOAT2 size, XMFLOAT4 color, FLOAT radian, int patternNo, int xPatternCount, int yPatternCount, bool reverseTexture)
{
	SetWorldViewProjection2D();

	//頂点の作成
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	float size_W = size.x * 0.5f;
	float size_H = size.y * 0.5f;

	float texPt_W = 1.0f / xPatternCount;
	float texPt_H = 1.0f / yPatternCount;

	float texPt_U = (patternNo % xPatternCount) * texPt_W;
	float texPt_V = (patternNo / xPatternCount) * texPt_H;

	//左上 中心からサイズの半分の場所で座標を作る
	vertex[0].Position = XMFLOAT3(-size_W, -size_H, 0.0f);
	vertex[0].Diffuse = color;
	//右上 中心からサイズの半分の場所で座標を作る
	vertex[1].Position = XMFLOAT3(size_W, -size_H, 0.0f);
	vertex[1].Diffuse = color;
	//左下 中心からサイズの半分の場所で座標を作る
	vertex[2].Position = XMFLOAT3(-size_W, size_H, 0.0f);
	vertex[2].Diffuse = color;
	//右下 中心からサイズの半分の場所で座標を作る
	vertex[3].Position = XMFLOAT3(size_W, size_H, 0.0f);
	vertex[3].Diffuse = color;

	//テクスチャ反転
	if (reverseTexture)
	{
		//反転
		vertex[0].TexCoord = XMFLOAT2(texPt_U + texPt_W, texPt_V);
		vertex[1].TexCoord = XMFLOAT2(texPt_U, texPt_V);
		vertex[2].TexCoord = XMFLOAT2(texPt_U + texPt_W, texPt_V + texPt_H);
		vertex[3].TexCoord = XMFLOAT2(texPt_U, texPt_V + texPt_H);
	}
	else
	{
		vertex[0].TexCoord = XMFLOAT2(texPt_U, texPt_V);
		vertex[1].TexCoord = XMFLOAT2(texPt_U + texPt_W, texPt_V);
		vertex[2].TexCoord = XMFLOAT2(texPt_U, texPt_V + texPt_H);
		vertex[3].TexCoord = XMFLOAT2(texPt_U + texPt_W, texPt_V + texPt_H);
	}

	//回転処理
	float co = cosf(radian);
	float si = sinf(radian);
	for (int i = 0; i < 4; i++)
	{
		float x = vertex[i].Position.x;
		float y = vertex[i].Position.y;
		vertex[i].Position.x = (x * co - y * si) + position.x;
		vertex[i].Position.y = (x * si + y * co) + position.y;
	}

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);


	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// ポリゴン描画
	GetDeviceContext()->Draw(NUM_SPRITE_VERTEX, 0);
}

//=========================================
// 頂点計算
//=========================================
std::vector<VERTEX_3D> CalcCircle(XMFLOAT3 pos, float rad, XMFLOAT4 color)
{
	XMFLOAT3 basepos;
	int part = (2 * PI * rad) * CIRCLE_ANCHOR_MULTIPLIER;
	std::vector<VERTEX_3D> v_vertexCircle(part + 1);

	// 円を描画
	for (int i = 0; i < part; i++)
	{
		// 割合を計算
		float rate = (float)(i + 1) / part;

		// 半径 * cos(直径 * 現在の描画割合)
		basepos.x = rad * cos(2.0f * PI * rate);

		// 半径 * sin(直径 * 現在の描画割合)
		basepos.y = rad * sin(2.0f * PI * rate);

		v_vertexCircle[i].Position = XMFLOAT3(basepos.x + pos.x, basepos.y + pos.y, 0.0f);
		v_vertexCircle[i].Diffuse = color;
		v_vertexCircle[i].TexCoord = XMFLOAT2(0.0f, 0.0f);
	}

	//最後と最初をつなぐ
	v_vertexCircle[(int)part].Position = v_vertexCircle[0].Position;
	v_vertexCircle[(int)part].Diffuse = v_vertexCircle[0].Diffuse;
	v_vertexCircle[(int)part].TexCoord = XMFLOAT2(0.0f, 0.0f);

	return v_vertexCircle;
}

std::vector<VERTEX_3D> CalcSquare(XMFLOAT3 pos, XMFLOAT2 size, XMFLOAT4 color)
{
	std::vector<VERTEX_3D> vertex(4);

	FLOAT size_W = size.x * 0.5f;
	FLOAT size_H = size.y * 0.5f;

	//四角形の中心を(0,0)と考えた4つの座標を作る。
	//左上
	vertex[0].Position = XMFLOAT3(-size_W, -size_H, 0);
	vertex[0].Diffuse = color;
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
	//右上
	vertex[1].Position = XMFLOAT3(size_W, -size_H, 0);
	vertex[1].Diffuse = color;
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
	//左下
	vertex[2].Position = XMFLOAT3(-size_W, size_H, 0);
	vertex[2].Diffuse = color;
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
	//右下
	vertex[3].Position = XMFLOAT3(size_W, size_H, 0);
	vertex[3].Diffuse = color;
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


	//平行移動処理
	for (int i = 0; i < 4; i++)
	{
		vertex[i].Position.x += pos.x;
		vertex[i].Position.y += pos.y;
	}

	return vertex;
}

