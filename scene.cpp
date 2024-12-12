#include "main.h"
#include "scene.h"
#include "title.h"
#include "gameselect.h"
#include "game.h"
#include "gameover.h"
#include "result.h"
#include "renderer.h"
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
	//ゲーム画面の初期化
	g_Scene = SCENE_NONE;

	//最初に表示する場所
	ChangeScene(SCENE_TITLE);
}


//終了処理
void FinalizeScene()
{
	//ゲームからインスタンスをとってファイナライズに呼んでいる
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
	case SCENE_TITLE://タイトル
		UpdateTitle();
		break;
	case SCENE_GAMESELECT://ステージセレクト
		UpdateGameSelect();
		break;
	case SCENE_GAME://ゲーム
		game.Update();
		break;
	case SCENE_GAMEOVER://ゲームオーバー
		UpdateGameOver();
		break;
	case SCENE_RESULT://リザルト
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
	case SCENE_TITLE://タイトル
		DrawTitle();
		break;
	case SCENE_GAMESELECT://ステージセレクト
		DrawGameSelect();
		break;
	case SCENE_GAME://ゲーム
		game.Draw();
		break;
	case SCENE_GAMEOVER://ゲームオーバー
		DrawGameOver();
		break;
	case SCENE_RESULT://リザルト
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
	case SCENE_TITLE://タイトル
		FinalizeTitle();
		break;
	case SCENE_GAMESELECT://ゲームセレクト
		FinalizeGameSelect();
		break;
	case SCENE_GAME://ゲーム
		//オブジェクトが消されたら追加する方針
		break;
	case SCENE_GAMEOVER://ゲームオーバー
		FinalizeGameOver();
		break;
	case SCENE_RESULT://リザルト
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
	case SCENE_TITLE://タイトル
		InitializeTitle();
		break;
	case SCENE_GAMESELECT://ゲームセレクト
		InitializeGameSelect();
		break;
	case SCENE_GAME://ゲーム
		game.Initialize();
		break;
	case SCENE_GAMEOVER://ゲームオーバー
		InitializeGameOver();
		break;
	case SCENE_RESULT://リザルト
		InitializeResult();
		break;
	default:
		break;

	}
}