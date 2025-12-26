#include "main.h"
#include <string>
#include "renderer.h"
#include "debugPrintf.h"
#include "textureloader.hpp"
#include "custompolygon.hpp"

//=========================================
//CustomnPolygonクラス
//=========================================

//コンストラクタ
//CustomPolygon::CustomPolygon(D3D_PRIMITIVE_TOPOLOGY renderType, std::vector<VERTEX_3D> const vertex, int index, ID3D11ShaderResourceView* texture) : vertex(vertex), renderType(renderType), index(index), texture(texture)
//{
//	//頂点バッファ生成
//	ID3D11Device* pDevice = GetDevice();
//
//	ZeroMemory(&bd, sizeof(bd));
//
//	bd.Usage = D3D11_USAGE_DYNAMIC;
//	bd.ByteWidth = sizeof(VERTEX_3D) * vertex.size(); //頂点最大数をvectorの要素数に
//	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//	GetDevice()->CreateBuffer(&bd, NULL, &vertexBuffer);
//
//	//this->SetVertexVector(vertex);
//	DebugPrintf("init %d\n", this->index);
//}

//=========================================
//コンストラクタ
//=========================================
CustomPolygon::CustomPolygon(D3D_PRIMITIVE_TOPOLOGY renderType, std::vector<VERTEX_3D> const vertex, int index, std::string filePath) : vertex(vertex), renderType(renderType), index(index), filePath(filePath)
{
	//頂点バッファ生成
	ID3D11Device* pDevice = GetDevice();

	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * vertex.size(); //頂点最大数をvectorの要素数に
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &vertexBuffer);

	//this->SetVertexVector(vertex);
	loaderClass.InitializeLoader(filePath);

	DebugPrintf("init %d\n", this->index);
}

//=========================================
//デストラクタ
//=========================================
CustomPolygon::~CustomPolygon()
{
	DebugPrintf("final %d\n", this->index);
	if (this->vertexBuffer != NULL)
	{
		this->vertexBuffer->Release();
	}
}

//=========================================
//描画関数
//=========================================
void CustomPolygon::Draw()
{
	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	//マトリクス設定
	SetWorldViewProjection2D();

	//プリミティブトポロジ設定 https://learn.microsoft.com/ja-jp/windows/uwp/graphics-concepts/images/d3d10-primitive-topologies.png
	GetDeviceContext()->IASetPrimitiveTopology(this->renderType);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//描画処理
	D3D11_MAPPED_SUBRESOURCE msr;
	ID3D11ShaderResourceView* texture = loaderClass.GetSprite();

	//テクスチャ設定
	if (texture != NULL)
	{
		GetDeviceContext()->PSSetShaderResources(0, 1, &texture);
	}

	//頂点バッファに書き込む
	GetDeviceContext()->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	//バッファへコピー
	memcpy_s(msr.pData, sizeof(VERTEX_3D) * vertex.size(), &vertex[0], sizeof(VERTEX_3D) * vertex.size());

	//書き込み終了
	GetDeviceContext()->Unmap(vertexBuffer, 0);

	GetDeviceContext()->Draw(vertex.size(), 0);
}

//=========================================
//頂点設定(上書き可)
//=========================================
void CustomPolygon::SetVertexVector(std::vector<VERTEX_3D> const vertex)
{
	this->vertex = vertex;

	vertexBuffer->Release();

	bd.ByteWidth = sizeof(VERTEX_3D) * vertex.size(); //頂点最大数をvectorの要素数に
	GetDevice()->CreateBuffer(&bd, NULL, &vertexBuffer);
}