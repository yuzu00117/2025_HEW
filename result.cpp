// ---------------------------------------------------------------------------------------------------- -
// #name result.cpp
// #description     リザルト画面
// #make 2025/02/02　永野義也
// #update 2024/02/27 ��橋連
// #comment 追加・修正予定
//----------------------------------------------------------------------------------------------------
#include"result.h"
#include"sprite.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"Xinput.h"
#include"keyboard.h"
#include"main.h"
#include"Item_Coin_UI.h"
#include"1-1_boss.h"
#include"gokai.h"


//テクスチャのダウンロード グローバル変数にしてる
ID3D11ShaderResourceView* g_result_Texture = NULL;
ID3D11ShaderResourceView* g_number_Texture = NULL;
ID3D11ShaderResourceView* g_colon_Texture = NULL;
ID3D11ShaderResourceView* g_gage_Texture = NULL;
ID3D11ShaderResourceView* g_gage_frame_Texture = NULL;

//スコアランクのテクスチャ
ID3D11ShaderResourceView* total_score_texture = NULL;		//トータルランクのテクスチャ格納用
ID3D11ShaderResourceView* coin_score_texture = NULL;		//コインランクのテクスチャ格納用
ID3D11ShaderResourceView* clear_time_score_texture = NULL;	//コインランクのテクスチャ格納用
ID3D11ShaderResourceView* gokai_score_texture = NULL;       //豪快ランクのテクスチャ格納用
ID3D11ShaderResourceView* g_score_rank_C_stamp_Texture = NULL;	//Cランク（スタンプ）
ID3D11ShaderResourceView* g_score_rank_B_stamp_Texture = NULL;	//Bランク（スタンプ）
ID3D11ShaderResourceView* g_score_rank_A_stamp_Texture = NULL;	//Aランク（スタンプ）
ID3D11ShaderResourceView* g_score_rank_S_stamp_Texture = NULL;	//Sランク（スタンプ）
ID3D11ShaderResourceView* g_score_rank_C_Texture = NULL;	//Cランク（スタンプ）
ID3D11ShaderResourceView* g_score_rank_B_Texture = NULL;	//Cランク（スタンプ）
ID3D11ShaderResourceView* g_score_rank_A_Texture = NULL;	//Cランク（スタンプ）
ID3D11ShaderResourceView* g_score_rank_S_Texture = NULL;	//Cランク（スタンプ）

void ResultScene::Initialize()
{
    //リザルト画面の背景テクスチャの読み込み
    g_result_Texture = InitTexture(L"asset\\texture\\result_texture\\result_back_test.png");
    //数字用テクスチャの読み込み
    g_number_Texture = InitTexture(L"asset\\texture\\result_texture\\sample_number.png");
    g_colon_Texture = InitTexture(L"asset\\texture\\result_texture\\sample_colon.png");

    //ゲージ用テクスチャ
    g_gage_Texture = InitTexture(L"asset\\texture\\result_texture\\score_gage.png");
    g_gage_frame_Texture = InitTexture(L"asset\\texture\\result_texture\\score_gage_frame.png");


    //ランクのテクスチャの読み込み
    g_score_rank_C_Texture = InitTexture(L"asset\\texture\\score_texture\\score_c.png");
    g_score_rank_B_Texture = InitTexture(L"asset\\texture\\score_texture\\score_b.png");
    g_score_rank_A_Texture = InitTexture(L"asset\\texture\\score_texture\\score_a.png");
    g_score_rank_S_Texture = InitTexture(L"asset\\texture\\score_texture\\score_s.png");

    //トータルランクのテクスチャの読み込み（スタンプ）
    g_score_rank_C_stamp_Texture = InitTexture(L"asset\\texture\\score_texture\\stamp03_c.png");
    g_score_rank_B_stamp_Texture = InitTexture(L"asset\\texture\\score_texture\\stamp03_b.png");
    g_score_rank_A_stamp_Texture = InitTexture(L"asset\\texture\\score_texture\\stamp03_a.png");
    g_score_rank_S_stamp_Texture = InitTexture(L"asset\\texture\\score_texture\\stamp03_s.png");


    //全ての音を止める
    app_atomex_stop_player();

    //リザルトのBGMをかける
    app_atomex_start(RESULT_BGM);

    //スコアの取得
    //コインの枚数を取得して、ランクに反映===============================
    m_coin_count = Item_Coin_UI::GetNowCoinCount(); //コインの枚数を取得
    int coin_score;

    //コインの枚数に応じてスコアランクのテクスチャとポイントを設定
    if (m_coin_count >= 30)
    {
        coin_score_texture = g_score_rank_A_Texture;
        coin_score = 100;
    }
    else if (m_coin_count >= 20)
    {
        coin_score_texture = g_score_rank_B_Texture;
        coin_score = 75;
    }
    else {
        coin_score_texture = g_score_rank_C_Texture;
        coin_score = 50;
    }

    //豪快度を取得して、ランクに反映する==============================
    m_gokai_count = Gokai_UI::GetNowGokaiCount(); //豪快度を取得
    int gokai_score;

    //豪快度に応じて豪快ランクのテクスチャとポイントを設定
    if (m_gokai_count >= 3000) {
        gokai_score_texture = g_score_rank_A_Texture;
        gokai_score = 100;
    }
    else if (m_gokai_count >= 2000)
    {
        gokai_score_texture = g_score_rank_B_Texture;
        gokai_score = 75;
    }
    else {
        gokai_score_texture = g_score_rank_C_Texture;
        gokai_score = 50;
    }


    //ボスのクリアタイムを取得して、ランクに反映===============================
    Boss_1_1& boss = Boss_1_1::GetInstance(); //ボスのインスタンスを取得
    m_boss_clear_time = boss.GetBossElapsedTime(); //ボスのクリアタイムを取得
    int boss_score;

    //クリアタイムに応じたランクとスコアポイントの設定
    if (m_boss_clear_time == 0)
    {
        clear_time_score_texture = g_score_rank_C_Texture;
        boss_score = 0;
    }
    else if (m_boss_clear_time >= 180.0f)
    { //3分以上
        clear_time_score_texture = g_score_rank_C_Texture;
        boss_score = 50;
    }
    else if (m_boss_clear_time >= 120.0f)
    { //2分以上
        clear_time_score_texture = g_score_rank_B_Texture;
        boss_score = 75;
    }
    else { //2分未満
        clear_time_score_texture = g_score_rank_A_Texture;
        boss_score = 100;
    }


    //トータルのスコアポイントを計算================================
    m_total_score_points = (coin_score+ gokai_score + boss_score);

    //トータルのスコアポイントに応じてランクを設定
    if (m_total_score_points >= 300)
    { //300以上ならSランク
        total_score_texture = g_score_rank_S_stamp_Texture;
    }
    else if (m_total_score_points >= 250)
    { //250以上ならAランク
        total_score_texture = g_score_rank_A_stamp_Texture;
    }
    else if (m_total_score_points >= 200)
    { //200以上ならBランク
        total_score_texture = g_score_rank_B_stamp_Texture;
    }
    else { //200以下ならCランク
        total_score_texture = g_score_rank_C_stamp_Texture;
    }
}

void ResultScene::Update()
{
    //コントローラーの入力の受け取り
    ControllerState state = GetControllerInput();

    if (Keyboard_IsKeyDown(KK_SPACE) || (state.buttonA))
    {
        SceneManager& sceneManager = SceneManager::GetInstance();
        sceneManager.ChangeScene(SCENE_STAGE_SELECT);
    }

    switch (m_state)
    {
    case STATE_RESULT_COIN:
        if (m_coin_pos.x >= m_coin_pos_finish.x)
        {
            m_coin_pos.x -= m_ui_move_speed;
            m_coin_alpha += 0.01 * m_ui_move_speed;
        }
        else if (m_coin_score_alpha <= 1)
        {
            m_coin_score_alpha += 0.01 * m_score_alpha_move_speed;
        }
        else
        {
            m_state = STATE_RESULT_DYNAMIC;
        }
        break;
    case STATE_RESULT_DYNAMIC:
        if (m_gokai_pos.x >= m_gokai_pos_finish.x)
        {
            m_gokai_pos.x -= m_ui_move_speed;
            m_gokai_alpha += 0.01 * m_ui_move_speed;
        }
        else if (m_gokai_score_alpha <= 1)
        {
            m_gokai_score_alpha += 0.01 * m_score_alpha_move_speed;
        }
        else
        {
            m_state = STATE_RESULT_BOSS;
        }
        break;
    case STATE_RESULT_BOSS:
        if (m_boss_pos.x >= m_boss_pos_finish.x)
        {
            m_boss_pos.x -= m_ui_move_speed;
            m_boss_alpha += 0.01 * m_ui_move_speed;
        }
        else if (m_boss_score_alpha <= 1)
        {
            m_boss_score_alpha += 0.01 * m_score_alpha_move_speed;
        }
        else
        {
            m_state = STATE_RESULT_TOTAL;
        }
        break;
    case STATE_RESULT_TOTAL:
        if (m_total_gage_filling <= 1)
        {
            m_total_gage_filling += 0.01 * m_gage_move_speed;
        }
        else if (m_total_score_alpha <= 1)
        {
            m_total_score_alpha += 0.01 * m_score_alpha_move_speed;
        }
        break;
    default: 
        break;
    }
}

void ResultScene::Draw()
{

    //バッファクリア
    Clear();

    //2D描画なので深度無効
    SetDepthEnable(false);

    {//リザルト画面の背景の描画
        // シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_result_Texture);

        DrawSpriteOld(
            XMFLOAT2(SCREEN_XCENTER, SCREEN_YCENTER),
            0.0f,
            XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT)
        );
    }


    {//コインのランクを描画
        //コインの獲得枚数を表示
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_number_Texture);
        int num = m_coin_count;
        int cnt = 0;
        while (1)
        {
            DrawSpriteAnimOld(
                XMFLOAT2(m_coin_pos.x - (NUMBER_SIZE * cnt), m_coin_pos.y),
                0,
                XMFLOAT2(NUMBER_SIZE, NUMBER_SIZE),
                10, 1, num % 10, m_coin_alpha
            );
            num /= 10;
            cnt++;
            if (num == 0)
            {
                break;
            }
        };

        //シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &coin_score_texture);

        DrawSpriteOld(
            XMFLOAT2(600, 160), //一旦マジックナンバーで管理。後で変える
            0.0f,
            XMFLOAT2(RANK_SIZE, RANK_SIZE), m_coin_score_alpha
        );
    }

    {//豪快度のランクを描画
        //豪快度の数値を表示
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_number_Texture);
        int num = m_gokai_count;
        int cnt = 0;
        while (1)
        {
            DrawSpriteAnimOld(
                XMFLOAT2(m_gokai_pos.x - (NUMBER_SIZE * cnt), m_gokai_pos.y),
                0,
                XMFLOAT2(NUMBER_SIZE, NUMBER_SIZE),
                10, 1, num % 10, m_gokai_alpha
            );
            num /= 10;
            cnt++;
            if (num == 0)
            {
                break;
            }
        };

        //シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &gokai_score_texture);

        DrawSpriteOld(
            XMFLOAT2(600, 240), //一旦マジックナンバーで管理。後で変える
            0.0f,
            XMFLOAT2(RANK_SIZE, RANK_SIZE), m_gokai_score_alpha
        );
    }

    {//ボスのクリアタイムランクを描画
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_number_Texture);
        int sec = m_boss_clear_time % 60;
        int cnt = 0;
        while (1)
        {
            DrawSpriteAnimOld(
                XMFLOAT2(m_boss_pos.x - (NUMBER_SIZE * cnt), m_boss_pos.y),
                0,
                XMFLOAT2(NUMBER_SIZE, NUMBER_SIZE),
                10, 1, sec % 10, m_boss_alpha
            );
            sec /= 10;
            cnt++;
            if (sec == 0)
            {
                break;
            }
        };

        {//コロンで区切る
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_colon_Texture);
            DrawSpriteOld(
                XMFLOAT2(m_boss_pos.x - (NUMBER_SIZE * cnt), m_boss_pos.y),
                0,
                XMFLOAT2(NUMBER_SIZE, NUMBER_SIZE), m_boss_alpha
            );
            cnt++;
        }

        GetDeviceContext()->PSSetShaderResources(0, 1, &g_number_Texture);
        int min = m_boss_clear_time / 60;
        while (1)
        {
            DrawSpriteAnimOld(
                XMFLOAT2(m_boss_pos.x - (NUMBER_SIZE * cnt), m_boss_pos.y),
                0,
                XMFLOAT2(NUMBER_SIZE, NUMBER_SIZE),
                10, 1, min % 10, m_boss_alpha
            );
            min /= 10;
            cnt++;
            if (min == 0)
            {
                break;
            }
        };

        //シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &clear_time_score_texture);

        DrawSpriteOld(
            XMFLOAT2(600, 330), //一旦マジックナンバーで管理。後で変える
            0.0f,
            XMFLOAT2(RANK_SIZE, RANK_SIZE), m_boss_score_alpha
        );
    }

    {//トータルスコアのゲージ表示
        //ゲージ表示
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_gage_Texture);
        float size = m_total_score_points / 300.0f;
        DrawSpriteOld(
            XMFLOAT2(110+(250.0f * size * m_total_gage_filling), 440), //一旦マジックナンバーで管理。後で変える
            0.0f,
            XMFLOAT2(500.0f * (size) * m_total_gage_filling, 30.0f)
        );

        //ゲージのフレーム表示
        GetDeviceContext()->PSSetShaderResources(0, 1, &g_gage_frame_Texture);

        DrawSpriteOld(
            XMFLOAT2(360, 440), //一旦マジックナンバーで管理。後で変える
            0.0f,
            XMFLOAT2(500, 50)
        );
    }

    {//トータルのランクを描画
        //シェーダリソースを設定
        GetDeviceContext()->PSSetShaderResources(0, 1, &total_score_texture);

        DrawSpriteOld(
            XMFLOAT2(185, 580), //一旦マジックナンバーで管理。後で変える
            0.0f,
            XMFLOAT2(TOTAL_RANK_SIZE, TOTAL_RANK_SIZE), m_total_score_alpha
        );
    }

    //バックバッファ、フロントバッファ入れ替え
    Present();

}

void ResultScene::Finalize()
{
    if (g_result_Texture) UnInitTexture(g_result_Texture);
    if (g_number_Texture) UnInitTexture(g_number_Texture);
    if (g_gage_Texture) UnInitTexture(g_gage_Texture);
    if (g_gage_frame_Texture) UnInitTexture(g_gage_frame_Texture);
    if (g_colon_Texture) UnInitTexture(g_colon_Texture);

    if (total_score_texture) UnInitTexture(total_score_texture);
    if (coin_score_texture) UnInitTexture(coin_score_texture);
    if (clear_time_score_texture) UnInitTexture(clear_time_score_texture);
    if (gokai_score_texture) UnInitTexture(gokai_score_texture);

    if (g_score_rank_C_stamp_Texture) UnInitTexture(g_score_rank_C_stamp_Texture);
    if (g_score_rank_B_stamp_Texture) UnInitTexture(g_score_rank_B_stamp_Texture);
    if (g_score_rank_A_stamp_Texture) UnInitTexture(g_score_rank_A_stamp_Texture);
    if (g_score_rank_S_stamp_Texture) UnInitTexture(g_score_rank_S_stamp_Texture);

    if (g_score_rank_C_Texture) UnInitTexture(g_score_rank_C_Texture);
    if (g_score_rank_B_Texture) UnInitTexture(g_score_rank_B_Texture);
    if (g_score_rank_A_Texture) UnInitTexture(g_score_rank_A_Texture);
    if (g_score_rank_S_Texture) UnInitTexture(g_score_rank_S_Texture);

}