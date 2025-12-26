#ifndef FADE_H_
#define FADE_H_

enum FADE_STATE
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT,

	FADE_MAX
};

struct FadeObject
{
	FADE_STATE state;
	float count;
	int frame;
	XMFLOAT4 fadeColor;
};

void InitFade();
void UpdateFade();
void DrawFade();
void UninitFade();

void SetFade(int frame, XMFLOAT4 color, FADE_STATE state);

FADE_STATE GetFadeState();
#endif // !FADE_H_
