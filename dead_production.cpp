//-----------------------------------------------------------------------------------------------------
// #name dead_production.cpp
// #description    ���S���o
// #make 2025/02/08�@�i��`��
// #update 2025/02/08
// #comment �ǉ��E�C���\��
//
//-------------------------------------------------------------------------------------------------------
#include"dead_production.h"
#include"texture.h"
#include"sprite.h"
#include"easing.h"

static ID3D11ShaderResourceView* g_Black_texture = NULL;//�����̃e�N�X�`��
static ID3D11ShaderResourceView* g_Black_hole_texture = NULL;//�����̃e�N�X�`��

float dead_production::Dead_Cnt = 0;
float dead_production::Black_fade_rate = 0;
bool dead_production::Dead_Flag = false;

void dead_production::Initialize()
{
	if (g_Black_texture == NULL)
	{
		g_Black_texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_block.png");
		g_Black_hole_texture = InitTexture(L"asset\\texture\\sample_texture\\sample_fade_black.png");
	}
}


void dead_production::Update()
{
	Dead_Cnt++;
}

void dead_production::Draw()
{
	if (Dead_Cnt != 0)
	{
		
			Black_fade_rate += 0.01;
			float fadeeasingRate = Ease::InCubic(Black_fade_rate);
			if (0.5 < fadeeasingRate)
			{
				fadeeasingRate = 0.5;
			}

			// �V�F�[�_���\�[�X��ݒ�
			GetDeviceContext()->PSSetShaderResources(0, 1, &g_Black_texture);
			DrawSpriteOld(XMFLOAT2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), 0.0, XMFLOAT2(SCREEN_WIDTH, SCREEN_HEIGHT), fadeeasingRate);



			if (180<Dead_Cnt)
			{
				Dead_Flag = true;
				Dead_Cnt = 0;
				Black_fade_rate = 0;
			}
	}

}

void dead_production::Finalize()
{
	if (g_Black_texture != NULL)
	{
		UnInitTexture(g_Black_texture);
		g_Black_texture = NULL;
	}
}