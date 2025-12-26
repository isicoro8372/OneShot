#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

enum SCENE
{
	SCENE_NONE = 0,
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_RESULT,
	SCENE_MAX
};

void InitManager();
void UninitManager(void);
void UpdateManager(void);
void DrawManager(void);

void SetScene(SCENE Scene);
SCENE GetScene();
#endif //!SCENEMANAGER_H_