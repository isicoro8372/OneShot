#include "light.h"
#include "texture.h"

#include "billboard.h"

static ID3D11Buffer* g_VertexBuffer = NULL;

static const int BILLBOARD_VERTEX_NUM = 4;

const VERTEX_3D shadowVertet[] = 
{
	{ {-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f,-1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} },
	{ { 0.5f, 0.5f, 0.0f}, {0.0f, 0.0f,-1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} },
	{ {-0.5f,-0.5f, 0.0f}, {0.0f, 0.0f,-1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f} },
	{ { 0.5f,-0.5f, 0.0f}, {0.0f, 0.0f,-1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f} },
};

void InitBillBoard()
{
	ID3D11Device* pDevice = GetDevice();

	//頂点取得
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = shadowVertet;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * BILLBOARD_VERTEX_NUM;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	GetDevice()->CreateBuffer(&bd, &data, &g_VertexBuffer);
}

void UpdateBillBoard()
{

}

void DrawBillBoard(const XMMATRIX& worldMatrix, const XMMATRIX& cameraViewMatrix, int textureID, const XMFLOAT2& texturePatternSize, const XMFLOAT3& scaleFloat3, int patternNo)
{
	//転置行列で逆行列に
	XMMATRIX billBoardMatrix = XMMatrixTranspose(cameraViewMatrix);
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, billBoardMatrix);
	matrix._14 = matrix._24 = matrix._34 = 0.0f; //平行移動成分をカット
	billBoardMatrix = XMLoadFloat4x4(&matrix);

	XMMATRIX scale = XMMatrixScalingFromVector(XMLoadFloat3(&scaleFloat3));

	//テクスチャ設定
	SetTexture(textureID);

	//UV設定
	//パターンの算出
	float texturePatternX = TextureGetWidth(textureID) / texturePatternSize.x;
	float texturePatternY = TextureGetHeight(textureID) / texturePatternSize.y;
	//テクスチャ座標の算出
	float texPt_U = (patternNo % ((int)texturePatternX) * TextureGetWidth(textureID));
	float texPt_V = (patternNo / ((int)texturePatternY) * TextureGetHeight(textureID));

	XMMATRIX uvScale = XMMatrixScalingFromVector({ texturePatternSize.x / TextureGetWidth(textureID), texturePatternSize.y / TextureGetHeight(textureID), 1.0f });
	XMMATRIX uvTranslation = XMMatrixTranslation(texPt_U, texPt_V, 0.0f);
	//XMMATRIX uvr = XMMatrixRotationZ(angle);
	SetUVMatrix(uvScale * uvTranslation);

	//マトリクス設定
	SetWorldMatrix(scale * billBoardMatrix * worldMatrix);

	SetShader2D();

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

	SetLight(billBoardMatrix);

	//ポリゴン描画
	GetDeviceContext()->Draw(BILLBOARD_VERTEX_NUM, 0);

	SetUVMatrix(XMMatrixIdentity());
}

void UninitBillBoard()
{
	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = nullptr;
	}
}