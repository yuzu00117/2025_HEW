//-----------------------------------------------------------------------------------------------------
// #name blown_away__effect.cpp
// #description �G���v���C���[�̃Z���T�[����ł��牉�o�𔭐�������
// #make 2025/02/03�@�i��`��
// #update 2024/02/03
// #comment �ǉ��E�C���\��
//          
//----------------------------------------------------------------------------------------------------
#include"blown_away_effect.h"
#include"texture.h"
#include"sprite.h"
#include"player_position.h"
#include"world_box2d.h"


static ID3D11ShaderResourceView* g_blown_away_effect_texture1 = NULL;//������΂��̃G�t�F�N�g
static ID3D11ShaderResourceView* g_blown_away_effect_texture2 = NULL;//������΂��̃G�t�F�N�g
static ID3D11ShaderResourceView* g_blown_away_effect_texture3 = NULL;//������΂��̃G�t�F�N�g


// **�O���[�o���ϐ� `impactEffects` �̒�`�i�����ł̂ݎ��̂����j**
std::vector<Blown_Away_Effect> blown_away_Effects;

void InitBlownAwayEffect()
{
    g_blown_away_effect_texture1 = InitTexture(L"asset\\texture\\enemy_texture\\Burst_Effect_1.png");
    g_blown_away_effect_texture2 = InitTexture(L"asset\\texture\\enemy_texture\\Burst_Effect_2.png");
    g_blown_away_effect_texture3 = InitTexture(L"asset\\texture\\enemy_texture\\Burst_Effect_3.png");
}


void UpdateBlownAwayEffects() {
    for (auto it = blown_away_Effects.begin(); it != blown_away_Effects.end();) {
        it->lifeTime--;
        if (it->lifeTime <= 0) {
            it = blown_away_Effects.erase(it); // **�������s������폜**
        }
        else {
            ++it;
        }
    }
}

void DrawBlownAwayEffects(float Draw_size) {

    float scale = SCREEN_SCALE;
    b2Vec2 screen_center;
    screen_center.x = SCREEN_CENTER_X;
    screen_center.y = SCREEN_CENTER_Y;

    for (const auto& effect : blown_away_Effects) {
        float effect_draw_x = ((effect.pos.x - PlayerPosition::GetPlayerPosition().x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
        float effect_draw_y = ((effect.pos.y - PlayerPosition::GetPlayerPosition().y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;



        if (g_blown_away_effect_texture1 == nullptr || g_blown_away_effect_texture2 == nullptr) {
            return;
        }

        switch (effect.type)
        {
        case 1:
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_blown_away_effect_texture1);
            break;
        case 2:
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_blown_away_effect_texture2);
            break;
        case 3:
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_blown_away_effect_texture3);
            break;
        default:
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_blown_away_effect_texture3);
            break;
        }

        DrawSplittingSprite(
            { effect_draw_x, effect_draw_y },
            effect.angle, // **�G�t�F�N�g�̉�]�p�x**
            { 400 * effect.scale, 400 * effect.scale }, // **�X�P�[���K�p**
            5, 4, effect.lifeTime / 4, 1.f
        );

    }
}
void FinalizeBlownAwayEffects()
{
    if (g_blown_away_effect_texture1) UnInitTexture(g_blown_away_effect_texture1);
    if (g_blown_away_effect_texture2) UnInitTexture(g_blown_away_effect_texture2);
    if (g_blown_away_effect_texture3) UnInitTexture(g_blown_away_effect_texture3);
}