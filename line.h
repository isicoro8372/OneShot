#ifndef LINE_H_
#define LINE_H_

//==============================================
// プロトタイプ宣言
//==============================================
HRESULT InitLine();
void UninitLine();

void DrawLine(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT4 color);

#endif // !LINE_H_