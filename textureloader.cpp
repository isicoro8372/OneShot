#include <string>
#include "main.h"
#include "renderer.h"
#include "debugprintf.h"
#include "textureloader.hpp"

//=========================================
//初期化
//=========================================

std::vector<std::string> TEXTURE::loadedTexturePath;
std::vector<unsigned int> TEXTURE::loadedTextureNumber;
std::vector<ID3D11ShaderResourceView*> TEXTURE::sprite;

void TextureLoader::InitializeLoader(std::string filePath, int xPatternCount, int yPatternCount)
{
	//パスの保存
	texture.filePath = filePath;

	//パターン数計算
	patternNo = xPatternCount * yPatternCount - 1;

	//何も読み込まれていない場合初期化
	if (texture.loadedTexturePath.empty())
	{
		texture.loadedTexturePath.resize(0);
		texture.loadedTextureNumber.resize(0);
		texture.sprite.resize(0);
	}

	LoadTexture(texture.filePath);
}

TextureLoader::~TextureLoader()
{
	texture.loadedTextureNumber[texture.textureID]--;

	//テクスチャが使われていないなら解放
	if (texture.loadedTextureNumber[texture.textureID] == 0)
	{
		//管理用配列の要素削除
		texture.loadedTextureNumber.erase(texture.loadedTextureNumber.begin() + texture.textureID);
		texture.loadedTexturePath.erase(texture.loadedTexturePath.begin() + texture.textureID);

		//テクスチャの開放
		texture.sprite[texture.textureID]->Release();
		texture.sprite.erase(texture.sprite.begin() + texture.textureID);

		DebugPrintf("UnloadTexture %d, Remain %dtype\n", texture.textureID, texture.loadedTextureNumber.size());
	}
}

//=========================================
//テクスチャの読み込み
//=========================================
void TextureLoader::LoadTexture(std::string filePath)
{
	//同じ画像が読み込まれているか探す
	for (int i = 0; i < texture.loadedTexturePath.size(); i++)
	{
		if (texture.filePath == texture.loadedTexturePath[i])
		{
			texture.textureID = i;
			texture.loadedTextureNumber[i]++;
			return;
		}
	}	

	//読み込まれていなかった場合NULL(未設定)ではないなら
	if (texture.filePath != "NULL")
	{
		TexMetadata metadata;
		ScratchImage image;
		//wcahr_tへの型変換
		std::wstring convertwstr = std::wstring(filePath.begin(), filePath.end());
		const wchar_t* filename = convertwstr.c_str();

		//ファイルの読み込み
		LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, image);

		//サイズの変更
		//texture.sprite.resize(texture.sprite.size() + 1);
		//読み込んだ画像データをDirextXに渡して管理させる
		ID3D11ShaderResourceView* resoureBuffer;
		CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &resoureBuffer);
		texture.sprite.push_back(resoureBuffer);

		//管理用配列に追加
		texture.textureID = texture.loadedTextureNumber.size();
		texture.loadedTexturePath.push_back(texture.filePath);
		texture.loadedTextureNumber.push_back(1);

		//失敗時の警告
		assert(resoureBuffer);
	}
}

//=========================================
//テクスチャの再設定
//=========================================
void TextureLoader::ReloadTexture(std::string filePath)
{
	texture.filePath = filePath;
	texture.sprite[texture.textureID]->Release();

	LoadTexture(texture.filePath);
}

//=========================================
//テクスチャの取得
//=========================================
ID3D11ShaderResourceView* TextureLoader::GetSprite()
{
	if (texture.filePath != "NULL")
	{
		return texture.sprite[texture.textureID];
	}

	else return nullptr;
}

/*----------------------------------------------------------------------------------------------------------------------------*/

//=========================================
//旧式読み込み
//=========================================
void TextureLoaderOld(std::vector<ID3D11ShaderResourceView*>& texture, const std::vector<std::string> filePath)
{
	size_t size = filePath.size();
	//テクスチャポインタの可変長配列の長さ設定
	texture.resize(size);

	for (int i = 0; i < size; i++)
	{
		//テクスチャの読み込み
		//Windows Image Componentで対応する画像を読み込む
		TexMetadata metadata;
		ScratchImage image;
		std::wstring convertwstr = std::wstring(filePath[i].begin(), filePath[i].end());
		const wchar_t* filename = convertwstr.c_str();
		LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, image);

		//読み込んだ画像データをDirextXに渡して管理させる
		CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture[i]);

		//失敗時の警告
		assert(&texture[i]);
	}
}

void TextureUnloaderOld(std::vector<ID3D11ShaderResourceView*>& texture)
{
	for (int i = 0; i < texture.size(); i++)
	{
		texture[i]->Release();
	}
}