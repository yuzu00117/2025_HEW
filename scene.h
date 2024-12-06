#ifndef SCENE_H
#define SCENE_H


HRESULT InitScene(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void InitScene();
void UninitScene();
void UpdateScene();
void DrawScene();

enum GAME_SCENE
{
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_GAMESELECT,
	SCENE_GAME,
	SCENE_GAMEOVER,
	SCENE_RESULT,
};


void ChangeScene(GAME_SCENE scene);

#endif //SCENE_H