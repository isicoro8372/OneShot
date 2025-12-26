#include "texture.h"
#include "golf_course.h"
#include "cube.hpp"

void DrawMap(int textureID)
{
	for (int i = 0; i < sizeof(g_Map) / sizeof(g_Map[0]); i++)
	{
		XMMATRIX translation = XMMatrixTranslation(g_Map[i].x, g_Map[i].y, g_Map[i].z);
		DrawCube(translation, textureID);
	}
}
