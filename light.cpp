#include "light.h"

struct LIGHT
{
	XMFLOAT4 localLightVector;
	XMFLOAT4 lightColor;
	XMFLOAT4 ambientColor;
};

static XMVECTOR g_WorldLightVector;
static ID3D11Buffer* g_LightBuffer = NULL;
static LIGHT g_Light;

void InitLight()
{
	{
		D3D11_BUFFER_DESC hBufferDesc;
		hBufferDesc.ByteWidth = sizeof(LIGHT);
		hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		hBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		hBufferDesc.CPUAccessFlags = 0;
		hBufferDesc.MiscFlags = 0;
		hBufferDesc.StructureByteStride = sizeof(float);

		GetDevice()->CreateBuffer(&hBufferDesc, NULL, &g_LightBuffer);
		GetDeviceContext()->PSSetConstantBuffers(3, 1, &g_LightBuffer);
	}
}

void UpdateLight()
{

}

void DrawLight()
{

}

void UninitLight()
{
	if (g_LightBuffer) { g_LightBuffer->Release(); }
}

void SetDirectionalLight(const XMVECTOR& dir, const XMFLOAT4 color)
{
	g_WorldLightVector = XMVector3Normalize(dir);
	g_Light.lightColor = color;
}

void SetAmbient(const XMFLOAT4& color)
{
	g_Light.ambientColor = color;
}

void SetLight(const XMMATRIX& world)
{
	XMVECTOR localLightVectorFloat4 = XMVector3TransformNormal(g_WorldLightVector, XMMatrixInverse(nullptr, world));
	XMStoreFloat4(&g_Light.localLightVector, XMVector3Normalize(localLightVectorFloat4));
	GetDeviceContext()->UpdateSubresource(g_LightBuffer, 0, NULL, &g_Light.localLightVector, 0, 0);
}