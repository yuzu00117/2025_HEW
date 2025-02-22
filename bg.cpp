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
#include"scene.h"


static ID3D11ShaderResourceView* g_Bg_Texture[4] = { NULL };  // 背景テクスチャ（配列化）
static ID3D11ShaderResourceView* g_Bg_Texture_light = NULL;  // ライト用
static ID3D11ShaderResourceView* g_Bg_Texture_Most = NULL;  // ライト用テクスチャ

// プレイヤーの過去の座標
b2Vec2 g_old_player_position;

// 背景インスタンス
Bg bg;

//tutorial
#define TUTORIAL_BACK_GROUND_HEIGHT (0)
#define TUTORIAL_BACK_GROUND_SIZE_X (1280*1.1)
#define TUTORIAL_BACK_GROUND_SIZE_Y (720*1.1)

//stage1_1森
#define STAGE_1_1_FOREST_BACK_GROUND_HEIGHT (50)
#define STAGE_1_1_FOREST_BACK_GROUND_SIZE_X (1280*1.7)
#define STAGE_1_1_FOREST_BACK_GROUND_SIZE_Y (720*1.7)

//stage1_1遺跡
#define STAGE_1_1_REMAINS_BACK_GROUND_HEIGHT (50)
#define STAGE_1_1_REMAINS_BACK_GROUND_SIZE_X (1280*1.6)
#define STAGE_1_1_REMAINS_BACK_GROUND_SIZE_Y (720*1.6)


//stage1_1遺跡
#define STAGE_1_1_BOSS_BACK_GROUND_HEIGHT (50)
#define STAGE_1_1_BOSS_BACK_GROUND_SIZE_X (1280*1.0)
#define STAGE_1_1_BOSS_BACK_GROUND_SIZE_Y (720*1.0)



void Bg::Initialize()
{


    //スクロールの高さなどをリセット
    g_old_player_position = b2Vec2_zero;
    // 各背景テクスチャの読み込み（配列に統一）
    
    SceneManager& sceneManager = SceneManager::GetInstance();

    int height=0;
    int width = 0;

    switch (sceneManager.GetStageName())
    {
    case STAGE_TUTORIAL: // チュートリアル
        g_Bg_Texture[0] = InitTexture(L"asset\\texture\\tutorial\\background_1.png");
        g_Bg_Texture[1] = InitTexture(L"asset\\texture\\sample_texture\\invisibility_wall.png");//透明な壁のテクスチャ
        g_Bg_Texture[2] = InitTexture(L"asset\\texture\\tutorial\\background_2.png");
        g_Bg_Texture[3] = InitTexture(L"asset\\texture\\tutorial\\background_3.png");

        g_Bg_Texture_light = InitTexture(L"asset\\texture\\stage1_1\\background_light.png");
        g_Bg_Texture_Most = InitTexture(L"asset\\texture\\stage1_1\\background_1.png");

        height = TUTORIAL_BACK_GROUND_HEIGHT;
        width = TUTORIAL_BACK_GROUND_SIZE_X;
        break;

    case STAGE_1_1: // 1-1森
        g_Bg_Texture[0] = InitTexture(L"asset\\texture\\stage1_1\\background_2.png");
        g_Bg_Texture[1] = InitTexture(L"asset\\texture\\stage1_1\\background_3.png");
        g_Bg_Texture[2] = InitTexture(L"asset\\texture\\stage1_1\\background_4.png");
        g_Bg_Texture[3] = InitTexture(L"asset\\texture\\stage1_1\\background_5.png");

        g_Bg_Texture_light = InitTexture(L"asset\\texture\\stage1_1\\background_light.png");


        g_Bg_Texture_Most = InitTexture(L"asset\\texture\\stage1_1\\background_1.png");
        height= STAGE_1_1_FOREST_BACK_GROUND_HEIGHT;
        width = STAGE_1_1_FOREST_BACK_GROUND_SIZE_X;
        break;

    case STAGE_ISEKI: // 1-1遺跡
        g_Bg_Texture[0] = InitTexture(L"asset\\texture\\stage1_1\\remains_background02.png");
        g_Bg_Texture[1] = InitTexture(L"asset\\texture\\stage1_1\\remains_background03.png");

        g_Bg_Texture_Most = InitTexture(L"asset\\texture\\stage1_1\\remains_background01.png");
        height= STAGE_1_1_REMAINS_BACK_GROUND_HEIGHT;
        width = STAGE_1_1_REMAINS_BACK_GROUND_SIZE_X;
        break;


    case STAGE_BOSS: // ボス戦
        g_Bg_Texture[0] = InitTexture(L"asset\\texture\\stage1_1\\remains_background02.png");
        g_Bg_Texture[1] = InitTexture(L"asset\\texture\\stage1_1\\remains_background03.png");

        g_Bg_Texture_Most = InitTexture(L"asset\\texture\\stage1_1\\remains_background01.png");
        height = STAGE_1_1_BOSS_BACK_GROUND_HEIGHT;
        width = STAGE_1_1_BOSS_BACK_GROUND_SIZE_X;
        break;
    default:
        g_Bg_Texture[0] = InitTexture(L"asset\\texture\\stage1_1\\background_2.png");
        g_Bg_Texture[1] = InitTexture(L"asset\\texture\\stage1_1\\background_3.png");
        g_Bg_Texture[2] = InitTexture(L"asset\\texture\\stage1_1\\background_4.png");
        g_Bg_Texture[3] = InitTexture(L"asset\\texture\\stage1_1\\background_5.png");

        g_Bg_Texture_light = InitTexture(L"asset\\texture\\stage1_1\\background_light.png");


        g_Bg_Texture_Most = InitTexture(L"asset\\texture\\stage1_1\\background_1.png");
        height = STAGE_1_1_FOREST_BACK_GROUND_HEIGHT;
        width = STAGE_1_1_FOREST_BACK_GROUND_SIZE_X;
        break;
    }

   
    // 背景画像の初期配置（配列化）
    for (int layer = 0; layer < 4; layer++)
    {
        float sizeMultiplier = (layer == 0) ? 1.5f : 1.0f;
        for (int i = 0; i < 4; i++)
        {
            bg.texture_pos[layer][i] = XMFLOAT2(width * sizeMultiplier * i, height * sizeMultiplier);
        }
    }
}
void Bg::Update()
{
    // プレイヤーの現在位置を取得
    b2Vec2 player_position = PlayerPosition::GetPlayerPosition();

    // テクスチャごとのスクロール速度
    float textureScrollSpeedsX[] = { 0.1f, 0.3f, 0.7f, 1.0f };
    float textureScrollSpeedsY[] = { 0.01f, 0.1f, 0.2f, 0.3f };

    float BACK_GROUND_SIZE_X, BACK_GROUND_SIZE_Y;
    SceneManager& sceneManager = SceneManager::GetInstance();
    bool isBossStage = false;
    float bossScrollCorrection = 1.0f;  // 🔴 ボスステージ用の補正値（デフォルトは1.0）

    switch (sceneManager.GetStageName())
    {
    case STAGE_TUTORIAL:
        BACK_GROUND_SIZE_X = TUTORIAL_BACK_GROUND_SIZE_X;
        BACK_GROUND_SIZE_Y = TUTORIAL_BACK_GROUND_SIZE_Y;
        break;

    case STAGE_1_1:
        BACK_GROUND_SIZE_X = STAGE_1_1_FOREST_BACK_GROUND_SIZE_X;
        BACK_GROUND_SIZE_Y = STAGE_1_1_FOREST_BACK_GROUND_SIZE_Y;
        break;

    case STAGE_ISEKI:
        BACK_GROUND_SIZE_X = STAGE_1_1_REMAINS_BACK_GROUND_SIZE_X;
        BACK_GROUND_SIZE_Y = STAGE_1_1_REMAINS_BACK_GROUND_SIZE_Y;
        break;

    case STAGE_BOSS:
        BACK_GROUND_SIZE_X = STAGE_1_1_REMAINS_BACK_GROUND_SIZE_X;
        BACK_GROUND_SIZE_Y = STAGE_1_1_REMAINS_BACK_GROUND_SIZE_Y;
        isBossStage = true;
        bossScrollCorrection = 3.5f;  // 🔴 ボスステージ用のY軸スクロール補正（例: 1.5倍）
        break;

    default:
        BACK_GROUND_SIZE_X = STAGE_1_1_FOREST_BACK_GROUND_SIZE_X;
        BACK_GROUND_SIZE_Y = STAGE_1_1_FOREST_BACK_GROUND_SIZE_Y;
        break;
    }

    // 背景のスクロール処理（X 軸 & Y 軸）
    for (int layer = 0; layer < 4; layer++)
    {
        float sizeMultiplier = (layer == 0) ? 1.5f : 1.0f;
        for (int i = 0; i < 4; i++)
        {
            // X 軸スクロール（全ステージ適用）
            bg.texture_pos[layer][i].x -= (player_position.x - g_old_player_position.x) * textureScrollSpeedsX[layer] * 100.0f;

            // 左にループ処理
            if (bg.texture_pos[layer][i].x < -BACK_GROUND_SIZE_X * sizeMultiplier)
                bg.texture_pos[layer][i].x += BACK_GROUND_SIZE_X * sizeMultiplier * 4.0f;

            // 右にループ処理
            if (bg.texture_pos[layer][i].x > BACK_GROUND_SIZE_X * sizeMultiplier * 3.0f)
                bg.texture_pos[layer][i].x -= BACK_GROUND_SIZE_X * sizeMultiplier * 4.0f;

            // Y 軸スクロール（ボスステージのみ補正をかける）
            if (isBossStage)
            {
                bg.texture_pos[layer][i].y -= (player_position.y - g_old_player_position.y) * textureScrollSpeedsY[layer] * 100.0f * bossScrollCorrection;

                // 縦スクロールのループ処理（ボスステージのみ）
                if (bg.texture_pos[layer][i].y < -BACK_GROUND_SIZE_Y * sizeMultiplier)
                    bg.texture_pos[layer][i].y += BACK_GROUND_SIZE_Y * sizeMultiplier * 4.0f;

                if (bg.texture_pos[layer][i].y > BACK_GROUND_SIZE_Y * sizeMultiplier * 3.0f)
                    bg.texture_pos[layer][i].y -= BACK_GROUND_SIZE_Y * sizeMultiplier * 4.0f;
            }
            else
            {
                bg.texture_pos[layer][i].y -= (player_position.y - g_old_player_position.y) * textureScrollSpeedsY[layer] * 100.0f;
            }
        }
    }

    // プレイヤー座標の更新
    g_old_player_position = player_position;
}



void Bg::Draw()
{
    float BACK_GROUND_SIZE_X, BACK_GROUND_SIZE_Y;
    SceneManager& sceneManager = SceneManager::GetInstance();
    bool isBossStage = false;

    switch (sceneManager.GetStageName())
    {
    case STAGE_TUTORIAL:
        BACK_GROUND_SIZE_X = TUTORIAL_BACK_GROUND_SIZE_X;
        BACK_GROUND_SIZE_Y = TUTORIAL_BACK_GROUND_SIZE_Y;
        break;

    case STAGE_1_1:
        BACK_GROUND_SIZE_X = STAGE_1_1_FOREST_BACK_GROUND_SIZE_X;
        BACK_GROUND_SIZE_Y = STAGE_1_1_FOREST_BACK_GROUND_SIZE_Y;
        break;

    case STAGE_ISEKI:
        BACK_GROUND_SIZE_X = STAGE_1_1_REMAINS_BACK_GROUND_SIZE_X;
        BACK_GROUND_SIZE_Y = STAGE_1_1_REMAINS_BACK_GROUND_SIZE_Y;
        break;

    case STAGE_BOSS:  // ボスステージのみ縦の層を追加
        BACK_GROUND_SIZE_X = STAGE_1_1_BOSS_BACK_GROUND_SIZE_X;
        BACK_GROUND_SIZE_Y = STAGE_1_1_BOSS_BACK_GROUND_SIZE_Y;
        isBossStage = true;
        break;

    default:
        BACK_GROUND_SIZE_X = TUTORIAL_BACK_GROUND_SIZE_X;
        BACK_GROUND_SIZE_Y = TUTORIAL_BACK_GROUND_SIZE_Y;
        break;
    }

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

            // 🔴 ボスステージのみ縦方向にも層を追加して描画
            if (isBossStage)
            {
                for (int j = 0; j < 4; j++)  // 縦に4枚描画
                {
                    XMFLOAT2 position = XMFLOAT2(bg.texture_pos[layer][i].x, bg.texture_pos[layer][i].y + BACK_GROUND_SIZE_Y * sizeMultiplier * j);

                    DrawBgSprite(position, 0.0f, XMFLOAT2(BACK_GROUND_SIZE_X * sizeMultiplier, BACK_GROUND_SIZE_Y * sizeMultiplier));
                }
            }
            else
            {
                // 通常の1回のみ描画
                DrawBgSprite(bg.texture_pos[layer][i], 0.0f, XMFLOAT2(BACK_GROUND_SIZE_X * sizeMultiplier, BACK_GROUND_SIZE_Y * sizeMultiplier));
            }
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
        g_Bg_Texture[i] = NULL;
    }
    
}

void Bg::FrontDraw()
{

}


