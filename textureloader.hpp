#ifndef TEXTURELOADER_H_
#define TEXTURELOADER_H_

#include <string>

struct TEXTURE
{
	std::string filePath;
	size_t textureID;
	static std::vector<ID3D11ShaderResourceView*> sprite;
	static std::vector<std::string> loadedTexturePath;
	static std::vector<unsigned int> loadedTextureNumber;

	TEXTURE() { filePath = "NULL", textureID = NULL; }
};

class TextureLoader
{
	TEXTURE texture;
	int patternNo, xPatternCount, yPatternCount;

public:
	~TextureLoader();

	void InitializeLoader(std::string filePath, int xPatternCount = 0, int yPatternCount = 0);
	void LoadTexture(std::string filePath);
	void ReloadTexture(std::string filePath);
	ID3D11ShaderResourceView* GetSprite();

};

//ãåéÆÉçÅ[Éh
void TextureLoaderOld(std::vector<ID3D11ShaderResourceView*>& p_Texture, const std::vector<std::string> filePath);
void TextureUnloaderOld(std::vector<ID3D11ShaderResourceView*>& texture);

#endif // !TEXTURELOADER_H_
