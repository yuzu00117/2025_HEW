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
#include"Gauge_UI.h"
#include"impact_effect.h"
#include"gokai.h"
#include"blown_away_effect.h"
#include"dead_production.h"
#include"break_effect.h"
#include"change_scene_end_production.h"
#include"change_scene_start_production.h"
#include"UI_StaminaSpirit_Gauge.h"
#include"Xinput_controller.h"
#include"Stamina_UI.h"

int HitStop::hit_stop_time = 0;
bool  HitStop::hit_stop_flag = false;
int HitStop::delay_hit_stop_time = 0;
int HitStop::delay_time = 0;

GAME_STATE  next_state = GAME_STATE_RESPAWN_INITIAL;



void Game::Initialize()
{

    //全ての音を止める
    app_atomex_stop_player();

    SceneManager& scene = SceneManager::GetInstance();

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


    //フィールドの初期化
    Field::Initialize();

    objectManager.InitializeAll();


    switch (m_state)
    {
    case GAME_STATE_START:
        //アイテムの初期化
        itemManager.InitializeAll();
        //残機の初期化
        PlayerLife::Initialize();
        //プレイヤーの体力の初期化
        PlayerStamina::Initialize();
        //ソウルゲージの初期化
        AnchorSpirit::Initialize();
        //ソウルゲージUIの初期化
        Gauge_UI::Initialize();
        //豪快度UIの初期化
        Gokai_UI::Initialize();
        //体力UIの初期化
        Stamina_UI::Initialize();
        break;
    case GAME_STATE_RESPAWN_INITIAL:
        //体力を初期化
        PlayerStamina::Initialize();
        //アンカーを初期化
        AnchorSpirit::Initialize();
        //豪快度を記録した値に戻す
        Gokai_UI::SetNowGokaiCount(Gokai_UI::GetGokai_WhenRespawn());

        //リスポン用のアイテムの初期化
        itemManager.Initialize_WhenRespawn();
        if (scene.GetStageName() == STAGE_BOSS)
        {
            itemManager.UseAllJewel();
        }
        break;
    case GAME_STATE_RESPAWN_SAVE_POINT:
        //体力を初期化
        PlayerStamina::Initialize();
        //アンカーを初期化
        AnchorSpirit::Initialize();
        //アンカーをlevel２にセット
        AnchorSpirit::SetAnchorSpiritValueDirectly(100);
        //豪快度を記録した値に戻す
        Gokai_UI::SetNowGokaiCount(Gokai_UI::GetGokai_WhenRespawn());

        //リスポン用のアイテムの初期化
        itemManager.Initialize_WhenRespawn();
        if (scene.GetStageName() == STAGE_BOSS)
        {
            itemManager.UseAllJewel();
        }
        break;
    case GAME_STATE_NEXT_STAGE:
        //リスポン用のアイテムの初期化
        itemManager.Initialize_WhenNextStage();

        //今の豪快値を豪快UIに記録
        Gokai_UI::SetGokai_WhenRespawn(Gokai_UI::GetNowGokaiCount());

        if (scene.GetStageName() == STAGE_BOSS)
        {
            itemManager.UseAllJewel();
        }
        break;
    case GAME_STATE_GAMEOVER:
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



    //アンカーの初期化
    Anchor::Initialize();


    //背景の初期化
    Bg::Initialize();
    //衝撃エフェクト
    InitImpactEffect();
    //撃墜演出エフェクト
    InitBlownAwayEffect();


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

    Box2dWorld& world_instance = Box2dWorld::GetInstance();
    world_instance.SetWorldCallStep(true);

    

#ifndef _DEBUG
    //デバッグ文字
    InitializeDebug();
#endif // !_DEBUG

}

//残機はゲームシーンではFinalizeはやらない、Resultシーンでやる
void Game::Finalize(void)
{

    //フィールドの終了処理
    Field::Finalize();

    //アンカーポイントの終了処理
    AnchorPoint::Finalize();
    //オブジェクトの終了処理
    objectManager.FinalizeAll();


    switch (next_state)
    {
    case GAME_STATE_START:
        break;
    case GAME_STATE_RESPAWN_INITIAL:
        //リスポン用のアイテムの終了処理
        itemManager.Finalize_WhenRespawn();
        break;
    case GAME_STATE_RESPAWN_SAVE_POINT:
        //リスポン用のアイテムの終了処理
        itemManager.Finalize_WhenRespawn();
        break;
    case GAME_STATE_NEXT_STAGE:
        //アイテムの終了処理
        itemManager.Finalize_WhenNextStage();
        //プレイヤーが登録した中間地点を解除
        player.RegisterSavePoint(nullptr);
        break;
    case GAME_STATE_GAMEOVER:
        //アイテムの終了処理
        itemManager.FinalizeAll();
        //残機終了処理
        PlayerLife::Finalize();
        //ソウルゲージUIの終了処理
        Gauge_UI::Finalize();
        //豪快度UIの終了処理
        Gokai_UI::Finalize();
        //体力UIの終了処理
        Stamina_UI::Finalize();
        //プレイヤーが登録した中間地点を解除
        player.RegisterSavePoint(nullptr);
        break;
    default:
        break;
    }
    m_state = next_state;
    if (m_state == GAME_STATE_GAMEOVER) { m_state = GAME_STATE_START; }
    next_state = GAME_STATE_RESPAWN_INITIAL;

	//プレイヤーの終了処理
    player.ResetPlayerParameter();
    player.Finalize();

    //アンカー終了処理
    Anchor::Finalize();


    //背景の終了処理
    Bg::Finalize();

	//ボスの終了処理
    boss.Finalize();

    //衝突時のエフェクトの終了処理
    FinalizeImpactEffects();

    //文字（絵）
    FinalizeWord();


    dead_production::Finalize();


    change_scene_end_production::Finalize();

    change_scene_start_production::Finalize();



    //衝突時のエフェクトを
    FinalizeImpactEffects();
    //撃墜演出エフェクト
    FinalizeBlownAwayEffects();

    //壊れるエフェクトブロックのファイナライズ
    PillarFragmentsManager::GetInstance().Finalize();


    


#ifndef _DEBUG
    //デバッグ文字
    FinalizeDebug();
#endif // _DEBUG


    Box2dWorld::GetInstance().RecreateWorld();

}

void Game::Update(void)
{
    SceneManager& sceneManager = SceneManager::GetInstance();

    // Box2D ワールドのステップ更新
    b2World * world = Box2dWorld::GetInstance().GetBox2dWorldPointer();

    Box2dWorld& world_instance = Box2dWorld::GetInstance();

    if (HitStop::GetHitStopFlag()==true)
    {
        HitStop::CountHitStop();
    }
    else {
        if (world && world->GetBodyCount() > 0 && world_instance.GetWorldCallStep() == true)
        {
            world->Step(1.0f / 60.0f, 6, 2);
        }
            //ディレイヒットストップの調整
            HitStop::DelayCountHitStop();

            //ディスプレイの更新処理
            display::Update();


            //アンカーの更新処理
            Anchor::Update();

            //プレイヤーライフの更新処理
            PlayerLife::Update();
            //ソウルゲージUIの更新処理
            Gauge_UI::Update();
            //体力UIの更新処理
            Stamina_UI::Update();

            AnchorSpirit::Update();

            //プレイヤーの更新処理
            player.Update();

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
         
#ifdef _DEBUG
            //シーン遷移の確認よう　　アンカーのstateが待ち状態の時
            if (Keyboard_IsKeyDown(KK_R) && Anchor::GetAnchorState() == Nonexistent_state)
            {
                next_state = GAME_STATE_GAMEOVER;
                sceneManager.ChangeScene(SCENE_RESULT);
            }

            if (Keyboard_IsKeyDown(KK_B))//ボスにいくものとする
            {
                next_state = GAME_STATE_NEXT_STAGE;
                sceneManager.SetStageName(STAGE_BOSS);
                sceneManager.ChangeScene(SCENE_GAME);
            }

            if (Keyboard_IsKeyDown(KK_I))//遺跡ステージにいく
            {
                next_state = GAME_STATE_NEXT_STAGE;
                sceneManager.SetStageName(STAGE_ISEKI);
                sceneManager.ChangeScene(SCENE_GAME);
            }

            //デバッグ文字
            UpdateDebug();
#endif // _DEBUG
        }


    VibrationController::UpdateVibration(); // 毎フレーム振動を管理
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
        if (PlayerLife::GetLife() > 1)
        {
            if (player.GetRegisteredSavePoint() != nullptr) { next_state = GAME_STATE_RESPAWN_SAVE_POINT; }
            else { next_state = GAME_STATE_RESPAWN_INITIAL; }

            PlayerLife::SetLife(PlayerLife::GetLife() - 1);
            dead_production::SetDeadFlag(false);
            SceneManager& sceneManager = SceneManager::GetInstance();
            sceneManager.ChangeScene(SCENE_GAME);
        }
        else
        {
            next_state = GAME_STATE_GAMEOVER;
            dead_production::SetDeadFlag(false);
            SceneManager& sceneManager = SceneManager::GetInstance();
            sceneManager.SetStageName(STAGE_TUTORIAL);
            sceneManager.ChangeScene(SCENE_TITLE);
        }
    }

    //シーン移行の管理（死亡した時以外のシーン移動）
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
                next_state = GAME_STATE_GAMEOVER;
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
            case STAGE_ISEKI:
                next_state = GAME_STATE_NEXT_STAGE;
                sceneManager.SetStageName(STAGE_ISEKI);
                sceneManager.ChangeScene(SCENE_GAME);
                break;
            case STAGE_BOSS:
                next_state = GAME_STATE_NEXT_STAGE;
                sceneManager.SetStageName(STAGE_BOSS);
                sceneManager.ChangeScene(SCENE_GAME);
                break;
            default:
                break;
            }
        }
    }
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

    boss.DrawObjectFront();

    itemManager.DrawFront();

    //プレイヤーの描画処理
    player.Draw();

    //アンカーの描画処理
    Anchor::Draw();

    objectManager.DrawFront();

    //衝突時のエフェクト
    DrawImpactEffects(1.0f);
    DrawBlownAwayEffects(1.0f);

    Bg::FrontDraw();
    // プレイヤーの残機を描画
	PlayerLife::Draw();
    // プレイヤーの体力UIの描画
    Stamina_UI::Draw(); 
    // ソウルゲージUIの描画
	Gauge_UI::Draw();
    // 豪快度UIの描画（スコア）
    Gokai_UI::Draw();

    PillarFragmentsManager::GetInstance().DrawFragments();

    dead_production::Draw();

    //チェンジシーン
    change_scene_end_production::Draw();

    change_scene_start_production::Draw();




#ifndef _DEBUG
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


