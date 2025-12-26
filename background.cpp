#include <string>
#include "main.h"
#include "renderer.h"
#include "sprite.h"
#include "background.h"
#include "textureloader.hpp"

static std::vector<ID3D11ShaderResourceView*> g_Texture;

//ファイルパス
const std::vector<std::string> filePath =
{
	"asset\\texture\\PuzzleGame\\BG.png",
};

void InitBackground()
{
	//TextureLoaderOld(g_Texture, filePath);
}
void UninitBackground()
{
	//TextureUnloaderOld(g_Texture);
}
void UpdateBackground()
{

}
void DrawBackground()
{
	//SetColor(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	//XMFLOAT3 position = XMFLOAT3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0);
	//XMFLOAT2 size = XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT);
	//XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

	//DrawSquare(position, size, color);
}