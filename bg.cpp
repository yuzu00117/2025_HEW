//-----------------------------------------------------------------------------------------------------
// #name bg.cpp
// #description 背景処理
// #make 2025/02/03　　永野義也
// #update 2025/02/03
// #comment  
//			　背景の大きさ1280*720だと多分隙間あかない（多分）
//          
//----------------------------------------------------------------------------------------------------

#include "sprite.h"
#include "texture.h"
#include "renderer.h"
#include "Bg.h"
#include "player_position.h"


static ID3D11ShaderResourceView* g_Bg_Texture[4] = { NULL };  // 背景テクスチャ（配列化）
static ID3D11ShaderResourceView* g_Bg_Texture_light = NULL;  // ライト用
static ID3D11ShaderResourceView* g_Bg_Texture_Most = NULL;  // ライト用テクスチャ

// プレイヤーの過去の座標
b2Vec2 g_old_player_position;

// 背景インスタンス
Bg bg;

#define BACK_GROUND_HEIGHT (50)
#define BACK_GROUND_SIZE_X (1280*1.0)
#define BACK_GROUND_SIZE_Y (720*1.2)

void Bg::Initialize()
{
    // 各背景テクスチャの読み込み（配列に統一）
    g_Bg_Texture[0] = InitTexture(L"asset\\texture\\stage1_1\\background_2.png");
    g_Bg_Texture[1] = InitTexture(L"asset\\texture\\stage1_1\\background_3.png");
    g_Bg_Texture[2] = InitTexture(L"asset\\texture\\stage1_1\\background_4.png");
    g_Bg_Texture[3] = InitTexture(L"asset\\texture\\stage1_1\\background_5.png");

    g_Bg_Texture_light = InitTexture(L"asset\\texture\\stage1_1\\background_light.png");


    g_Bg_Texture_Most = InitTexture(L"asset\\texture\\stage1_1\\background_1.png");
    // 背景画像の初期配置（配列化）
    for (int layer = 0; layer < 4; layer++)
    {
        float sizeMultiplier = (layer == 0) ? 1.5f : 1.0f;
        for (int i = 0; i < 4; i++)
        {
            bg.texture_pos[layer][i] = XMFLOAT2(BACK_GROUND_SIZE_X * sizeMultiplier * i, BACK_GROUND_HEIGHT * sizeMultiplier);
        }
    }
}

void Bg::Update()
{
    // プレイヤーの現在位置を取得
    b2Vec2 player_position = PlayerPosition::GetPlayerPosition();

    // テクスチャごとのスクロール速度
    float textureScrollSpeedsX[] = { 0.1f, 0.3f, 0.7f, 1.0f };
    float textureScrollSpeedY[] = { 0.01f, 0.1f, 0.2f, 0.3f }; // 縦スクロールの速度（調整可）

    // 背景のスクロール処理（X 軸 & Y 軸）
    for (int layer = 0; layer < 4; layer++)
    {
        float sizeMultiplier = (layer == 0) ? 1.5f : 1.0f;
        for (int i = 0; i < 4; i++)
        {
            // X 軸スクロール
            bg.texture_pos[layer][i].x -= (player_position.x - g_old_player_position.x) * textureScrollSpeedsX[layer] * 100.0f;

            // 左にループ処理
            if (bg.texture_pos[layer][i].x < -BACK_GROUND_SIZE_X * sizeMultiplier)
                bg.texture_pos[layer][i].x += BACK_GROUND_SIZE_X * sizeMultiplier * 4.0f;

            // 右にループ処理
            if (bg.texture_pos[layer][i].x > BACK_GROUND_SIZE_X * sizeMultiplier * 3.0f)
                bg.texture_pos[layer][i].x -= BACK_GROUND_SIZE_X * sizeMultiplier * 4.0f;

            // Y 軸スクロール（調整追加）
            bg.texture_pos[layer][i].y -= (player_position.y - g_old_player_position.y) * textureScrollSpeedY[layer] * 100.0f;

          
        }
    }

    // プレイヤー座標の更新
    g_old_player_position = player_position;
}

void Bg::Draw()
{

    // ライトテクスチャの描画（背景の影響を受ける）
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_Bg_Texture_Most);
    DrawSpriteOld({ BACK_GROUND_SIZE_X / 2, BACK_GROUND_SIZE_Y / 2 }, 0.0f, { BACK_GROUND_SIZE_X, BACK_GROUND_SIZE_Y }, 1.0f);
    // 背景テクスチャの描画
    for (int layer = 0; layer < 4; layer++)  // 4層分を描画
    {
        float sizeMultiplier = (layer == 0) ? 1.5f : 1.0f;
        for (int i = 0; i < 4; i++)
        {
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Bg_Texture[layer]);
            DrawSprite(bg.texture_pos[layer][i], 0.0f, XMFLOAT2(BACK_GROUND_SIZE_X * sizeMultiplier, BACK_GROUND_SIZE_Y * sizeMultiplier));
        }
    }

    // ライトテクスチャの描画（背景の影響を受ける）
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_Bg_Texture_light);
    DrawSpriteOld({ BACK_GROUND_SIZE_X / 2, BACK_GROUND_SIZE_Y / 2 }, 0.0f, { BACK_GROUND_SIZE_X, BACK_GROUND_SIZE_Y }, 1.0f);
}

void Bg::Finalize()
{
    // 各背景テクスチャの解放
    for (int i = 0; i < 4; i++)
    {
        UnInitTexture(g_Bg_Texture[i]);
    }
}

void Bg::FrontDraw()
{

}


