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

//シーンの切り替えのスイッチ
static int g_Scene;

//コントローラ
DirectInputController controller;

//初期化処理おおもとです
HRESULT AllInitializeScene(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{

    //レンダリング処理の初期化
	InitRenderer(hInstance, hWnd, bWindow);

	//ポリゴン
	InitSprite();

	//文字（絵）
	InitializeWord();

	//コントローラーの初期化
	controller.Initialize(hInstance, hWnd);


#ifndef _DEBUG
	InitializeDebug();
#endif //_DEBUG

	return S_OK;

}

//初期化処理
void InitializeScene()
{
	g_Scene = SCENE_NONE;

	ChangeScene(SCENE_TITLE);
}


//終了処理
void FinalizeScene()
{

	Game& game = Game::GetInstance();



	//全部のUninitまたはfinalizeを終了処理を入れるか



	//ポリゴン終了処理
	UninitSprite();

	//コントローラーの終了処理
	controller.Release();



	game.Finalize();//最後に呼び出すファイル

#ifndef _DEBUG
	FinalizeDebug();
#endif //_DEBUG

}

//更新処理
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

//描画処理
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
		FinalizeTitle();
		break;
	case SCENE_GAMESELECT:
		FinalizeGameSelect();
		break;
	case SCENE_GAME:
		//オブジェクトが消されたら追加する方針
		break;
	case SCENE_GAMEOVER:
		FinalizeGameOver();
		break;
	case SCENE_RESULT:
		FinalizeResult();
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
		InitializeTitle();
		break;
	case SCENE_GAMESELECT:
		InitializeGameSelect();
		break;
	case SCENE_GAME:
		game.Initialize();
		break;
	case SCENE_GAMEOVER:
		InitializeGameOver();
		break;
	case SCENE_RESULT:
		InitializeResult();
		break;
	default:
		break;

	}
}