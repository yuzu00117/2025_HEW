//-----------------------------------------------------------------------------------------------------
// #name debug.cpp
// #description �f�o�b�N�p�̃p�����[�^�ꗗ
// #make 2024/11/9		���j�S
// #update 2024/11/20
// #comment �ǉ��E�C���\��
//          �E�A���J�[�̃|�W�V����
//          �E�A���J�[�̌o���l
// 			�E �A���J�[�̃��x��
//          �E �A���J�[�̏��
//----------------------------------------------------------------------------------------------------

#include "debug.h"
#include"player_position.h"
#include"keyboard.h"
#include"sprite.h"
#include"renderer.h"
#include"word.h"
#include"main.h"
#include<iostream>
#include"player.h"
#include"player_stamina.h"
#include"anchor_spirit.h"


#define	MAX_WORD	(100)
#define NUMBER_PARAMETER	(3)

enum PARAMETER
{
	PARAMETER_PLAYER_POSITION = 0,
	PARAMETER_PLAYER_STAMINA,
	PARAMETER_SPIRIT_GAUGE,
};



//��������摜�ɕϊ�����ۂ̏��
WORD_INFORM g_word[NUMBER_PARAMETER][MAX_WORD];


//�f�o�b�N�p�p�����[�^�ꗗ���J������		
bool	g_pressed ;
//�f�o�b�O�p�p�����[�^�ꗗ
bool	g_open = true;

//�v���C���[�̈ʒu���
b2Vec2	g_player_position = { 0.0f,0.0f };


//�̗͒l
float	g_stamina;
//�\�E���l
float	g_spirit;




void	InitializeDebug()
{
	
}


void	FinalizeDebug()
{
	
}


void	UpdateDebug()
{
	//�{�^����
	if (Keyboard_IsKeyDown(KK_X) && !g_pressed)
	{
		g_open = !g_open;	//�J�������邩�X�C�b�`

	}
	g_pressed = Keyboard_IsKeyDown(KK_X);	//�g���K�[

	//�����f�o�b�O�p�p�����[�^�ꗗ���J���Ă����ԂȂ�
	if (g_open)
	{
		PlayerPosition	player;
		Player player_collider = Player::GetInstance();


		std::wstring words;
		
		for (int i = 0; i < NUMBER_PARAMETER; i++)
		{
			switch (i)
			{
				//�v���C���[�ʒu���
			case PARAMETER_PLAYER_POSITION:
				//�v���C���[�̈ʒu���
				g_player_position = player.GetPlayerPosition();
				//�ʒu���𕶎���ɕϊ�
				words = ToString(L"�v �� �C �� �[ Position : ", g_player_position.x, L" , ", g_player_position.y);
				//��������摜�ɕϊ�
				SetWord(words, g_word[i], MAX_WORD);
				break;				
				//�v���C���[�̗̑�
			case PARAMETER_PLAYER_STAMINA:
				//�v���C���[�̈ʒu���
				float stamina;
				stamina = PlayerStamina::GetPlayerStaminaValue();
				//�ʒu���𕶎���ɕϊ�
				words = ToString(L"�v �� �C �� �[ Stamina : ", stamina);
				//��������摜�ɕϊ�
				SetWord(words, g_word[i], MAX_WORD);
				break;		
				//�\�E���Q�[�W
			case PARAMETER_SPIRIT_GAUGE:
				//�v���C���[�̈ʒu���
				float spirit = AnchorSpirit::GetAnchorSpiritValue();
				//�ʒu���𕶎���ɕϊ�
				words = ToString(L"�\ �[ �� �Q �[ �W : ", spirit);
				//��������摜�ɕϊ�
				SetWord(words, g_word[i], MAX_WORD);
				break;
			}
		}
		


	}
}


void	DrawDebug()
{
	if (g_open)
	{
		//��ʉE��̈ʒu
		XMFLOAT2 screen_rightup = { 16.0f, 0.4f };
		// �R���C�_�[�ƈʒu���̕␳�����邽��
		float scale = 80.0f;

		//�\������ꏊ��ݒ�
		XMFLOAT2 first_word_position;
		//�\������T�C�Y��ݒ�
		XMFLOAT2 word_scales = { 18.0f,33.0f };
		//�\�������]�p�x��ݒ�
		float word_rotate = 0.0f;

		float distance_y = 0.3f;

		for (int i = 0; i < NUMBER_PARAMETER; i++)
		{
			switch (i)
			{
			case PARAMETER_PLAYER_POSITION:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[i]);
				break;			
			case PARAMETER_PLAYER_STAMINA:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[i]);
				break;			
			case PARAMETER_SPIRIT_GAUGE:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[i]);
				break;
			}
		}




	}
}
