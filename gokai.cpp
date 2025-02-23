// ---------------------------------------------------------------------------------------------------- -
// #name gokai.cpp
// #description     gokai��UI�\���p
// #make 2025/02/02�@�i��`��
// #update 2024/02/02
// #comment �ǉ��E�C���\��
//
//----------------------------------------------------------------------------------------------------
#include"gokai.h"
#include"texture.h"
#include"sprite.h"


static ID3D11ShaderResourceView * g_number_Texture = NULL;//�����̃e�N�X�`��

// �ÓI�����o�ϐ��̒�`�i�������j


int Gokai_UI::now_get_gokai_count = 0;  // ���݂̍����l	
int Gokai_UI::gokai_when_respawn = 0;  // ���X�|�����̍����l	

void Gokai_UI::Initialize()
{
	//�����o�[�ϐ��̏�����
	now_get_gokai_count = 0;  // ���݂̍����l	
	gokai_when_respawn = 0;  // ���X�|�����̍����l	

	if (g_number_Texture == NULL)
	{
		g_number_Texture = InitTexture(L"asset\\texture\\sample_texture\\sample_number.png");
	}
}

void Gokai_UI::Draw()
{

		// �V�F�[�_���\�[�X��ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_number_Texture);


		int cnt = now_get_gokai_count;
		// �����̕\�� ���݂̗�
		for (int i = 0; i < 5; i++)
		{
			DrawDividedSprite(XMFLOAT2(1250 - (i * 40), 40), 0.0f, XMFLOAT2(40, 40), 10, 1, cnt, 1.0);
			cnt /= 10;
		}

	




}

void Gokai_UI::Finalize()
{
	if (g_number_Texture != NULL)
	{
		UnInitTexture(g_number_Texture);
		g_number_Texture = NULL;
	}
}