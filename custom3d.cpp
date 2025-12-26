/*==============================================================================

   カスタム3D図形描画 [custom3d.cpp]
														 Author : 桜井優輝
														 Date   : 25/01/21
--------------------------------------------------------------------------------

==============================================================================*/
#include "main.h"
#include "renderer.h"
#include "custom3d.h"
#include "light.h"
#include "texture.h"

static ID3D11Buffer* g_VertexBuffer = NULL;

static const VERTEX_3D testvertex[] =
{
	{ XMFLOAT3{ -0.5f, 0.0f, 0.0f }, XMFLOAT3{ 0.0f, 0.0f, -1.0f }, XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f }, XMFLOAT2{ 0.0f, 0.0f } },
	{ XMFLOAT3{ 0.0f, 1.0f, 0.0f}, XMFLOAT3{0.0f, 0.0f, -1.0f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.25f, 0.0f} },
	{ XMFLOAT3{ 0.5f, 0.0f, 0.0f}, XMFLOAT3{0.0f, 0.0f, -1.0f}, XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, XMFLOAT2{0.0f, 0.25f} }
};

void InitCustom3D()
{

}

void UninitCustom3D()
{
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = nullptr;
	}
}

void DrawCustom3D(const XMMATRIX& world, int textureID, const std::vector<VERTEX_3D> vertex, int patternNo, int xPatternCount, int yPatternCount)
{
	SetTexture(textureID);

	SetWorldMatrix(world);

	SetShader3D();

	//頂点取得
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertex.data();
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * vertex.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	GetDevice()->CreateBuffer(&bd, &data, &g_VertexBuffer);

	//頂点バッファ
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//プリミティブトポロジ
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアル設定
	MATERIAL material = {};
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	SetLight(world);

	//ポリゴン描画
	GetDeviceContext()->Draw((sizeof(VERTEX_3D) * vertex.size()), 0);
}


#include <DirectXMath.h>

struct Ray
{
	XMVECTOR origin;
	XMVECTOR direction;
};

struct Triangle
{
	XMVECTOR v0;
	XMVECTOR v1;
	XMVECTOR v2;
};

//bool intersectRayTriangle(const Ray& ray, const Triangle& triangle, XMVECTOR& hitposition)
//{
//	//三角形の辺の計算
//	XMVECTOR edge1 = XMVectorSubtract(triangle.v1, triangle.v0);
//	XMVECTOR edge2 = XMVectorSubtract(triangle.v2, triangle.v0);
//
//	//Rayの方向と三角形の法線の外積を計算
//	XMVECTOR h = XMVector3Cross(ray.direction, edge2);
//	float a = XMVectorGetX(XMVector3Dot(edge1, h));
//
//	//Rayが三角形と平行でないかを確認
//	if (fabs(a) < le - 6)
//	{
//		return false;
//	}
//
//	float f = 1.0f / a;
//	XMVECTOR s = XMVectorSubtract(ray.origin, triangle.v0);
//	float u = f * XMVectorGetX(XMVector3Dot(s, h));
//
//	//パラメータが範囲外か確認
//	if (u < 0.0f || u > 1.0f)
//	{
//		return false;
//	}
//
//	XMVECTOR q = XMVector3Cross(s, edge1);
//	float v = f * XMVectorGetX(XMVector3Dot(ray.direction, q));
//
//	//パラメータが範囲外か確認
//	if (v < 0.0f || u + v > 1.0f)
//	{
//		return false;
//	}
//
//	//交差点の計算
//	hitposition = XMVectorAdd(ray.origin, XMVectorScale(ray.direction, t))
//		return true;
//}
//
//void testRay()
//{
//
//	Ray ray;
//	ray.origin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
//	ray.direction = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//
//	Triangle triangle;
//	triangle.v0 = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
//	triangle.v1 = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
//	triangle.v2 = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
//
//	XMVECTOR intersectionPoint;
//	if (intersectRayTriangle(ray, triangle, intersectionPoint))
//	{
//		XMFLOAT3 point;
//		XMStoreFloat3(&point, intersectionPoint);
//	}
//}