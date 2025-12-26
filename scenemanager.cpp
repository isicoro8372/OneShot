#include "main.h"
#include "scenemanager.h"
#include "fade.hpp"
#include "title.h"
#include "game.h"
#include "background.h"
#include "audio.h"

static SCENE g_Scene = SCENE_NONE;
static SCENE targetScene = SCENE_NONE;
static bool changeScene = false;

static int g_Bgm;

void InitManager()
{	
	g_Bgm = LoadAudio("asset/audio/bgm.wav");
	PlayAudio(g_Bgm, true);
	SetScene(SCENE_TITLE);
}

void UninitManager() 
{
	SetScene(SCENE_NONE);
}

void UpdateManager() 
{
	switch (g_Scene)
	{
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		UpdateTitle();
		break;
	case SCENE_GAME:
		UpdateGame();
		break;
	}

}

void DrawManager(void) 
{
	switch (g_Scene)
	{
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		DrawTitle();
		break;
	case SCENE_GAME:
		DrawGame();
		break;
	}
}


void SetScene(SCENE Scene) 
{
	targetScene = Scene;
	changeScene = true;
	//if (Scene == SCENE_NONE) { g_Scene = SCENE_EXIT; }

	if (GetFadeState() == FADE_NONE)
	{
		switch (g_Scene)
		{
		case SCENE_NONE:
			break;
		case SCENE_TITLE:
			UninitTitle();
			break;
		case SCENE_GAME:
			UninitGame();
			break;
		default:
			break;
		}

		g_Scene = Scene;

		switch (g_Scene)
		{
		case SCENE_NONE:
			break;
		case SCENE_TITLE:
			InitTitle();
			break;
		case SCENE_GAME:
			InitGame();
			break;
		default:
			break;
		}

		changeScene = false;
	}
}

SCENE GetScene() { return g_Scene; }