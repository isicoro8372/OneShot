#ifndef CUSTOMPOLYGON_H_
#define CUSTOMPOLYGON_H_

#include <string>
#include "textureloader.hpp"

#define MAX_POLYGONS (5)

//=========================================
//CustomnPolygonクラス
//=========================================
class CustomPolygon
{
	std::vector<VERTEX_3D> vertex;
	D3D_PRIMITIVE_TOPOLOGY renderType;
	TextureLoader loaderClass;
	std::string filePath;
	//ID3D11ShaderResourceView* texture;

	int index;

	ID3D11Buffer* vertexBuffer = NULL;
	D3D11_BUFFER_DESC bd;

public:
	void Draw();
	//CustomPolygon(D3D_PRIMITIVE_TOPOLOGY renderType, std::vector<VERTEX_3D> const vertex, int index, ID3D11ShaderResourceView* texture = NULL);
	CustomPolygon(D3D_PRIMITIVE_TOPOLOGY renderType, std::vector<VERTEX_3D> const vertex, int index, std::string filePath = NULL);
	~CustomPolygon();
	void SetVertexVector(std::vector<VERTEX_3D> const vertex);
};

#endif // !COUSTOMPOLYGON_H_

//=========================================
//クラスコンテナ
//=========================================
static CustomPolygon* polygonClassContainer[MAX_POLYGONS]; //後々消す

//=========================================
//クラス初期化(includeしたファイルのInit関数に必ず入れる)
//=========================================
static void InitPolygonClassContainer()
{
	for (int i = 0; i < MAX_POLYGONS; i++)
	{
		polygonClassContainer[i] = NULL;
	}

}

//=========================================
//クラスインスタンス生成関数
//=========================================
//各cpp内で自由に入れられるようにしたい
static void GeneratePolygon(D3D_PRIMITIVE_TOPOLOGY renderType, std::vector<VERTEX_3D> const vertex, /*追加*/ std::string filePath = "NULL")
{
	for (int i = 0; i < MAX_POLYGONS; i++)
	{
		if (polygonClassContainer[i] == nullptr)
		{
			DebugPrintf("Generate ");
			//CustomPolygon* classbuf = new CustomPolygon(renderType, vertex, i);
			polygonClassContainer[i] = new CustomPolygon(renderType, vertex, i, filePath);

			break;
		}
	}
}

//static CustomPolygon GeneratePolygonB(D3D_PRIMITIVE_TOPOLOGY renderType, std::vector<VERTEX_3D> const vertex, std::string filePath = "NULL")
//{
//	DebugPrintf("Generate ");
//	return { renderType, vertex, filePath };
//}

//コンテナ要素番号指定
static void GeneratePolygon(D3D_PRIMITIVE_TOPOLOGY renderType, std::vector<VERTEX_3D> const vertex, int index, /*追加*/ std::string filePath = NULL)
{
	polygonClassContainer[index] = new CustomPolygon(renderType, vertex, index, filePath);
}

//=========================================
//クラスインスタンス破棄関数
//=========================================
static void DestroyPolygon(int index)
{
	delete polygonClassContainer[index];
	polygonClassContainer[index] = nullptr;
}

static void DestroyPolygon(CustomPolygon* pointer)
{
	delete pointer;
	pointer = nullptr;
}
