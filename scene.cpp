#include "main.h"
#include "scene.h"
#include "title.h"
#include "gameselect.h"
#include "game.h"
#include "gameover.h"
#include "result.h"
#include "renderer.h"
#include "sound.h"
#include "directx_controller.h"
#include "word.h"
#include "debug.h"


#include "sprite.h"


static int g_Scene;

DirectInputController controller;

HRESULT InitScene(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//レンダリング処理の初期化
	InitRenderer(hInstance, hWnd, bWindow);

	//ポリゴン
	InitSprite();

	//文字（絵）
	InitializeWord();

	//コントローラーの初期化
	controller.Initialize(hInstance, hWnd);

#ifdef _DEBUG
	//デバッグ文字
	InitializeDebug();
#endif // _DEBUG

	return S_OK;
}

void InitScene()
{
	g_Scene = SCENE_NONE;

	ChangeScene(SCENE_TITLE);

}


void UninitScene()
{

	ChangeScene(SCENE_NONE);


	//ポリゴン
	UninitSprite();

	//コントローラーの終了処理
	controller.Release();


}


void UpdateScene()
{

	Game& game = Game::GetInstance();
	switch (g_Scene)
	{
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		UpdateTitle();
		break;
	case SCENE_GAMESELECT:
		UpdateGameSelect();
		break;
	case SCENE_GAME:
		game.Update();
		break;
	case SCENE_GAMEOVER:
		UpdateGameOver();
		break;
	case SCENE_RESULT:
		UpdateResult();
		break;
	default:
		break;

	}

	controller.CheckInput();
}


void DrawScene()
{
	Game& game = Game::GetInstance();
	Clear();

	SetDepthEnable(false);

	switch (g_Scene)
	{
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		DrawTitle();
		break;
	case SCENE_GAMESELECT:
		DrawGameSelect();
		break;
	case SCENE_GAME:
		game.Draw();
		break;
	case SCENE_GAMEOVER:
		DrawGameOver();
		break;
	case SCENE_RESULT:
		DrawResult();
		break;
	default:
		break;

	}

	//バックバッファ、フロントバッファ入れ替え
	Present();

}





void ChangeScene(GAME_SCENE scene)
{
	Game& game = Game::GetInstance();
	switch (g_Scene)
	{
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		UninitTitle();
		break;
	case SCENE_GAMESELECT:
		UninitGameSelect();
		break;
	case SCENE_GAME:
		game.Finalize();
		break;
	case SCENE_GAMEOVER:
		UninitGameOver();
		break;
	case SCENE_RESULT:
		UninitResult();
		break;
	default:
		break;

	}

	g_Scene = scene;
	switch (g_Scene)
	{
	case SCENE_NONE:
		break;
	case SCENE_TITLE:
		InitTitle();
		break;
	case SCENE_GAMESELECT:
		InitGameSelect();
		break;
	case SCENE_GAME:
		game.Initialize();
		break;
	case SCENE_GAMEOVER:
		InitGameOver();
		break;
	case SCENE_RESULT:
		InitResult();
		break;
	default:
		break;

	}
}