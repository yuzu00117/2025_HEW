//-----------------------------------------------------------------------------------------------------
// #name bg.cpp
// #description 背景処理
// #make 2024/12/12　　今村友哉
// #update 2024/12/12
// #comment  
//			　背景の大きさ1280*720だと多分隙間あかない（多分）
//          
//----------------------------------------------------------------------------------------------------

#include "sprite.h"
#include "texture.h"
#include "renderer.h"
#include "Bg.h"
#include "player_position.h"


static ID3D11ShaderResourceView* g_Bg_Texture1 = NULL;//背景のテクスチャ
static ID3D11ShaderResourceView* g_Bg_Texture2 = NULL;//背景のテクスチャ
static ID3D11ShaderResourceView* g_Bg_Texture3 = NULL;//背景のテクスチャ
static ID3D11ShaderResourceView* g_Bg_Texture4 = NULL;//背景のテクスチャ
static ID3D11ShaderResourceView* g_Bg_Texture5 = NULL;//背景のテクスチャ



// プレイヤーの過去の座標
b2Vec2 g_old_player_position;

// 各背景のスクロール速度
static float g_ScrollSpeed[3] = { 0.5f, 0.8f, 1.0f };

// UVスクロール用Tu座標
static float g_UvScrollTu = 0.0f;

// 背景インスタンス
Bg bg;

#define BACK_GROUND_HEIGHT (-350)

void Bg::Initialize()
{
    // 各背景テクスチャの読み込み
    g_Bg_Texture1 = InitTexture(L"asset\\texture\\stage1_1\\background1.png");
    g_Bg_Texture2 = InitTexture(L"asset\\texture\\stage1_1\\background5.png");
    g_Bg_Texture3 = InitTexture(L"asset\\texture\\stage1_1\\background4.png");
    g_Bg_Texture4 = InitTexture(L"asset\\texture\\stage1_1\\background3.png");
    g_Bg_Texture5 = InitTexture(L"asset\\texture\\stage1_1\\background2.png");

    
   
    
   // 背景画像の初期配置（4枚に増やす）
    bg.texture_1_pos[0] = XMFLOAT2(0.0f, BACK_GROUND_HEIGHT);
    bg.texture_1_pos[1] = XMFLOAT2(SCREEN_WIDTH*2, BACK_GROUND_HEIGHT);
    bg.texture_1_pos[2] = XMFLOAT2(SCREEN_WIDTH * 2*2, BACK_GROUND_HEIGHT);
    bg.texture_1_pos[3] = XMFLOAT2(SCREEN_WIDTH * 3*2, BACK_GROUND_HEIGHT);


    bg.texture_2_pos[0] = XMFLOAT2(0.0f, BACK_GROUND_HEIGHT);
    bg.texture_2_pos[1] = XMFLOAT2(SCREEN_WIDTH*2, BACK_GROUND_HEIGHT);
    bg.texture_2_pos[2] = XMFLOAT2(SCREEN_WIDTH * 2*2, BACK_GROUND_HEIGHT);
    bg.texture_2_pos[3] = XMFLOAT2(SCREEN_WIDTH * 3*2, BACK_GROUND_HEIGHT);


    bg.texture_3_pos[0] = XMFLOAT2(0.0f, BACK_GROUND_HEIGHT);
    bg.texture_3_pos[1] = XMFLOAT2(SCREEN_WIDTH * 2, BACK_GROUND_HEIGHT);
    bg.texture_3_pos[2] = XMFLOAT2(SCREEN_WIDTH * 2 * 2, BACK_GROUND_HEIGHT);
    bg.texture_3_pos[3] = XMFLOAT2(SCREEN_WIDTH * 3 * 2, BACK_GROUND_HEIGHT);


    bg.texture_4_pos[0] = XMFLOAT2(0.0f, BACK_GROUND_HEIGHT);
    bg.texture_4_pos[1] = XMFLOAT2(SCREEN_WIDTH * 2, BACK_GROUND_HEIGHT);
    bg.texture_4_pos[2] = XMFLOAT2(SCREEN_WIDTH * 2 * 2, BACK_GROUND_HEIGHT);
    bg.texture_4_pos[3] = XMFLOAT2(SCREEN_WIDTH * 3 * 2, BACK_GROUND_HEIGHT);


    bg.texture_5_pos[0] = XMFLOAT2(0.0f, BACK_GROUND_HEIGHT);
    bg.texture_5_pos[1] = XMFLOAT2(SCREEN_WIDTH * 2, BACK_GROUND_HEIGHT);
    bg.texture_5_pos[2] = XMFLOAT2(SCREEN_WIDTH * 2 * 2, BACK_GROUND_HEIGHT);
    bg.texture_5_pos[3] = XMFLOAT2(SCREEN_WIDTH * 3 * 2, BACK_GROUND_HEIGHT);
}

void Bg::Update()
{
    // プレイヤーの現在位置を取得
    b2Vec2 player_position = PlayerPosition::GetPlayerPosition();

    // テクスチャごとのスクロール速度
    float textureScrollSpeeds[] = { 0.1f, 0.3f, 0.7f,1.0 ,1.5};

    // 背景1のスクロール処理
    for (int i = 0; i < 4; i++)
    {
        bg.texture_1_pos[i].x -= (player_position.x - g_old_player_position.x) * textureScrollSpeeds[0] * 100.0f;

        // 左にループ処理
        if (bg.texture_1_pos[i].x < -SCREEN_WIDTH*2)
            bg.texture_1_pos[i].x += SCREEN_WIDTH*2 * 4.0f;

        // 右にループ処理
        if (bg.texture_1_pos[i].x > SCREEN_WIDTH*2 * 3.0f)
            bg.texture_1_pos[i].x -= SCREEN_WIDTH *2* 4.0f;
    }

    // 背景2のスクロール処理
    for (int i = 0; i < 4; i++)
    {
        bg.texture_2_pos[i].x -= (player_position.x - g_old_player_position.x) * textureScrollSpeeds[1] * 100.0f;

        // 左にループ処理
        if (bg.texture_2_pos[i].x < -SCREEN_WIDTH*2)
            bg.texture_2_pos[i].x += SCREEN_WIDTH*2 * 4.0f;

        // 右にループ処理
        if (bg.texture_2_pos[i].x > SCREEN_WIDTH*2 * 3.0f)
            bg.texture_2_pos[i].x -= SCREEN_WIDTH*2 * 4.0f;
    }

    // 背景2のスクロール処理
    for (int i = 0; i < 4; i++)
    {
        bg.texture_3_pos[i].x -= (player_position.x - g_old_player_position.x) * textureScrollSpeeds[2] * 100.0f;

        // 左にループ処理
        if (bg.texture_3_pos[i].x < -SCREEN_WIDTH * 2)
            bg.texture_3_pos[i].x += SCREEN_WIDTH * 2 * 4.0f;

        // 右にループ処理
        if (bg.texture_3_pos[i].x > SCREEN_WIDTH * 2 * 3.0f)
            bg.texture_3_pos[i].x -= SCREEN_WIDTH * 2 * 4.0f;
    }

    // 背景2のスクロール処理
    for (int i = 0; i < 4; i++)
    {
        bg.texture_4_pos[i].x -= (player_position.x - g_old_player_position.x) * textureScrollSpeeds[3] * 100.0f;

        // 左にループ処理
        if (bg.texture_4_pos[i].x < -SCREEN_WIDTH * 2)
            bg.texture_4_pos[i].x += SCREEN_WIDTH * 2 * 4.0f;

        // 右にループ処理
        if (bg.texture_4_pos[i].x > SCREEN_WIDTH * 2 * 3.0f)
            bg.texture_4_pos[i].x -= SCREEN_WIDTH * 2 * 4.0f;
    }


    // 背景2のスクロール処理
    for (int i = 0; i < 4; i++)
    {
        bg.texture_5_pos[i].x -= (player_position.x - g_old_player_position.x) * textureScrollSpeeds[4] * 100.0f;

        // 左にループ処理
        if (bg.texture_5_pos[i].x < -SCREEN_WIDTH * 2)
            bg.texture_5_pos[i].x += SCREEN_WIDTH * 2 * 4.0f;

        // 右にループ処理
        if (bg.texture_5_pos[i].x > SCREEN_WIDTH * 2 * 3.0f)
            bg.texture_5_pos[i].x -= SCREEN_WIDTH * 2 * 4.0f;
    }

    // プレイヤー座標の更新
    g_old_player_position = player_position;
}

void Bg::Draw()
{


    // 背景テクスチャの描画
    ID3D11ShaderResourceView* textures[] = { g_Bg_Texture1, g_Bg_Texture2, g_Bg_Texture3 ,g_Bg_Texture4,g_Bg_Texture5};

    // 背景1の描画
    for (int i = 0; i < 4; i++)
    {
        GetDeviceContext()->PSSetShaderResources(0, 1, &textures[0]);
        DrawSprite(bg.texture_1_pos[i], 0.0f, XMFLOAT2(SCREEN_WIDTH*2, SCREEN_HEIGHT*2));
    }

    // 背景2の描画
    for (int i = 0; i < 4; i++)
    {
        GetDeviceContext()->PSSetShaderResources(0, 1, &textures[1]);
        DrawSprite(bg.texture_2_pos[i], 0.0f, XMFLOAT2(SCREEN_WIDTH*2, SCREEN_HEIGHT*2));
    }
    // 背景2の描画
    for (int i = 0; i < 4; i++)
    {
        GetDeviceContext()->PSSetShaderResources(0, 1, &textures[2]);
        DrawSprite(bg.texture_3_pos[i], 0.0f, XMFLOAT2(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2));
    }

    // 背景2の描画
    for (int i = 0; i < 4; i++)
    {
        GetDeviceContext()->PSSetShaderResources(0, 1, &textures[3]);
        DrawSprite(bg.texture_3_pos[i], 0.0f, XMFLOAT2(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2));
    }

    // 背景2の描画
    for (int i = 0; i < 4; i++)
    {
        GetDeviceContext()->PSSetShaderResources(0, 1, &textures[4]);
        DrawSprite(bg.texture_3_pos[i], 0.0f, XMFLOAT2(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2));
    }
}

void Bg::Finalize()
{
    // 各背景テクスチャの解放
    UnInitTexture(g_Bg_Texture1);
    UnInitTexture(g_Bg_Texture2);
    UnInitTexture(g_Bg_Texture3);
    UnInitTexture(g_Bg_Texture4);
    UnInitTexture(g_Bg_Texture5);
}
