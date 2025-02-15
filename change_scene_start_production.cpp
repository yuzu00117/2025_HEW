//-----------------------------------------------------------------------------------------------------
// #name change_scene_start_production.cpp
// #description    �V�[���ڍs�̍ۂ̏I��鑤�̉��o
// #make 2025/02/15�@�i��`��
// #update 2025/02/15
// #comment �ǉ��E�C���\��
//
//-------------------------------------------------------------------------------------------------------
#include"change_scene_start_production.h"
#include"texture.h"
#include"sprite.h"
#include"easing.h"
#include"player_life.h"
#include"player_stamina.h"
#include"hit_stop.h"
#include"player_position.h"
#include"world_box2d.h"
#include"player.h"

static ID3D11ShaderResourceView* g_Black_texture = NULL;//�����̃e�N�X�`��
static ID3D11ShaderResourceView* g_Black_hole_texture = NULL;//�����̃e�N�X�`��

float change_scene_start_production::Change_Scene_Cnt = 0;
float change_scene_start_production::Black_fade_rate = 0;
float change_scene_start_production::Black_hole_rate = 0;
bool change_scene_start_production::Change_Flag = false;

void change_scene_start_production::Initialize()
{
	if (g_Black_texture == NULL)
	{
		g_Black_texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_block.png");
		g_Black_hole_texture = InitTexture(L"asset\\texture\\sample_texture\\sample_fade_black.png");
	}
}


void change_scene_start_production::Reset()
{
	Change_Scene_Cnt = 1;
	Black_fade_rate = 0;
	Black_hole_rate = 0;
	Change_Flag = false;
}

void change_scene_start_production::Update()
{
	if (Change_Scene_Cnt != 0)
	{
		Change_Scene_Cnt++;

		// Black_fade_rate��0�`1�͈̔͂Ɏ��߂�
		if (Black_fade_rate < 1.0f)
		{
			Black_fade_rate += 0.01f;
		}
	}

	// Change_Scene_Cnt�����𒴂����烊�Z�b�g
	if (Change_Scene_Cnt > 150)
	{
		Change_Scene_Cnt = 0;
		Black_fade_rate = 0.0f;
	}
}

void change_scene_start_production::Draw()
{
	if (Change_Scene_Cnt != 0)
	{

		
		float fadeeasingRate = Ease::InCubic(Black_fade_rate);
		
		

		//�������̍����d�˂�
		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Black_texture);
		DrawSpriteOld(XMFLOAT2(SCREEN_WIDTH / 2 ,SCREEN_HEIGHT / 2 ), 0.0, XMFLOAT2(SCREEN_WIDTH * 2, SCREEN_HEIGHT * 4), 1.0-fadeeasingRate);

	}


}

void change_scene_start_production::Finalize()
{
	if (g_Black_texture != NULL)
	{
		UnInitTexture(g_Black_texture);
		g_Black_texture = NULL;
	}
}