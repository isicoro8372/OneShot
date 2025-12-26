#include "main.h"
#include "renderer.h"
#include "trail.h"
#include "texture.h"
#include "camera.h"

#define TRAIL_LENGTH 30

static XMVECTOR g_Position[TRAIL_LENGTH]; //位置
static XMVECTOR g_CameraPosition = {};

static ID3D11Buffer* g_VertexBuffer = NULL;
static int g_Texture = -1;


void TrailInitialize(void)
{
	// 頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * TRAIL_LENGTH * 2;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);
	}

	// テクスチャ読込
	g_Texture = LoadTexture(L"asset\\texture\\blank.png");

}

void TrailFinalize(void)
{
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}
}

void TrailUpdate(void)
{

}

void TrailDraw(XMVECTOR cameraPosition, float width)
{
	g_CameraPosition = cameraPosition;

	SetShader2D();

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	GetDeviceContext()->Map(g_VertexBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	//ここで頂点データ入力
	for (int i = 0; i < TRAIL_LENGTH - 1; i++)
	{
		XMVECTOR cameraDirection, trailDirection, cross, position;

		//視線ベクトル
		cameraDirection = g_Position[i] - g_CameraPosition;
		//トレイルの方向ベクトル
		trailDirection = g_Position[i] - g_Position[i + 1];

		//外積
		cross = XMVector3Normalize(XMVector3Cross(cameraDirection, trailDirection));

		//上頂点
		position = g_Position[i] + cross * width;

		XMStoreFloat3(&vertex[i * 2].Position, position);
		vertex[i * 2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[i * 2].TexCoord = XMFLOAT2(0.5f, 0.0f);
		vertex[i * 2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

		//下頂点
		position = g_Position[i] - cross * width;

		XMStoreFloat3(&vertex[i * 2 + 1].Position, position);
		vertex[i * 2 + 1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[i * 2 + 1].TexCoord = XMFLOAT2(0.5f, 1.0f);
		vertex[i * 2 + 1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}

	XMStoreFloat3(&vertex[TRAIL_LENGTH * 2 - 2].Position, g_Position[TRAIL_LENGTH - 1]);
	vertex[TRAIL_LENGTH * 2 - 2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[TRAIL_LENGTH * 2 - 2].TexCoord = XMFLOAT2(0.5f, 0.0f);

	XMStoreFloat3(&vertex[TRAIL_LENGTH * 2 - 1].Position, g_Position[TRAIL_LENGTH - 1]);
	vertex[TRAIL_LENGTH * 2 - 1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[TRAIL_LENGTH * 2 - 1].TexCoord = XMFLOAT2(0.5f, 1.0f);

	GetDeviceContext()->Unmap(g_VertexBuffer, 0);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// テクスチャ設定
	SetTexture(g_Texture);

	// プリミティブトポロジ設定
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ワールド変換行列
	SetWorldMatrix(XMMatrixIdentity());

	// ポリゴン描画
	GetDeviceContext()->Draw(TRAIL_LENGTH * 2, 0);

}

void SetTrailTargetCamera(const XMVECTOR& position)
{
	g_CameraPosition = position;
}

void SetTrailPosition(const XMVECTOR& Position)
{
	for (int i = 0; i < TRAIL_LENGTH - 1; i++)
	{
		g_Position[i] = g_Position[i + 1];
	}

	g_Position[TRAIL_LENGTH - 1] = Position;
}

void ResetTrailPosition(const XMVECTOR& Position)
{
	for (int i = 0; i < TRAIL_LENGTH; i++)
	{
		g_Position[i] = Position;
	}
}

