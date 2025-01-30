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
#include"display.h"
#include"player_life.h"
#include"scene.h"
#include"player.h"
#include"player_stamina.h"
#include"anchor_spirit.h"
#include"bg.h"
#include"hit_stop.h"
#include"camera_shake.h"
#include"player_UI.h"

int HitStop::hit_stop_time = 0;
bool  HitStop::hit_stop_flag = false;


void Game::Initialize()
{

    //文字（絵）
    InitializeWord();


    //プレイヤーの初期化
    player.Initialize(b2Vec2(1, 0), b2Vec2(1, 2),player.GetSensorSizeLev1_2());
	//プレイヤーライフの初期化
    PlayerLife::Initialize();
	//プレイヤーUIの初期化
    player_UI::Initialize();
    //プレイヤーの体力の初期化
    PlayerStamina::Initialize();

    //ソウルゲージの初期化
    AnchorSpirit::Initialize();

    //アンカーの初期化
    Anchor::Initialize();

    //フィールドの初期化
    Field::Initialize();

    //体力ソウルゲージUIの初期化
    stamina_spirit_gauge.Initialize();

    //背景の初期化
    Bg::Initialize();

	//criの初期化
    CRIInitialize();

    b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
    // 衝突リスナーをワールドに登録
    MyContactListener& contactListener = MyContactListener::GetInstance();
    world->SetContactListener(&contactListener);

#ifndef _DEBUG
    //デバッグ文字
    InitializeDebug();
#endif // !_DEBUG

}

void Game::Finalize(void)
{

    //プレイヤーライフの終了処理
    PlayerLife::Finalize();
	//プレイヤーUIの終了処理
    player_UI::Finalize();
	//プレイヤーの終了処理
    player.Finalize();

	//criの終了処理
    CRIFinalize();

    //アンカー終了処理
    Anchor::Finalize();

    //フィールドの終了処理
    Field::Finalize();

    //背景の終了処理
    Bg::Finalize();

	//ボスの終了処理
    boss.Finalize();

    //文字（絵）
    FinalizeWord();

    //体力ソウルゲージUIの終了処理
    stamina_spirit_gauge.Finalize();

#ifdef _DEBUG
    //デバッグ文字
    FinalizeDebug();
#endif // _DEBUG

}

void Game::Update(void)
{

    // Box2D ワールドのステップ更新
    b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();

    if (HitStop::GetHitStopFlag()==true)
    {
        HitStop::CountHitStop();
    }
    else {
        world->Step(1.0f / 60.0f, 6, 2);

		//ディスプレイの更新処理
        display::Update();

        //プレイヤーライフの更新処理
        PlayerLife::Update();
		//プレイヤーUIの更新処理
        player_UI::Update();
        //プレイヤーの更新処理
        player.Update();

        //アンカーの更新処理
        Anchor::Update();

		//criの更新処理
        CRIUpdate();

		//背景の更新処理
        Bg::Update();

        //フィールドの更新処理
        Field::Update();

		//ボスの更新処理
        boss.Update();

        //プレイヤーが死亡したらリザルト画面に遷移
        if (PlayerStamina::IsPlayerDead())
        {
            SceneManager& sceneManager = SceneManager::GetInstance();
            sceneManager.ChangeScene(SCENE_RESULT);
        }

        //シーン遷移の確認よう　　アンカーのstateが待ち状態の時
        if (Keyboard_IsKeyDown(KK_R) && Anchor::GetAnchorState() == Nonexistent_state)
        {
            SceneManager& sceneManager = SceneManager::GetInstance();
            sceneManager.ChangeScene(SCENE_RESULT);
        }

  


	  //プレイヤーが死亡したらリザルト画面に遷移
	  if (PlayerStamina::IsPlayerDead())
	  {
		  SceneManager& sceneManager = SceneManager::GetInstance();
		  sceneManager.ChangeScene(SCENE_RESULT);
	  }

		//シーン遷移の確認よう　　アンカーのstateが待ち状態の時
		if (Keyboard_IsKeyDown(KK_R) && Anchor::GetAnchorState() == Nonexistent_state)
		{
			SceneManager& sceneManager = SceneManager::GetInstance();
			sceneManager.ChangeScene(SCENE_RESULT);
		}

		if (Keyboard_IsKeyDown(KK_B))//ボスにいくものとする
		{
			b2Vec2 size = player.GetSensorSize();

			player.Finalize();

			player.Initialize(b2Vec2(48, 0), b2Vec2(1, 2), size);

			boss.Initialize(b2Vec2(53, 0), b2Vec2(18, 24),true);

		}



        

#ifdef _DEBUG
        //デバッグ文字
        UpdateDebug();
#endif // _DEBUG
    }

	//カメラシェイクの更新処理
    CameraShake::Update();

}

void Game::Draw(void)
{
    //バッファクリア
    Clear();

    //背景の描画処理
    Bg::Draw();

    //2D描画なので深度無効
    SetDepthEnable(false);

    //プレイヤーの描画処理
    player.Draw();


    //ボスの描画処理
    boss.Draw();

    //フィールドの描画処理
    Field::Draw();



    //アンカーの描画処理
    Anchor::Draw();







	



	//�c�@�̕`�揈��
	PlayerLife::Draw();



	player_UI::Draw();

  //体力ソウルゲージUIの描画処理
	stamina_spirit_gauge.Draw();


	//プレイヤーUIの描画処理
    player_UI::Draw();

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

void Game::Teleport_player(b2Vec2 position)
{
    b2Vec2 size_sensor=player.GetSensorSize();

    player.Finalize();
    player.Initialize(position, b2Vec2(1.f, 2.f), size_sensor);

}
