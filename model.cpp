#define NOMINMAX

#include "main.h"
#include "renderer.h"
#include "model.h"

#include <string>

static int g_TextureWhite = -1;

MODEL* ModelLoad( const char *FileName, bool blenderFlag )
{
	MODEL* model = new MODEL(blenderFlag);

	const std::string modelPath( FileName );

	model->AiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded | aiProcess_GenBoundingBoxes);
	//MessageBox(NULL, aiGetErrorString(), NULL, NULL);
	assert(model->AiScene);

	model->VertexBuffer = new ID3D11Buffer*[model->AiScene->mNumMeshes];
	model->IndexBuffer = new ID3D11Buffer*[model->AiScene->mNumMeshes];

	XMFLOAT3 minSurface, maxSurface;

	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = model->AiScene->mMeshes[m];

		// 頂点バッファ生成
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				if (blenderFlag)
				{
					//blender
					vertex[v].Position = XMFLOAT3(mesh->mVertices[v].x, -mesh->mVertices[v].z, mesh->mVertices[v].y);
				}
				else
				{
					//maya
					vertex[v].Position = XMFLOAT3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				}

				if (m == 0 && v == 0)
				{
					minSurface = maxSurface = vertex[v].Position;
				}
				else
				{
					minSurface.x = std::min(vertex[v].Position.x, minSurface.x);
					minSurface.y = std::min(vertex[v].Position.y, minSurface.y);
					minSurface.z = std::min(vertex[v].Position.z, minSurface.z);

					maxSurface.x = std::max(vertex[v].Position.x, maxSurface.x);
					maxSurface.y = std::max(vertex[v].Position.y, maxSurface.y);
					maxSurface.z = std::max(vertex[v].Position.z, maxSurface.z);
				}

				vertex[v].TexCoord = XMFLOAT2( mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				vertex[v].Normal = XMFLOAT3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
			}

			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			GetDevice()->CreateBuffer(&bd, &sd, &model->VertexBuffer[m]);

			delete[] vertex;
		}

		//model->aabb = { minSurface, maxSurface };

		// インデックスバッファ生成
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				//assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd = {};
			sd.pSysMem = index;

			GetDevice()->CreateBuffer(&bd, &sd, &model->IndexBuffer[m]);

			delete[] index;
		}

	}

	model->aabb = { minSurface, maxSurface };

	if (!model->AiScene->mNumTextures)
	{
		g_TextureWhite = LoadTexture(L"asset/texture/blank.png");
	}


	//テクスチャ読み込み
	for(int i = 0; i < model->AiScene->mNumTextures; i++)
	{
		aiTexture* aitexture = model->AiScene->mTextures[i];

		ID3D11ShaderResourceView* texture;
		TexMetadata metadata;
		ScratchImage image;
		LoadFromWICMemory(aitexture->pcData, aitexture->mWidth, WIC_FLAGS_NONE, &metadata, image);
		CreateShaderResourceView(GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &texture);
		assert(texture);

		model->Texture[aitexture->mFilename.data] = texture;
	}

	return model;
}

void ModelDraw(MODEL* model, const XMMATRIX& world, std::wstring forceTexture)
{
	SetWorldMatrix(world);

	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		if (forceTexture != L"null")
		{
			SetTexture(LoadTexture(forceTexture));
		}
		else if (model->AiScene->mNumTextures)
		{
			//テクスチャ設定
			aiString texture;
			aiMaterial* aimaterial = model->AiScene->mMaterials[model->AiScene->mMeshes[m]->mMaterialIndex];
			aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texture);

			if (texture != aiString(""))
			{
				GetDeviceContext()->PSSetShaderResources(0, 1, &model->Texture[texture.data]);
			}
		}
		else
		{
			ID3D11ShaderResourceView* srv = GetTexture(g_TextureWhite);
			GetDeviceContext()->PSGetShaderResources(0, 1, &srv);
		}

		// 頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		GetDeviceContext()->IASetVertexBuffers(0, 1, &model->VertexBuffer[m], &stride, &offset);

		//インデックスバッファ設定
		GetDeviceContext()->IASetIndexBuffer(model->IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		//プリミティブトポロジ設定
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// マテリアル設定
		MATERIAL material = {};
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);

		aiMaterial* aimaterial = model->AiScene->mMaterials[model->AiScene->mMeshes[m]->mMaterialIndex];
		aiColor3D diffuse;
		aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
		material.Diffuse = { diffuse.r, diffuse.g, diffuse.b, 1.0f };

		SetLight(world);

		// ポリゴン描画
		GetDeviceContext()->DrawIndexed(model->AiScene->mMeshes[m]->mNumFaces * 3, 0, 0);
	}
}

void ModelRelease(MODEL* model)
{
	for (unsigned int m = 0; m < model->AiScene->mNumMeshes; m++)
	{
		model->VertexBuffer[m]->Release();
		model->IndexBuffer[m]->Release();
	}

	delete[] model->VertexBuffer;
	delete[] model->IndexBuffer;


	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : model->Texture)
	{
		pair.second->Release();
	}

	aiReleaseImport(model->AiScene);

	delete model;
}