//-----------------------------------------------------------------------------------------------------
// #name bg.cpp
// #description �w�i����
// #make 2024/12/12�@�@�����F��
// #update 2024/12/12
// #comment  
//			�@�w�i�̑傫��1280*720���Ƒ������Ԃ����Ȃ��i�����j
//          
//----------------------------------------------------------------------------------------------------

#include "sprite.h"
#include "texture.h"
#include "renderer.h"
#include "Bg.h"
#include "player_position.h"


static ID3D11ShaderResourceView* g_Bg_Texture1 = NULL;//�w�i�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Bg_Texture2 = NULL;//�w�i�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Bg_Texture3 = NULL;//�w�i�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Bg_Texture4 = NULL;//�w�i�̃e�N�X�`��
static ID3D11ShaderResourceView* g_Bg_Texture5 = NULL;//�w�i�̃e�N�X�`��



// �v���C���[�̉ߋ��̍��W
b2Vec2 g_old_player_position;

// �e�w�i�̃X�N���[�����x
static float g_ScrollSpeed[3] = { 0.5f, 0.8f, 1.0f };

// UV�X�N���[���pTu���W
static float g_UvScrollTu = 0.0f;

// �w�i�C���X�^���X
Bg bg;

#define BACK_GROUND_HEIGHT (-350)

void Bg::Initialize()
{
    // �e�w�i�e�N�X�`���̓ǂݍ���
    g_Bg_Texture1 = InitTexture(L"asset\\texture\\stage1_1\\background1.png");
    g_Bg_Texture2 = InitTexture(L"asset\\texture\\stage1_1\\background5.png");
    g_Bg_Texture3 = InitTexture(L"asset\\texture\\stage1_1\\background4.png");
    g_Bg_Texture4 = InitTexture(L"asset\\texture\\stage1_1\\background3.png");
    g_Bg_Texture5 = InitTexture(L"asset\\texture\\stage1_1\\background2.png");

    
   
    
   // �w�i�摜�̏����z�u�i4���ɑ��₷�j
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
    // �v���C���[�̌��݈ʒu���擾
    b2Vec2 player_position = PlayerPosition::GetPlayerPosition();

    // �e�N�X�`�����Ƃ̃X�N���[�����x
    float textureScrollSpeeds[] = { 0.1f, 0.3f, 0.7f,1.0 ,1.5};

    // �w�i1�̃X�N���[������
    for (int i = 0; i < 4; i++)
    {
        bg.texture_1_pos[i].x -= (player_position.x - g_old_player_position.x) * textureScrollSpeeds[0] * 100.0f;

        // ���Ƀ��[�v����
        if (bg.texture_1_pos[i].x < -SCREEN_WIDTH*2)
            bg.texture_1_pos[i].x += SCREEN_WIDTH*2 * 4.0f;

        // �E�Ƀ��[�v����
        if (bg.texture_1_pos[i].x > SCREEN_WIDTH*2 * 3.0f)
            bg.texture_1_pos[i].x -= SCREEN_WIDTH *2* 4.0f;
    }

    // �w�i2�̃X�N���[������
    for (int i = 0; i < 4; i++)
    {
        bg.texture_2_pos[i].x -= (player_position.x - g_old_player_position.x) * textureScrollSpeeds[1] * 100.0f;

        // ���Ƀ��[�v����
        if (bg.texture_2_pos[i].x < -SCREEN_WIDTH*2)
            bg.texture_2_pos[i].x += SCREEN_WIDTH*2 * 4.0f;

        // �E�Ƀ��[�v����
        if (bg.texture_2_pos[i].x > SCREEN_WIDTH*2 * 3.0f)
            bg.texture_2_pos[i].x -= SCREEN_WIDTH*2 * 4.0f;
    }

    // �w�i2�̃X�N���[������
    for (int i = 0; i < 4; i++)
    {
        bg.texture_3_pos[i].x -= (player_position.x - g_old_player_position.x) * textureScrollSpeeds[2] * 100.0f;

        // ���Ƀ��[�v����
        if (bg.texture_3_pos[i].x < -SCREEN_WIDTH * 2)
            bg.texture_3_pos[i].x += SCREEN_WIDTH * 2 * 4.0f;

        // �E�Ƀ��[�v����
        if (bg.texture_3_pos[i].x > SCREEN_WIDTH * 2 * 3.0f)
            bg.texture_3_pos[i].x -= SCREEN_WIDTH * 2 * 4.0f;
    }

    // �w�i2�̃X�N���[������
    for (int i = 0; i < 4; i++)
    {
        bg.texture_4_pos[i].x -= (player_position.x - g_old_player_position.x) * textureScrollSpeeds[3] * 100.0f;

        // ���Ƀ��[�v����
        if (bg.texture_4_pos[i].x < -SCREEN_WIDTH * 2)
            bg.texture_4_pos[i].x += SCREEN_WIDTH * 2 * 4.0f;

        // �E�Ƀ��[�v����
        if (bg.texture_4_pos[i].x > SCREEN_WIDTH * 2 * 3.0f)
            bg.texture_4_pos[i].x -= SCREEN_WIDTH * 2 * 4.0f;
    }


    // �w�i2�̃X�N���[������
    for (int i = 0; i < 4; i++)
    {
        bg.texture_5_pos[i].x -= (player_position.x - g_old_player_position.x) * textureScrollSpeeds[4] * 100.0f;

        // ���Ƀ��[�v����
        if (bg.texture_5_pos[i].x < -SCREEN_WIDTH * 2)
            bg.texture_5_pos[i].x += SCREEN_WIDTH * 2 * 4.0f;

        // �E�Ƀ��[�v����
        if (bg.texture_5_pos[i].x > SCREEN_WIDTH * 2 * 3.0f)
            bg.texture_5_pos[i].x -= SCREEN_WIDTH * 2 * 4.0f;
    }

    // �v���C���[���W�̍X�V
    g_old_player_position = player_position;
}

void Bg::Draw()
{


    // �w�i�e�N�X�`���̕`��
    ID3D11ShaderResourceView* textures[] = { g_Bg_Texture1, g_Bg_Texture2, g_Bg_Texture3 ,g_Bg_Texture4,g_Bg_Texture5};

    // �w�i1�̕`��
    for (int i = 0; i < 4; i++)
    {
        GetDeviceContext()->PSSetShaderResources(0, 1, &textures[0]);
        DrawSprite(bg.texture_1_pos[i], 0.0f, XMFLOAT2(SCREEN_WIDTH*2, SCREEN_HEIGHT*2));
    }

    // �w�i2�̕`��
    for (int i = 0; i < 4; i++)
    {
        GetDeviceContext()->PSSetShaderResources(0, 1, &textures[1]);
        DrawSprite(bg.texture_2_pos[i], 0.0f, XMFLOAT2(SCREEN_WIDTH*2, SCREEN_HEIGHT*2));
    }
    // �w�i2�̕`��
    for (int i = 0; i < 4; i++)
    {
        GetDeviceContext()->PSSetShaderResources(0, 1, &textures[2]);
        DrawSprite(bg.texture_3_pos[i], 0.0f, XMFLOAT2(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2));
    }

    // �w�i2�̕`��
    for (int i = 0; i < 4; i++)
    {
        GetDeviceContext()->PSSetShaderResources(0, 1, &textures[3]);
        DrawSprite(bg.texture_3_pos[i], 0.0f, XMFLOAT2(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2));
    }

    // �w�i2�̕`��
    for (int i = 0; i < 4; i++)
    {
        GetDeviceContext()->PSSetShaderResources(0, 1, &textures[4]);
        DrawSprite(bg.texture_3_pos[i], 0.0f, XMFLOAT2(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2));
    }
}

void Bg::Finalize()
{
    // �e�w�i�e�N�X�`���̉��
    UnInitTexture(g_Bg_Texture1);
    UnInitTexture(g_Bg_Texture2);
    UnInitTexture(g_Bg_Texture3);
    UnInitTexture(g_Bg_Texture4);
    UnInitTexture(g_Bg_Texture5);
}
