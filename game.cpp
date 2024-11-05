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
#include"box2d_world.h"
#include"square_collider.h"
#include"directx_controller.h"
#include"game.h"
#include"player.h"
#include"feild.h"


//この管理方法どうなん？
SquareCollider* square_collider[100];

Player* player_collider;


DirectInputController controller;



HRESULT Game::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//レンダリング処理の初期化
	InitRenderer(hInstance, hWnd, bWindow);

	//サウンドの初期化
	InitSound(hWnd);

	//ポリゴン
	InitSprite();

	//コントローラーの初期化
	controller.Initialize(hInstance,hWnd);

	
	//インスタンスを取得
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();

	//ワールドを生成
	b2World* world = new b2World(b2Vec2(0.0f, 10.f));

	//生成したワールドのポインタをセット
	box2d_world.SetBox2dWorldPointer(world);


	//テキトーに設定した

	//プレイヤー
	player_collider->Init();

	//Init
	player_collider = new Player(XMFLOAT2(5.0f, 5.0f), XMFLOAT2(1.0f, 1.0f), 0.0f, false);



	Field::Init(30,20);

	





	//// 溶接ジョイントを作成
	//b2WeldJointDef weldJointDef;
	//weldJointDef.bodyA =square_collider[2]->GetColliderBody();
	//weldJointDef.bodyB = square_collider[3]->GetColliderBody();
	//weldJointDef.localAnchorA.Set(1.0f, 0.0f); // ボディAの右端
	//weldJointDef.localAnchorB.Set(-1.0f, 0.0f); // ボディBの左端
	//weldJointDef.collideConnected = false;  // ジョイントで接続された物体同士が衝突しない

	//b2WeldJoint* weldJoint = (b2WeldJoint*)world->CreateJoint(&weldJointDef);


	return S_OK;
}

void Game::Uninit(void)
{
	//ポリゴン
	UninitSprite();

	//コントローラーの終了処理
	controller.Release();

	//サウンドの終了処理
	UninitSound();


	Field::Uninit();

	//レンダリングの終了処理
	UninitRenderer();
}



void Game::Update(void)
{
	//インスタンスを取得
	Box2dWorld& box2d_world = Box2dWorld::GetInstance();
	//ワールドのステップしてる
	box2d_world.GetBox2dWorldPointer()->Step(1.f / 60.f, 8, 3);



	


	controller.CheckInput();
}


void Game::Draw(void)
{
	//バッファクリア
	Clear();

	//2D描画なので深度無効
	SetDepthEnable(false);

	//ここに描画処理

	//ここから下はテスト
	
	player_collider->Draw();

	Field::Draw();
	


	//バックバッファ、フロントバッファ入れ替え
	Present();
}

Game::Game()
{
}

Game::~Game()
{
}