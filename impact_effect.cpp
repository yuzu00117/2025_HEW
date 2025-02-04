//-----------------------------------------------------------------------------------------------------
// #name impact_effect.cpp
// #description 衝突の時の衝撃を取得してエフェクトを表示するCPP
// #make 2025/02/01　永野義也
// #update 2024/02/01
// #comment 追加・修正予定
//          ・衝突時のエフェクトに変更があれば変更
//----------------------------------------------------------------------------------------------------
#include"impact_effect.h"
#include"texture.h"
#include"sprite.h"
#include"player_position.h"
#include"world_box2d.h"


static ID3D11ShaderResourceView* g_Imapct_effect_texture1 = NULL;//衝突時のエフェクト１
static ID3D11ShaderResourceView* g_Imapct_effect_texture2 = NULL;//衝突時のエフェクト2


// **グローバル変数 `impactEffects` の定義（ここでのみ実体を持つ）**
std::vector<ImpactEffect> impactEffects;

void InitImpactEffect()
{
    g_Imapct_effect_texture1 = InitTexture(L"asset\\texture\\sample_texture\\stage_smoke1.png");
    g_Imapct_effect_texture2 = InitTexture(L"asset\\texture\\sample_texture\\stage_smoke2.png");
}


void UpdateImpactEffects() {
    for (auto it = impactEffects.begin(); it != impactEffects.end();) {
        it->lifeTime--;
        if (it->lifeTime <= 0) {
            it = impactEffects.erase(it); // **寿命が尽きたら削除**
        }
        else {
            ++it;
        }
    }
}

void DrawImpactEffects(float Draw_size) {

    float scale = SCREEN_SCALE;
    b2Vec2 screen_center;
    screen_center.x = SCREEN_CENTER_X;
    screen_center.y = SCREEN_CENTER_Y;

    for (const auto& effect : impactEffects) {
        float effect_draw_x = ((effect.pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
        float effect_draw_y = ((effect.pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;



        if (g_Imapct_effect_texture1 == nullptr|| g_Imapct_effect_texture2==nullptr) {
            return;
        }


        if (effect.type == 1)
        {
            // **正しいポインタで設定**
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Imapct_effect_texture1);

            DrawSplittingSprite(
                { effect_draw_x, effect_draw_y },
                effect.angle, // **エフェクトの回転角度**
                { 200 * effect.scale, 200 * effect.scale }, // **スケール適用**
                5, 4, effect.lifeTime / 4, 1.f
            );
        }
        else
        {
            // **正しいポインタで設定**
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Imapct_effect_texture2);

            DrawSplittingSprite(
                { effect_draw_x, effect_draw_y },
                effect.angle, // **エフェクトの回転角度**
                { 400 * effect.scale, 400 * effect.scale }, // **スケール適用**
                5, 4, effect.lifeTime / 4, 1.f
            );
        }
      
    }
}
void FinalizeImpactEffects()
{
    if (g_Imapct_effect_texture1 != NULL)
    {
        UnInitTexture(g_Imapct_effect_texture1);
        UnInitTexture(g_Imapct_effect_texture2);

        g_Imapct_effect_texture1 = NULL;
        g_Imapct_effect_texture2 = NULL;

    }
}