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
#include"anchor_spirit.h"
#include"player_stamina.h"


#define	MAX_WORD	(100)
#define NUMBER_PARAMETER	(6)

enum PARAMETER
{
	PARAMETER_PLAYER_POSITION = 0,
	PARAMETER_CAN_JUMP,
	PARAMETER_BEGINCONTACT,
	PARAMETER_ENDCONTACT,
	PARAMETER_STAMINA,
	PARAMETER_SPIRIT,

};



//��������摜�ɕϊ�����ۂ̏��
WORD_INFORM g_word[NUMBER_PARAMETER][MAX_WORD];


//�f�o�b�N�p�p�����[�^�ꗗ���J������		
bool	g_pressed ;
//�f�o�b�O�p�p�����[�^�ꗗ
bool	g_open;

//�v���C���[�̈ʒu���
b2Vec2	g_player_position = { 0.0f,0.0f };

//�v���C���[�W�����v�\���ǂ���
bool	g_can_jump;

//�̗͒l
float	g_stamina;
//�\�E���l
float	g_spirit;


//�R���^�N�g���X�g�̊֐��Ă΂�Ă��邩�ǂ���
bool	g_BeginContact;
//�R���^�N�g���X�g�̊֐��Ă΂�Ă��邩�ǂ���
bool	g_EndContact;

void SetBeginContact(bool a)
{
	g_BeginContact = a;
}

void SetEndContact(bool a)
{
	g_EndContact = a;
}

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
				SetWord(words, g_word[PARAMETER_PLAYER_POSITION], MAX_WORD);
				break;
				//�v���C���[���W�����v�\���ǂ���
			case PARAMETER_CAN_JUMP:
				g_can_jump = player_collider.GetJumpFlag();
				words = ToString(L"Can �W �� �� �v : ", g_can_jump);
				SetWord(words, g_word[PARAMETER_CAN_JUMP], MAX_WORD);
				break;
			case PARAMETER_BEGINCONTACT:
				words = ToString(L"BeginContact : ", g_BeginContact);
				SetWord(words, g_word[PARAMETER_BEGINCONTACT], MAX_WORD);
				break;
			case PARAMETER_ENDCONTACT:
				words = ToString(L"EndContact : ", g_EndContact);
				SetWord(words, g_word[PARAMETER_ENDCONTACT], MAX_WORD);
				break;
			case PARAMETER_STAMINA:
				g_stamina = PlayerStamina::GetPlayerStaminaValue();
				words = ToString(L"�X �^ �~ �i �Q �[ �W : ", g_stamina);
				SetWord(words, g_word[PARAMETER_STAMINA], MAX_WORD);
				break;
			case PARAMETER_SPIRIT:
				g_spirit = AnchorSpirit::GetAnchorSpiritValue();
				words = ToString(L"�\ �E �� �Q �[ �W : ", g_spirit);
				SetWord(words, g_word[PARAMETER_SPIRIT], MAX_WORD);
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


		for (int i = 0; i < NUMBER_PARAMETER; i++)
		{
			switch (i)
			{
			case PARAMETER_PLAYER_POSITION:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[PARAMETER_PLAYER_POSITION]);
				break;
			case PARAMETER_CAN_JUMP:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + 0.3f) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[PARAMETER_CAN_JUMP]);
				break;
			case PARAMETER_BEGINCONTACT:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + 0.6f) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[PARAMETER_BEGINCONTACT]);
				break;
			case PARAMETER_ENDCONTACT:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + 0.9f) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[PARAMETER_ENDCONTACT]);
				break;
			case PARAMETER_STAMINA:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + 1.2f) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[PARAMETER_STAMINA]);
				break;
			case PARAMETER_SPIRIT:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + 1.5f) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_word[PARAMETER_SPIRIT]);
				break;

			}
		}




	}
}
