#include <string>
#include "main.h"
#include "renderer.h"
#include "sprite.h"
#include "textureloader.hpp"
#include "scenemanager.h"
#include "fade.hpp"

FadeObject g_Fade;
static std::vector<ID3D11ShaderResourceView*> g_Texture;

//ファイルパス
const std::vector<std::string> filePath =
{
		"asset\\texture\\PuzzleGame\\Fade.bmp",
};

void InitFade()
{
	//TextureLoaderOld(g_Texture, filePath);

	//g_Fade.fadeColor = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//g_Fade.frame = 60;
	//g_Fade.state = FADE_NONE;
}

void UpdateFade()
{
	//switch (g_Fade.state)
	//{
	//case FADE_NONE:
	//	break;
	//case FADE_IN:
	//	g_Fade.fadeColor.w -= (1.0f / g_Fade.frame);

	//	if (g_Fade.fadeColor.w < 0.0f)
	//	{
	//		g_Fade.fadeColor.w = 0.0f;
	//		g_Fade.state = FADE_NONE;
	//	}
	//	break;
	//case FADE_OUT:
	//	g_Fade.fadeColor.w += (1.0f / g_Fade.frame);

	//	if (g_Fade.fadeColor.w > 1.0f)
	//	{
	//		g_Fade.fadeColor.w = 1.0f;
	//		g_Fade.state = FADE_NONE;
	//	}
	//	break;
	//}
}

void DrawFade()
{

	//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

	//XMMATRIX translationMatrix = XMMatrixTranslation(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	////回転行列
	//XMMATRIX rotationMatrix = XMMatrixRotationZ(XMConvertToRadians(0.0f));
	////スケーリング行列
	//XMMATRIX scalingMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	////ワールド行列
	//XMMATRIX worldMatrix = scalingMatrix * rotationMatrix * translationMatrix;

	//DrawSquare(XMFLOAT2{ SCREEN_WIDTH, SCREEN_HEIGHT }, g_Fade.fadeColor, worldMatrix);
}

void UninitFade()
{
	//TextureUnloaderOld(g_Texture);
}

void SetFade(int frame, XMFLOAT4 color, FADE_STATE state)
{
	//g_Fade.frame = frame;
	//g_Fade.fadeColor = color;
	//g_Fade.state = state;
}

FADE_STATE GetFadeState()
{
	return g_Fade.state;
}