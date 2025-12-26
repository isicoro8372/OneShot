#ifndef SPRITE_H_
#define SPRITE_H_

#define CIRCLE_ANCHOR_MULTIPLIER (0.1f)

//==============================================
// プロトタイプ宣言
//==============================================
HRESULT InitSprite();
void UninitSprite();

void DrawSquare(XMFLOAT3 pos, XMFLOAT2 size, XMFLOAT4 color, bool scroll = false);
void DrawSquareRotate(XMFLOAT3 position, XMFLOAT2 size, XMFLOAT4 color, FLOAT radian);
void DrawSquareRotateUV(XMFLOAT3 position, XMFLOAT2 size, XMFLOAT4 color, FLOAT radian, int patternNo, int xPatternCount, int yPatternCount, bool reverseTexture = false);
void DrawTriangle(XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT4 color);

//行列
void DrawSquare(XMFLOAT2 size, XMFLOAT4 color, XMMATRIX worldMatrix, int patternNo = 0, int xPatternCount = 0, int yPatternCount = 0, bool reverseTexture = false);

std::vector<VERTEX_3D> CalcCircle(XMFLOAT3 pos, float rad, XMFLOAT4 color);
std::vector<VERTEX_3D> CalcSquare(XMFLOAT3 pos, XMFLOAT2 size, XMFLOAT4 color);

#endif // !SPRITE_H_