//無理.cpp
#include "map.h"
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "shadow.h"

static ID3D11Buffer* g_VertexBuffer = NULL;
static XMVECTOR g_Position{};
static int g_TextureID = 0;
static float g_ShadowVectorY = 0.0f;


static const int SHADOW_VERTEX_NUM = 4;

const VERTEX_3D shadowVertex[] =
{
	{ {-0.5f, 0.0f, 0.5f}, {0.0f, 0.0f,-1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
	{ { 0.5f, 0.0f, 0.5f}, {0.0f, 0.0f,-1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
	{ {-0.5f, 0.0f,-0.5f}, {0.0f, 0.0f,-1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },
	{ { 0.5f, 0.0f,-0.5f}, {0.0f, 0.0f,-1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
};

void InitShadow()
{
	ID3D11Device* pDevice = GetDevice();

	//頂点取得
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = shadowVertex;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * SHADOW_VERTEX_NUM;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	GetDevice()->CreateBuffer(&bd, &data, &g_VertexBuffer);

	g_TextureID = LoadTexture(L"asset/texture/shadow.png");
}

void UpdateShadow()
{

}

void DrawShadow()
{
	SetTexture(g_TextureID);
	SetShader2D();

	XMMATRIX translate = XMMatrixTranslationFromVector(g_Position);
	SetWorldMatrix(translate);

	SetUVMatrix(XMMatrixIdentity());

	//頂点バッファ
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	//プリミティブトポロジ
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//マテリアル設定
	MATERIAL material = {};
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//ポリゴン描画
	GetDeviceContext()->Draw(SHADOW_VERTEX_NUM, 0);


	////転置行列で逆行列に
	//XMMATRIX billBoardMatrix = XMMatrixTranspose(cameraViewMatrix);
	//XMFLOAT4X4 matrix;
	//XMStoreFloat4x4(&matrix, billBoardMatrix);
	//matrix._14 = matrix._24 = matrix._34 = 0.0f; //平行移動成分をカット
	//billBoardMatrix = XMLoadFloat4x4(&matrix);

	//XMMATRIX scale = XMMatrixScalingFromVector(XMLoadFloat3(&scaleFloat3));

	////テクスチャ設定
	//SetTexture(textureID);

	////UV設定
	////パターンの算出
	//float texturePatternX = TextureGetWidth(textureID) / texturePatternSize.x;
	//float texturePatternY = TextureGetHeight(textureID) / texturePatternSize.y;
	////テクスチャ座標の算出
	//float texPt_U = (patternNo % ((int)texturePatternX) * TextureGetWidth(textureID));
	//float texPt_V = (patternNo / ((int)texturePatternY) * TextureGetHeight(textureID));

	//XMMATRIX uvScale = XMMatrixScalingFromVector({ texturePatternSize.x / TextureGetWidth(textureID), texturePatternSize.y / TextureGetHeight(textureID), 1.0f });
	//XMMATRIX uvTranslation = XMMatrixTranslation(texPt_U, texPt_V, 0.0f);
	////XMMATRIX uvr = XMMatrixRotationZ(angle);
	//SetUVMatrix(uvScale * uvTranslation);

	////マトリクス設定
	//SetWorldMatrix(scale * billBoardMatrix * worldMatrix);

	//SetShader2D();

	////頂点バッファ
}

void UninitShadow()
{
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = nullptr;
	}
}

void SetShadowPosition(const XMVECTOR& position, Map* map)
{
	g_Position = position;

	float cubeRadius = 0.5f;
	float shadowY = -100.0f;

	if (std::round(XMVectorGetZ(position)) > -1 && std::round(XMVectorGetX(position)) > -1)
	{
		if (map->GetMapTile(std::round(XMVectorGetZ(position)))[(int)std::round(XMVectorGetX(position))] > 0)
		{
			float y = map->GetMapHeight(std::round(XMVectorGetZ(position)))[(int)std::round(XMVectorGetX(position))] + 0.501f;

			if (y > shadowY)
			{
				shadowY = y;
			}
		}
	}

	g_ShadowVectorY = XMVectorGetY(g_Position) - shadowY;

	g_Position = XMVectorSetY(g_Position, shadowY);
}
