/*==============================================================================

   テクスチャ管理 [texture.h]
														 Author : Youhei Sato
														 Date   : 2024/06/04
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <d3d11.h>


void InitTexture();
int LoadTexture(const std::wstring& texture_filename);
void SetTexture(int textureID);
ID3D11ShaderResourceView* GetTexture(int id);
int TextureGetWidth(int id);
int TextureGetHeight(int id);
void UninitTexture(void);

#endif // TEXTURE_H
