//-----------------------------------------------------------------------------------------------------
// #name bg.cpp
// #description �w�i����
// #make 2025/02/03�@�@�i��`��
// #update 2025/02/03
// #comment  
//			�@�w�i�̑傫��1280*720���Ƒ������Ԃ����Ȃ��i�����j
//          
//----------------------------------------------------------------------------------------------------

#include "sprite.h"
#include "texture.h"
#include "renderer.h"
#include "Bg.h"
#include "player_position.h"


static ID3D11ShaderResourceView* g_Bg_Texture[5] = { NULL };  // �w�i�e�N�X�`���i�z�񉻁j
static ID3D11ShaderResourceView* g_Bg_Texture_light = NULL;  // ���C�g�p�e�N�X�`��

// �v���C���[�̉ߋ��̍��W
b2Vec2 g_old_player_position;

// �w�i�C���X�^���X
Bg bg;

#define BACK_GROUND_HEIGHT (-200)
#define BACK_GROUND_SIZE_X (1280*1.2)
#define BACK_GROUND_SIZE_Y (720*1.2)

void Bg::Initialize()
{
    // �e�w�i�e�N�X�`���̓ǂݍ��݁i�z��ɓ���j
    g_Bg_Texture[0] = InitTexture(L"asset\\texture\\stage1_1\\background5.png");
    g_Bg_Texture[1] = InitTexture(L"asset\\texture\\stage1_1\\background4.png");
    g_Bg_Texture[2] = InitTexture(L"asset\\texture\\stage1_1\\background3.png");
    g_Bg_Texture[3] = InitTexture(L"asset\\texture\\stage1_1\\background2.png");
    g_Bg_Texture[4] = InitTexture(L"asset\\texture\\stage1_1\\background1.png");

    g_Bg_Texture_light = InitTexture(L"asset\\texture\\stage1_1\\background_light.png");

    // �w�i�摜�̏����z�u�i�z�񉻁j
    for (int layer = 0; layer < 5; layer++)
    {
        for (int i = 0; i < 4; i++)
        {
            bg.texture_pos[layer][i] = XMFLOAT2(BACK_GROUND_SIZE_X * i, BACK_GROUND_HEIGHT);
        }
    }
}

void Bg::Update()
{
    // �v���C���[�̌��݈ʒu���擾
    b2Vec2 player_position = PlayerPosition::GetPlayerPosition();

    // �e�N�X�`�����Ƃ̃X�N���[�����x
    float textureScrollSpeeds[] = { 0.1f, 0.3f, 0.7f, 1.0f, 3.0f };

    // �w�i�̃X�N���[������
    for (int layer = 0; layer < 5; layer++)
    {
        for (int i = 0; i < 4; i++)
        {
            bg.texture_pos[layer][i].x -= (player_position.x - g_old_player_position.x) * textureScrollSpeeds[layer] * 100.0f;

            // ���Ƀ��[�v����
            if (bg.texture_pos[layer][i].x < -BACK_GROUND_SIZE_X)
                bg.texture_pos[layer][i].x += BACK_GROUND_SIZE_X * 4.0f;

            // �E�Ƀ��[�v����
            if (bg.texture_pos[layer][i].x > BACK_GROUND_SIZE_X * 3.0f)
                bg.texture_pos[layer][i].x -= BACK_GROUND_SIZE_X * 4.0f;
        }
    }

    // �v���C���[���W�̍X�V
    g_old_player_position = player_position;
}

void Bg::Draw()
{
    // �w�i�e�N�X�`���̕`��
    for (int layer = 0; layer < 4; layer++)  // 4�w����`��
    {
        for (int i = 0; i < 4; i++)
        {
            GetDeviceContext()->PSSetShaderResources(0, 1, &g_Bg_Texture[layer]);
            DrawSprite(bg.texture_pos[layer][i], 0.0f, XMFLOAT2(BACK_GROUND_SIZE_X+10, BACK_GROUND_SIZE_Y));
        }
    }

    // ���C�g�e�N�X�`���̕`��
    GetDeviceContext()->PSSetShaderResources(0, 1, &g_Bg_Texture_light);
    DrawSpriteOld({ BACK_GROUND_SIZE_X / 2, BACK_GROUND_SIZE_Y / 2 }, 0.0f, { BACK_GROUND_SIZE_X, BACK_GROUND_SIZE_Y }, 1.0f);
}

void Bg::FrontDraw()
{
    //// �w�i5�i�O�i�j�̕`��
    //for (int i = 0; i < 4; i++)
    //{
    //    GetDeviceContext()->PSSetShaderResources(0, 1, &g_Bg_Texture[4]);
    //    DrawSprite(bg.texture_pos[4][i], 0.0f, XMFLOAT2(BACK_GROUND_SIZE_X+10, BACK_GROUND_SIZE_Y),0.5);
    //}
}

void Bg::Finalize()
{
    // �e�w�i�e�N�X�`���̉��
    for (int i = 0; i < 5; i++)
    {
        UnInitTexture(g_Bg_Texture[i]);
    }
}
