/*==============================================================================

   レンダリング管理[renderer.h]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef RENDERER_H_
#define RENDERER_H_

//ブレンドステート定義
enum BLENDSTATE
{
	BLENDSTATE_NONE = 0,
	BLENDSTATE_ALPHA,
	BLENDSTATE_ADD,
	BLENDSTATE_SUB,

	BLENDSTATE_MAX,
};

//*********************************************************
// 構造体
//*********************************************************

// 頂点構造体
struct VERTEX_3D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};


// マテリアル構造体
struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	float		Dummy[3];//16byte境界用
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitRenderer(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void UninitRenderer(void);

void Clear(float r, float g, float b);
void Present(void);

ID3D11Device* GetDevice(void);
ID3D11DeviceContext* GetDeviceContext(void);

XMVECTOR GetWorldLightVector();
void SetWorldLightVector(const XMVECTOR& newWorldLightVector);
XMFLOAT4 GetLocalLightVector();
void SetLocalLightVector(XMFLOAT4& newVector);

void SetShader2D();
void SetShader3D();

void SetDepthEnable(bool Enable);

void SetWorldViewProjection2D(void);
void SetWorldViewProjection3D(const XMMATRIX& world);
void SetWorldMatrix(XMMATRIX WorldMatrix);
void SetViewMatrix(XMMATRIX ViewMatrix);
void SetProjectionMatrix(XMMATRIX ProjectionMatrix);

void SetViewPort(FLOAT left, FLOAT top, FLOAT width, FLOAT height);

void SetMaterial(MATERIAL Material);
void SetColor(XMFLOAT4 color);
void SetUVMatrix(XMMATRIX uvMatrix);
//void SetLight(XMMATRIX worldMatrix);

void SetBlendState(BLENDSTATE blendstate);
#endif // !RENDERER_H_