//-----------------------------------------------------------------------------------------------------
// #name GamePause.cpp
// #description     ゲームのポーズ画面
// #make 2025/2/28　王泳心
// #update 2025/2/28
// #comment 追加・修正予定
//                  
//                  
//----------------------------------------------------------------------------------------------------
#include "GamePause.h"
#include "texture.h"
#include "sprite.h"
#include "main.h"
#include "keyboard.h"
#include "Xinput_controller.h"
#include "player.h"
#include "game.h"
#include "scene.h"
#include "Item_Coin_UI.h"
#include "Gauge_UI.h"

//グローバル変数
static ID3D11ShaderResourceView* g_UnPause_words_Texture;				//ポーズ解除ボタンのテクスチャ
static ID3D11ShaderResourceView* g_Respawn_SavePoint_words_Texture;     //中間地にリスポンするためのボタンのテクスチャ
static ID3D11ShaderResourceView* g_Respawn_InitalPoint_words_Texture;   //リスポンするためのボタンのテクスチャ
static ID3D11ShaderResourceView* g_SelectScene_words_Texture;           //セレクト画面に戻るボタンのテクスチャ
static ID3D11ShaderResourceView* g_TitleScene_words_Texture;            //タイトル画面に戻るボタンのテクスチャ
static ID3D11ShaderResourceView* g_button_frame_Texture;				//選択されていないボタンの枠のテクスチャ
static ID3D11ShaderResourceView* g_button_selected_frame_Texture;		//選択されたボタンの枠のテクスチャ
static ID3D11ShaderResourceView* g_button_locked_frame_Texture;		    //選択できないボタンの枠のテクスチャ
static ID3D11ShaderResourceView* g_PauseBackground_Texture;				//ポーズ画面の背景のテクスチャ
static ID3D11ShaderResourceView* g_PauseSelectionBackground_Texture;	//ポーズ画面の選択肢の背景のテクスチャ
static ID3D11ShaderResourceView* g_PauseSelectionHand_Texture;	        //ポーズ画面の選択肢を指す手のテクスチャ
static ID3D11ShaderResourceView* g_PauseUIRecordingBackground_Texture;	//ポーズ画面の取得数記録の背景のテクスチャ
static ID3D11ShaderResourceView* g_PauseUIRecording_JewelRing_Texture;	//ポーズ画面の宝石のリングのテクスチャ
static ID3D11ShaderResourceView* g_PauseUIRecording_Jewel_Blue_Texture; //ポーズ画面の青宝石のテクスチャ
static ID3D11ShaderResourceView* g_PauseUIRecording_Jewel_Yellow_Texture; //ポーズ画面の青宝石のテクスチャ
static ID3D11ShaderResourceView* g_PauseUIRecording_Jewel_Red_Texture;  //ポーズ画面の青宝石のテクスチャ
static ID3D11ShaderResourceView* g_PauseUIRecording_CoinUI_Texture; 	//ポーズ画面のコインUIのテクスチャ
static ID3D11ShaderResourceView* g_PauseUIRecording_NumberUI_Texture; 	//ポーズ画面の数字UIのテクスチャ

bool    Respawn_SavePoint = false;
int     Max_Coin = 0;
int     Current_Coin = 0;


//ボタンのサイズ
XMFLOAT2 g_button_scale[BUTTON_NUM] =
{
    {370.0f,60.0f},
    {370.0f,60.0f},
    {370.0f,60.0f},
    {370.0f,60.0f},
    {370.0f,60.0f}
};

float space = 100.0f;
//ボタンの座標
XMFLOAT2 g_button_position[BUTTON_NUM] =
{
    {320.0f, 220.0f},
    {320.0f, g_button_position[0].y + space},
    {320.0f, g_button_position[1].y + space},
    {320.0f, g_button_position[2].y + space},
    {320.0f, g_button_position[3].y + space}
};
//選択肢の背景（紙）
XMFLOAT2    g_selection_background_scale = { 650.0f, 870.0f };
XMFLOAT2    g_selection_background_position = { g_button_position[1].x, 330.0f};
//選択肢を指す手
XMFLOAT2    g_selection_hand_scale = { 50.0f, 50.0f };
XMFLOAT2    g_selection_hand_position = { g_button_position[0].x + (g_button_scale[0].x/2)*1.15f, g_button_position[0].y};
//取得数の背景（紙）
XMFLOAT2    g_UIRecording_background_scale = { 650.0f, 420.0f };
XMFLOAT2    g_UIRecording_background_position = { 950.0f, 520.0f};
//宝石のリング
XMFLOAT2    g_UIRecording_JewelRing_scale = { 260.0f, 200.0f };
XMFLOAT2    g_UIRecording_JewelRing_position = { g_UIRecording_background_position.x, g_UIRecording_background_position.y * 0.85f };
//宝石（0：青　　１：黄色　　2：赤）
XMFLOAT2 g_UIRecording_Jewel_scale[3] =
{
    {400.0f,800.0f},
    {400.0f,800.0f},
    {400.0f,800.0f}
};
XMFLOAT2 g_UIRecording_Jewel_position[3] =
{
    {g_UIRecording_JewelRing_position.x * 1.064f, g_UIRecording_JewelRing_position.y * 0.33f},
    {g_UIRecording_JewelRing_position.x * 1.069f, g_UIRecording_JewelRing_position.y * 0.33f},
    {g_UIRecording_JewelRing_position.x * 1.06f, g_UIRecording_JewelRing_position.y * 0.33f}
};
//コインのアイコン
XMFLOAT2    g_UIRecording_CoinUI_scale = { 120.0f, 120.0f };
XMFLOAT2    g_UIRecording_CoinUI_position = { g_UIRecording_background_position.x * 0.83f, g_UIRecording_background_position.y * 1.15f };
//コインの数字表示
XMFLOAT2    g_UIRecording_NumberUI_scale = { 40.0f, 40.0f };
XMFLOAT2    g_UIRecording_NumberUI_Left_position = { g_UIRecording_background_position.x, g_UIRecording_background_position.y * 1.19f };
XMFLOAT2    g_UIRecording_NumberUI_Right_position = { g_UIRecording_background_position.x * 1.12f, g_UIRecording_background_position.y * 1.19f };

void GamePause::Initialize()
{
    g_UnPause_words_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_ResumeGame.png");
    g_Respawn_SavePoint_words_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_Restart_Checkpoint.png");
    g_Respawn_InitalPoint_words_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_RestartBeginning.png");
    g_SelectScene_words_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_ReturnStageSelect.png");
    g_TitleScene_words_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_ReturnTitle.png");
    g_button_frame_Texture = InitTexture(L"asset\\texture\\Pause_texture\\button_frame.png");
    g_button_selected_frame_Texture = InitTexture(L"asset\\texture\\Pause_texture\\button_selected_frame.png");
    g_button_locked_frame_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_RestartCheckpoint_NOcheckpoint.png");
    g_PauseBackground_Texture = InitTexture(L"asset\\texture\\Pause_texture\\PauseBackground.png");
    g_PauseSelectionBackground_Texture = InitTexture(L"asset\\texture\\Pause_texture\\pose_BG.png");
    g_PauseSelectionHand_Texture = InitTexture(L"asset\\texture\\Pause_texture\\stage_select_player.png");
    g_PauseUIRecordingBackground_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_pose_koin&houseki_BG.png");
    g_PauseUIRecording_JewelRing_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_houseiki_ringng.png");
    g_PauseUIRecording_Jewel_Blue_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\blue_jewel.png");
    g_PauseUIRecording_Jewel_Yellow_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\yellow_jewel.png");
    g_PauseUIRecording_Jewel_Red_Texture = InitTexture(L"asset\\texture\\UI_soul_gage\\red_jewel.png");
    g_PauseUIRecording_CoinUI_Texture = InitTexture(L"asset\\texture\\Pause_texture\\UI_koin_akon.png");
    g_PauseUIRecording_NumberUI_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_number.png");

    Max_Coin = 0;
    Current_Coin = 0;
    Max_Coin = Item_Coin_UI::GetMaxCoinCount();
    key_flag.ControllerButton_Start = true;
}

void GamePause::Finalize()
{
    if (g_UnPause_words_Texture) { UnInitTexture(g_UnPause_words_Texture); }
    if (g_Respawn_SavePoint_words_Texture) { UnInitTexture(g_Respawn_SavePoint_words_Texture); }
    if (g_Respawn_InitalPoint_words_Texture) { UnInitTexture(g_Respawn_InitalPoint_words_Texture); }
    if (g_SelectScene_words_Texture) { UnInitTexture(g_SelectScene_words_Texture); }
    if (g_TitleScene_words_Texture) { UnInitTexture(g_TitleScene_words_Texture); }
    if (g_button_frame_Texture) { UnInitTexture(g_button_frame_Texture); }
    if (g_button_selected_frame_Texture) { UnInitTexture(g_button_selected_frame_Texture); }
    if (g_button_locked_frame_Texture) { UnInitTexture(g_button_locked_frame_Texture); }
    if (g_PauseBackground_Texture) { UnInitTexture(g_PauseBackground_Texture); }
    if (g_PauseSelectionBackground_Texture) { UnInitTexture(g_PauseSelectionBackground_Texture); }
    if (g_PauseSelectionHand_Texture) { UnInitTexture(g_PauseSelectionHand_Texture); }
    if (g_PauseUIRecordingBackground_Texture) { UnInitTexture(g_PauseUIRecordingBackground_Texture); }
    if (g_PauseUIRecording_JewelRing_Texture) { UnInitTexture(g_PauseUIRecording_JewelRing_Texture); }
    if (g_PauseUIRecording_CoinUI_Texture) { UnInitTexture(g_PauseUIRecording_CoinUI_Texture); }
    if (g_PauseUIRecording_NumberUI_Texture) { UnInitTexture(g_PauseUIRecording_NumberUI_Texture); }
}

void GamePause::Update()
{
    Player& player = Player::GetInstance();
    if (player.GetPrevRegisteredSavePoint() != nullptr || player.GetRegisteredSavePoint() != nullptr)
    {
        Respawn_SavePoint = true;
    }
    else
    {
        Respawn_SavePoint = false;
    }
    
    Current_Coin = Item_Coin_UI::GetNowCoinCount();

    // コントローラーの入力の受け取り
    ControllerState state = GetControllerInput();
    //↓キーで下選択
    if(state.leftStickY < -100 && key_flag.CountTime > 30.0f)
    {
        switch (m_button_selected)
        {
        case Button_UnPause:
            m_button_selected = Button_Respawn_SavePoint;
            if (!Respawn_SavePoint)
            {
                m_button_selected = Button_Respawn_InitalPoint;
            }
            break;
        case Button_Respawn_SavePoint:
            m_button_selected = Button_Respawn_InitalPoint;
            break;
        case Button_Respawn_InitalPoint:
            m_button_selected = Button_SelectScene;
            break;
        case Button_SelectScene:
            m_button_selected = Button_TitleScene;
            break;
        case Button_TitleScene:
            m_button_selected = Button_UnPause;
            break;
        case Button_NULL:
            m_button_selected = Button_UnPause;
            break;
        }
        key_flag.CountTime = 0.0f;
        g_selection_hand_position.y = g_button_position[m_button_selected].y + 20.0f;
    }
    //↑キーで上選択
    else if (state.leftStickY > 100 && key_flag.CountTime > 30.0f)
    {
        switch (m_button_selected)
        {
        case Button_UnPause:
            m_button_selected = Button_TitleScene;
            break;
        case Button_Respawn_SavePoint:
            m_button_selected = Button_UnPause;
            break;
        case Button_Respawn_InitalPoint:
            m_button_selected = Button_Respawn_SavePoint;
            if (!Respawn_SavePoint)
            {
                m_button_selected = Button_UnPause;
            }
            break;
        case Button_SelectScene:
            m_button_selected = Button_Respawn_InitalPoint;
            break;
        case Button_TitleScene:
            m_button_selected = Button_SelectScene;
            break;
        case Button_NULL:
            m_button_selected = Button_TitleScene;
            break;
        }
        key_flag.CountTime = 0.0f;
        g_selection_hand_position.y = g_button_position[m_button_selected].y + 20.0f;
    }

    key_flag.CountTime++;

    //選択確定
    if (state.buttonA && !key_flag.ControllerButton_A)
    {
       SceneManager& sceneManager = SceneManager::GetInstance();
       Game& game = Game::GetInstance();

       game.SetCurrentGameState(GAME_STATE_RESPAWN_INITIAL);    //gameの方の処理に影響ないので、適当で大丈夫

       switch(m_button_selected)
       {
       case Button_UnPause:
          // game.SetCurrentGameState(GAME_STATE_RESPAWN_INITIAL);    //gameの方の処理に影響ないので、適当で大丈夫
           break;
       case Button_Respawn_SavePoint:
       {
           game.SetNextGameState(GAME_STATE_PAUSE_RESPAWN_SAVE_POINT);
           Player& player = Player::GetInstance();
           ItemSavePoint* registered_SavePoint = player.GetRegisteredSavePoint();
           //今のステージに登録した中間地がなかったら、前ステージの登録した中間地に行く
           if (registered_SavePoint == nullptr)
           {
               registered_SavePoint = player.GetPrevRegisteredSavePoint();
               sceneManager.SetStageName(static_cast<STAGE_NAME>(registered_SavePoint->GetSavePoint_StageID()));
               player.RegisterSavePoint(registered_SavePoint);
           }
           else
           {
               sceneManager.SetStageName(static_cast<STAGE_NAME>(registered_SavePoint->GetSavePoint_StageID()));
           }
           sceneManager.Set_Chenge_Scene_flag(true);
       }
           break;
       case Button_Respawn_InitalPoint:
       {
           game.SetNextGameState(GAME_STATE_PAUSE_RESPAWN_INITIAL);
           STAGE_NAME stage = sceneManager.GetStageName();
           if (stage == STAGE_TUTORIAL || stage == STAGE_TEST)
           {
               sceneManager.SetStageName(stage);
           }
           else { sceneManager.SetStageName(STAGE_1_1); }
           sceneManager.Set_Chenge_Scene_flag(true);
       }
           break;
       case Button_SelectScene:
           game.SetNextGameState(GAME_STATE_PAUSE_SELECT_SCENE);
           sceneManager.SetStageName(STAGE_SELECT);
           sceneManager.Set_Chenge_Scene_flag(true);
           break;
       case Button_TitleScene:
           game.SetNextGameState(GAME_STATE_PAUSE_TITLE);
           sceneManager.Set_Chenge_Scene_flag(true);
           break;
       }
       
       m_button_selected = Button_NULL;
    }

    //ポーズ押したかどうか
    if (state.start && !key_flag.ControllerButton_Start)
    {
        SceneManager& sceneManager = SceneManager::GetInstance();
        Game& game = Game::GetInstance();
        game.SetCurrentGameState(GAME_STATE_RESPAWN_INITIAL);    //gameの方の処理に影響ないので、適当で大丈夫
    }
    key_flag.ControllerButton_Start = state.start;

#ifdef _DEBUG
    //↓キーで下選択
    if (Keyboard_IsKeyDown(KK_DOWN) && key_flag.CountTime > 30.0f)
    {
        switch (m_button_selected)
        {
        case Button_UnPause:
            m_button_selected = Button_Respawn_SavePoint;
            if (!Respawn_SavePoint)
            {
                m_button_selected = Button_Respawn_InitalPoint;
            }
        break;
        case Button_Respawn_SavePoint:
            m_button_selected = Button_Respawn_InitalPoint;
            break;
        case Button_Respawn_InitalPoint:
            m_button_selected = Button_SelectScene;
            break;
        case Button_SelectScene:
            m_button_selected = Button_TitleScene;
            break;
        case Button_TitleScene:
            m_button_selected = Button_UnPause;
            break;
        case Button_NULL:
            m_button_selected = Button_UnPause;
            break;
        }
        key_flag.CountTime = 0.0f;
        g_selection_hand_position.y = g_button_position[m_button_selected].y + 20.0f;
    }
    //↑キーで上選択
    else if (Keyboard_IsKeyDown(KK_UP) && key_flag.CountTime > 30.0f)
    {
        switch (m_button_selected)
        {
        case Button_UnPause:
            m_button_selected = Button_TitleScene;
            break;
        case Button_Respawn_SavePoint:
            m_button_selected = Button_UnPause;
            break;
        case Button_Respawn_InitalPoint:
            m_button_selected = Button_Respawn_SavePoint;
            if (!Respawn_SavePoint)
            {
                m_button_selected = Button_UnPause;
            }
            break;
        case Button_SelectScene:
            m_button_selected = Button_Respawn_InitalPoint;
            break;
        case Button_TitleScene:
            m_button_selected = Button_SelectScene;
            break;
        case Button_NULL:
            m_button_selected = Button_TitleScene;
            break;
        }
        key_flag.CountTime = 0.0f;
        g_selection_hand_position.y = g_button_position[m_button_selected].y + 20.0f;
    }


    key_flag.CountTime++;

    //選択確定
    if (Keyboard_IsKeyDown(KK_SPACE) && !key_flag.KeyboardButton_Space)
    {
        SceneManager& sceneManager = SceneManager::GetInstance();
        Game& game = Game::GetInstance();

        game.SetCurrentGameState(GAME_STATE_RESPAWN_INITIAL);    //gameの方の処理に影響ないので、適当で大丈夫

        switch (m_button_selected)
        {
        case Button_UnPause:
            // game.SetCurrentGameState(GAME_STATE_RESPAWN_INITIAL);    //gameの方の処理に影響ないので、適当で大丈夫
            break;
        case Button_Respawn_SavePoint:
        {
            game.SetNextGameState(GAME_STATE_PAUSE_RESPAWN_SAVE_POINT);
            Player& player = Player::GetInstance();
            ItemSavePoint* registered_SavePoint = player.GetRegisteredSavePoint();
            //今のステージに登録した中間地がなかったら、前ステージの登録した中間地に行く
            if (registered_SavePoint == nullptr)
            {
                registered_SavePoint = player.GetPrevRegisteredSavePoint();
                sceneManager.SetStageName(static_cast<STAGE_NAME>(registered_SavePoint->GetSavePoint_StageID()));
                player.RegisterSavePoint(registered_SavePoint);
            }
            else
            {
                sceneManager.SetStageName(static_cast<STAGE_NAME>(registered_SavePoint->GetSavePoint_StageID()));
            }
            sceneManager.Set_Chenge_Scene_flag(true);
        }
        break;
        case Button_Respawn_InitalPoint:
        {
            game.SetNextGameState(GAME_STATE_PAUSE_RESPAWN_INITIAL);
            STAGE_NAME stage = sceneManager.GetStageName();
            if (stage == STAGE_TUTORIAL || stage == STAGE_TEST)
            {
                sceneManager.SetStageName(stage);
            }
            else { sceneManager.SetStageName(STAGE_1_1); }
            sceneManager.Set_Chenge_Scene_flag(true);
        }
        break;
        case Button_SelectScene:
            game.SetNextGameState(GAME_STATE_PAUSE_SELECT_SCENE);
            sceneManager.SetStageName(STAGE_SELECT);
            sceneManager.Set_Chenge_Scene_flag(true);
            break;
        case Button_TitleScene:
            game.SetNextGameState(GAME_STATE_PAUSE_TITLE);
            sceneManager.Set_Chenge_Scene_flag(true);
            break;
        }

        m_button_selected = Button_NULL;

    }

#endif // _DEBUG
}

void GamePause::Draw()
{
    //ポーズ画面背景
    // シェーダリソースを設定
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseBackground_Texture);
    DrawSpriteOld(XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER), 0.0f, XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), 0.7f);

    //ポーズ画面アイテム取得数の背景
    // シェーダリソースを設定
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseUIRecordingBackground_Texture);
    DrawSpriteOld(g_UIRecording_background_position, 0.0f, g_UIRecording_background_scale);
    
    //ポーズ画面の宝石のリング
    // シェーダリソースを設定
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseUIRecording_JewelRing_Texture);
    DrawSpriteOld(g_UIRecording_JewelRing_position, 0.0f, g_UIRecording_JewelRing_scale);

    //ポーズ画面の宝石のリング
    if (Gauge_UI::GetIfJewelCollected(BLUE))
    {
        // シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseUIRecording_Jewel_Blue_Texture);
        DrawSpriteOld(g_UIRecording_Jewel_position[0], 0.0f, g_UIRecording_Jewel_scale[0]);
    }
    if (Gauge_UI::GetIfJewelCollected(YELLOW))
    {
        // シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseUIRecording_Jewel_Yellow_Texture);
        DrawSpriteOld(g_UIRecording_Jewel_position[1], 0.0f, g_UIRecording_Jewel_scale[1]);
    }
    if (Gauge_UI::GetIfJewelCollected(RED))
    {
        // シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseUIRecording_Jewel_Red_Texture);
        DrawSpriteOld(g_UIRecording_Jewel_position[2], 0.0f, g_UIRecording_Jewel_scale[2]);
    }

    //ポーズ画面のコインUI
    // シェーダリソースを設定
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseUIRecording_CoinUI_Texture);
    DrawSpriteOld(g_UIRecording_CoinUI_position, 0.0f, g_UIRecording_CoinUI_scale);

    //ポーズ画面の数字UI
    // シェーダリソースを設定
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseUIRecording_NumberUI_Texture);
    int cnt = Current_Coin;
    // 左側の表示 現在の量
    for (int i = 0; i < 2; i++)
    {
        DrawDividedSprite({ g_UIRecording_NumberUI_Left_position.x - (i * 45), g_UIRecording_NumberUI_Left_position.y }, 0.0f, g_UIRecording_NumberUI_scale, 10, 1, cnt);
        cnt /= 10;
    }
    //　右側の表示　最大数
    int max_cnt = Max_Coin;
    for (int i = 0; i < 2; i++)
    {
        DrawDividedSprite({ g_UIRecording_NumberUI_Right_position.x - (i * 40), g_UIRecording_NumberUI_Right_position.y }, 0.0f, g_UIRecording_NumberUI_scale, 10, 1, max_cnt);
        max_cnt /= 10;
    }


    //----------------------------------------------------------------------------------------------------------------------------
    //ポーズ画面選択肢の背景
    // シェーダリソースを設定
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseSelectionBackground_Texture);
    DrawSpriteOld(g_selection_background_position, 0.0f, g_selection_background_scale);

    //選択された時のサイズ増加 (これは倍率)
    XMFLOAT2 selected_size = { 1.0f,1.0f };

    for (int i = 0; i < BUTTON_NUM; i++)
    {
        if (m_button_selected == i)
        {
            selected_size = { 1.15f,1.15f };
        }
        switch (i)
        {
        case Button_UnPause:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_UnPause_words_Texture);
            DrawSpriteOld(g_button_position[i], 0.0f, { g_button_scale[i].x * selected_size.x,g_button_scale[i].y * selected_size.y });
            break;
        case Button_Respawn_SavePoint:
            if (!Respawn_SavePoint)
            {
                // シェーダリソースを設定
                GetDeviceContext()->PSSetShaderResources(0, 1, &g_button_locked_frame_Texture);
                DrawSpriteOld(g_button_position[i], 0.0f, g_button_scale[i]);
            }
            else
            {
                // シェーダリソースを設定
                GetDeviceContext()->PSSetShaderResources(0, 1, &g_Respawn_SavePoint_words_Texture);
                DrawSpriteOld(g_button_position[i], 0.0f, { g_button_scale[i].x * selected_size.x,g_button_scale[i].y * selected_size.y });
            }
            break;        
        case Button_Respawn_InitalPoint:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Respawn_InitalPoint_words_Texture);
            DrawSpriteOld(g_button_position[i], 0.0f, { g_button_scale[i].x * selected_size.x,g_button_scale[i].y * selected_size.y });
            break;
        case Button_SelectScene:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_SelectScene_words_Texture);
            DrawSpriteOld(g_button_position[i], 0.0f, { g_button_scale[i].x * selected_size.x,g_button_scale[i].y * selected_size.y });
            break;
        case Button_TitleScene:
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_TitleScene_words_Texture);
            DrawSpriteOld(g_button_position[i], 0.0f, { g_button_scale[i].x * selected_size.x,g_button_scale[i].y * selected_size.y });
            break;
        }

        selected_size = { 1.0f, 1.0f };

        //選択肢を指す手の描画
        if (m_button_selected == i)
        {
            // シェーダリソースを設定
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_PauseSelectionHand_Texture);
            DrawSpriteOld(g_selection_hand_position, 0.0f, g_selection_hand_scale);
        }

    }

}
