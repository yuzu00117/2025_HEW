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
#include"impact_effect.h"
#include"gokai.h"
#include"blown_away_effect.h"
#include"dead_production.h"
#include"break_effect.h"
#include"change_scene_end_production.h"
#include"change_scene_start_production.h"
#include"UI_StaminaSpirit_Gauge.h"

int HitStop::hit_stop_time = 0;
bool  HitStop::hit_stop_flag = false;


void Game::Initialize()
{
    //ゲームシーンに入って初回はInitialize要らない（BeforeGameScene.cppで事前にInitializeした）（目的：リスポンする時Initializeしたくない物があるから）
    if (!m_respawn)
    {
        PlayerLife::Initialize();
    }


    //全ての音を止める
    app_atomex_stop_player();

    SceneManager &scene= SceneManager::GetInstance();

    //ステージに合わせてBGMを切り替える
    switch (scene.GetStageName())
    {
    case STAGE_TUTORIAL:
        app_atomex_start(POP_BGM);
        break;
    case STAGE_1_1:
        app_atomex_start(STAGE1_BGM);
        break;
    default:
        break;
    }



    //文字（絵）
    InitializeWord();


    //死亡処理の演出のリセット
    dead_production::Reset();
    //changeシートの終了処理リセット
    change_scene_end_production::Reset();
    //changeシーン開始処理のリセット
    change_scene_start_production::Reset();



    //マップによって初期リスを変える　　　これアンカーのレベル引き継いでないわ
    SceneManager& sceneManager = SceneManager::GetInstance();
   



	//プレイヤーUIの初期化
    player_UI::Initialize();
    //プレイヤーの体力の初期化
    PlayerStamina::Initialize();

    //ソウルゲージの初期化
    AnchorSpirit::Initialize();

    //アンカーの初期化
    Anchor::Initialize();

    //フィールドの初期化
    Field::Initialize(m_respawn);

    //背景の初期化
    Bg::Initialize();
    //衝撃エフェクト
    InitImpactEffect();
    //撃墜演出エフェクト
    InitBlownAwayEffect();

    Gokai_UI::Initialize();

    //死亡処理の演出のイニシャライズを行う
    dead_production::Initialize();
    //シーン終了の演出のイニシャライズを行う
    change_scene_end_production::Initialize();
    //シーン開始の演出のイニシャライズを行う
    change_scene_start_production::Initialize();


    b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();


    //壊れるオブジェクトのエフェクト
    PillarFragmentsManager::GetInstance().Init(world, 3, 3);

    
    // 衝突リスナーをワールドに登録
    MyContactListener& contactListener = MyContactListener::GetInstance();
    world->SetContactListener(&contactListener);

#ifndef _DEBUG
    //デバッグ文字
    InitializeDebug();
#endif // !_DEBUG

    Respawn();
}

//残機はゲームシーンではFinalizeはやらない、Resultシーンでやる
void Game::Finalize(void)
{
    if (!m_respawn)
    {
        PlayerLife::Finalize();
    }
	//プレイヤーUIの終了処理
    player_UI::Finalize();
	//プレイヤーの終了処理
    player.Finalize();

	

    //アンカー終了処理
    Anchor::Finalize();

    //フィールドの終了処理
    Field::Finalize(m_respawn);

    //背景の終了処理
    Bg::Finalize();

	//ボスの終了処理
    boss.Finalize();

    //衝突時のエフェクトの終了処理
    FinalizeImpactEffects();

    //文字（絵）
    FinalizeWord();

    Gokai_UI::Finalize();

    dead_production::Finalize();


    change_scene_end_production::Finalize();

    change_scene_start_production::Finalize();

    //体力ソウルゲージUIの終了処理
    //stamina_spirit_gauge.Finalize()がなかったのでそれらしいものを探してみた
    StaminaSpiritGauge staminaSpiritGauge;
    staminaSpiritGauge.Finalize();


    //衝突時のエフェクトを
    FinalizeImpactEffects();
    //撃墜演出エフェクト
    FinalizeBlownAwayEffects();

    //壊れるエフェクトブロックのファイナライズ
    PillarFragmentsManager::GetInstance().Finalize();


    


#ifdef _DEBUG
    //デバッグ文字
    FinalizeDebug();
#endif // _DEBUG


    Box2dWorld::GetInstance().RecreateWorld();

}

void Game::Update(void)
{
    SceneManager& sceneManager = SceneManager::GetInstance();

    // Box2D ワールドのステップ更新
    b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();

    if (HitStop::GetHitStopFlag()==true)
    {
        HitStop::CountHitStop();
    }
    else {
       if(world->GetBodyCount() > 0)
        {
            world->Step(1.0f / 60.0f, 6, 2);

            //ディスプレイの更新処理
            display::Update();

            //プレイヤーライフの更新処理
            PlayerLife::Update();
            //プレイヤーUIの更新処理
            player_UI::Update();

            AnchorSpirit::Update();

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

            //壊れる演出のエフェクト
            PillarFragmentsManager::GetInstance().UpdateFragments();

            //衝突エフェクトの描画処理
            UpdateImpactEffects();

            //撃墜演出エフェクト
            UpdateBlownAwayEffects();

         

            //シーン遷移の確認よう　　アンカーのstateが待ち状態の時
            if (Keyboard_IsKeyDown(KK_R) && Anchor::GetAnchorState() == Nonexistent_state)
            {
                m_respawn = false;
                sceneManager.ChangeScene(SCENE_RESULT);
            }

            if (Keyboard_IsKeyDown(KK_B))//ボスにいくものとする
            {
               
                sceneManager.SetStageName(STAGE_BOSS);
                sceneManager.ChangeScene(SCENE_GAME);
            }


            //シーン移行の管理
            if (sceneManager.Get_Chenge_Scene_flag() == true)
            {
                //シーン移行したらfalseにする
                sceneManager.Set_Chenge_Scene_flag(false);
                switch (sceneManager.GetStageName())
                {
                case STAGE_SELECT:
                    sceneManager.SetStageName(STAGE_SELECT);
                    sceneManager.ChangeScene(SCENE_STAGE_SELECT);
                    break;
                case STAGE_TUTORIAL:
                    sceneManager.SetStageName(STAGE_TUTORIAL);
                    sceneManager.ChangeScene(SCENE_GAME);
                    break;
                case STAGE_1_1:
                    sceneManager.SetStageName(STAGE_1_1);
                    sceneManager.ChangeScene(SCENE_GAME);
                    break;
                case STAGE_BOSS:
                    sceneManager.SetStageName(STAGE_BOSS);
                    sceneManager.ChangeScene(SCENE_GAME);
                    break;
                default:
                    break;
                }

              
            }




            
        }
       else
       {
           std::cerr << "[Error] No bodies exist in the Box2D world!" << std::endl;
       }



        

#ifdef _DEBUG
        //デバッグ文字
        UpdateDebug();
#endif // _DEBUG
    }

	//カメラシェイクの更新処理
    CameraShake::Update();


    //画面開始処理
    change_scene_start_production::Update();

    //プレイヤーが死亡したらリザルト画面に遷移
    if (PlayerStamina::IsPlayerDead())
    {
        dead_production::Update();
    }

    //プレイヤーが死亡したらリザルト画面に遷移
    if (dead_production::GetDeadFlag())
    {
        //プレイヤーの残機が残っていたら最初からスタート
        if (PlayerLife::GetLife() > 0)
        {
            m_respawn = true;
            PlayerLife::SetLife(PlayerLife::GetLife() - 1);
            dead_production::SetDeadFlag(false);
            SceneManager& sceneManager = SceneManager::GetInstance();
            sceneManager.ChangeScene(SCENE_GAME);
        }
        else
        {
            m_respawn = false;
            dead_production::SetDeadFlag(false);
            SceneManager& sceneManager = SceneManager::GetInstance();
            sceneManager.ChangeScene(SCENE_RESULT);
        }


    }

    //シーン移行の管理
    if (sceneManager.Get_Chenge_Scene_flag() == true)
    {
        //シーン移行したらfalseにする
        change_scene_end_production::Update();
        if (change_scene_end_production::GetChangeFlag() == true)
        {
            sceneManager.Set_Chenge_Scene_flag(false);
            switch (sceneManager.GetStageName())
            {
            case STAGE_SELECT:
                sceneManager.SetStageName(STAGE_SELECT);
                sceneManager.ChangeScene(SCENE_STAGE_SELECT);
                break;
            case STAGE_TUTORIAL:
                sceneManager.SetStageName(STAGE_TUTORIAL);
                sceneManager.ChangeScene(SCENE_GAME);
                break;
            case STAGE_1_1:
                sceneManager.SetStageName(STAGE_1_1);
                sceneManager.ChangeScene(SCENE_GAME);
                break;
            case STAGE_BOSS:
                sceneManager.SetStageName(STAGE_BOSS);
                sceneManager.ChangeScene(SCENE_GAME);
                break;
            default:
                break;
            }
        }


    }

}


void Game::Respawn()
{
    ////リスポン時の座標決め
    ////==========================================================================
    //b2Vec2 respawn_position = b2Vec2{ 5.0f, 6.0f };
    //ItemManager& item_manager = ItemManager::GetInstance();
    //ItemSavePoint* savepoint = item_manager.FindItem_SavePoint();
    ////ステージに中間地がある場合
    //if (savepoint != nullptr)
    //{
    //    if (savepoint->GetIfPlayerPassed())
    //    {
    //        respawn_position = savepoint->GetBody()->GetPosition();
    //        AnchorSpirit::SetAnchorSpiritValueDirectly(100);    //アンカーをlevel２にセット
    //    }
    //}


    //Player::GetOutSidePlayerBody()->SetTransform(respawn_position, 0.0f);



    //リスポンした時の効果音
    app_atomex_start(Player_Jewelry_Colect_Sound);
}



void Game::Draw(void)
{
    //バッファクリア
    Clear();

    //背景の描画処理
    Bg::Draw();

    //2D描画なので深度無効
    SetDepthEnable(false);

 
    //背景に描画
    objectManager.DrawBack();

    //ボスの描画処理
    boss.Draw();



    //描画の順番を調整するためにDrawのみ、外に出す
    itemManager.DrawAll();
    objectManager.DrawAll();
    //フィールドの描画処理
    Field::Draw();

 

    //プレイヤーの描画処理
    player.Draw();

    //アンカーの描画処理
    Anchor::Draw();

  

    itemManager.DrawFront();
    objectManager.DrawFront();


    //衝突時のエフェクト
    DrawImpactEffects(1.0f);
    DrawBlownAwayEffects(1.0f);



    Bg::FrontDraw();


	
 


	//�c�@�̕`�揈��
	PlayerLife::Draw();



	player_UI::Draw();

    Gokai_UI::Draw();



	player_UI::Draw();

    PillarFragmentsManager::GetInstance().DrawFragments();

    dead_production::Draw();



    //チェンジシーン
    change_scene_end_production::Draw();


    change_scene_start_production::Draw();




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


