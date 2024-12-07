//-----------------------------------------------------------------------------------------------------
// #name game.cpp
// #description game
// #make 2024/11/02　　永野義也
// #update 2024/11/02
// #comment 追加・修正予定
//          ・ここで初期化、更新、描画、終了処理を管理している
// 　　　　 ・ここに直で値ぶち込んでテストしてるから、テスト終わったら消すよ！
//          
//----------------------------------------------------------------------------------------------------

#include <Windows.h>
#include "renderer.h"
#include "sprite.h"
#include "keyboard.h"
#include "sound.h"
#include"include/box2d/box2d.h"
#include"directx_controller.h"
#include"game.h"
#include"contactlist.h"
#include"anchor.h"
#include"word.h"
#include"debug.h"




HRESULT Game::Initialize(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//レンダリング処理の初期化
	InitRenderer(hInstance, hWnd, bWindow);

	//サウンドの初期化
	InitSound(hWnd);

	//ポリゴン
	InitSprite();

	//文字（絵）
	InitializeWord();

	//コントローラーの初期化
	controller.Initialize(hInstance,hWnd);

	//プレイヤーの初期化
	player.Initialize();

	//アンカーの初期化
	Anchor::Initialize();

	//フィールドの初期化
	Field::Initialize();

	//体力ソウルゲージUIの初期化
	stamina_spirit_gauge.Initialize();

	b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
	// 衝突リスナーをワールドに登録
	MyContactListener& contactListener = MyContactListener::GetInstance();
	world->SetContactListener(&contactListener);



#ifndef _DEBUG
	//デバッグ文字
	InitializeDebug();
#endif // !_DEBUG

	return S_OK;
}

void Game::Finalize(void)
{
	//ポリゴン
	UninitSprite();

	//コントローラーの終了処理
	controller.Release();

	//サウンドの終了処理
	UninitSound();


	//プレイヤーの終了処理
	player.Finalize();

	//アンカー終了処理
	Anchor::Finalize();

	//フィールドの終了処理
	Field::Finalize();

	//文字（絵）
	FinalizeWord();

	//体力ソウルゲージUIの終了処理
	stamina_spirit_gauge.Finalize();
	
	//レンダリングの終了処理
	UninitRenderer();


#ifdef _DEBUG
	//デバッグ文字
	FinalizeDebug();
#endif // _DEBUG

}



void Game::Update(void)
{

	// Box2D ワールドのステップ更新
	b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
	world->Step(1.0f / 60.0f, 6, 2);

	//プレイヤーの更新処理
	player.Update();

	//アンカーの更新処理
	Anchor::Update();

	//フィールドの更新処理
	Field::Update();

	controller.CheckInput();


#ifdef _DEBUG
	//デバッグ文字
	UpdateDebug();
#endif // _DEBUG

}


void Game::Draw(void)
{
	//バッファクリア
	Clear();

	//2D描画なので深度無効
	SetDepthEnable(false);

	//プレイヤーの描画処理
	player.Draw();

	//フィールドの描画処理
	Field::Draw();

	//アンカーの描画処理
	Anchor::Draw();

	//体力ソウルゲージUIの描画処理
	stamina_spirit_gauge.Draw();



#ifdef _DEBUG
	//デバッグ文字
	DrawDebug();
#endif // _DEBUG

	//バックバッファ、フロントバッファ入れ替え
	Present();
}



/**
 * @fn  Gameクラスのコンストラクタ
 * @brief Gameクラスのメンバ変数の中にあるクラスにシングルトンでインスタンスをいれる
 */
Game::Game()
{
	//プレイヤーのインスタンスを持って来てGameクラスのメンバを登録する
	player = Player::GetInstance();//シングルトン

	
}

Game::~Game()
{
}