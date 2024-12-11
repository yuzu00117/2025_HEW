#ifndef SCENE_H
#define SCENE_H

#include <windows.h>
#include"player.h"  

enum GAME_SCENE
{
	SCENE_NONE,
	SCENE_TITLE,
	SCENE_GAMESELECT,
	SCENE_GAME,
	SCENE_GAMEOVER,
	SCENE_RESULT,
};


HRESULT AllInitializeScene(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);

void InitializeScene();
void FinalizeScene();
void UpdateScene();
void DrawScene();

void ChangeScene(GAME_SCENE scene);




#endif //SCENE_H