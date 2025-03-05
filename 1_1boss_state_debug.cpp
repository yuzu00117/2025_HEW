//-----------------------------------------------------------------------------------------------------
// #name 1_1_boss_state_debug.cpp
// #description �f�o�b�N�p�̃p�����[�^�ꗗ
// #make 2025/01/24		�i��`��
// #update 2025/01/24
// #comment �ǉ��E�C���\��
//----------------------------------------------------------------------------------------------------

#include"1_1boss_state_debug.h"
#include"debug.h"
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
#include"1-1_boss.h"




enum PARAMETER_BOSS
{
	BOSS_POSITION = 0,
	BOSS_HP,
	BOSS_STATE,
	BOSS_PLAYER_NEAR,
	BOSS_ELAPSED_TIME,
};



//��������摜�ɕϊ�����ۂ̏��
WORD_INFORM g_boss_word[NUMBER_PARAMETER][MAX_WORD];


//�f�o�b�N�p�p�����[�^�ꗗ���J������		
bool	g_boss_pressed;
//�f�o�b�O�p�p�����[�^�ꗗ
bool	g_boss_open = true;

//�v���C���[�̈ʒu���
b2Vec2	g_boss_position = { 0.0f,0.0f };


//�̗͒l
float	g_boss_stamina;
//�\�E���l
float	g_boss_spirit;




void	InitializeBossDebug()
{
	g_boss_open = 1;
}


void	FinalizeBossDebug()
{

}


void	UpdateBossDebug()
{
	
	Boss_1_1& boss = Boss_1_1::GetInstance();


	//�����f�o�b�O�p�p�����[�^�ꗗ���J���Ă����ԂȂ�
	if (g_boss_open)
	{
		PlayerPosition	player;
		Player &player_collider = Player::GetInstance();


		std::wstring words;

		for (int i = 0; i < NUMBER_PARAMETER; i++)
		{
			switch (i)
			{
				//�v���C���[�ʒu���
			case BOSS_POSITION:
				//�v���C���[�̈ʒu���
				g_boss_position = boss.GetBossBody()->GetPosition();
				//�ʒu���𕶎���ɕϊ�
				words = ToString(L"Boss Position: ", g_boss_position.x, L" , ", g_boss_position.y);
				//��������摜�ɕϊ�
				SetWord(words, g_boss_word[i], MAX_WORD);
				break;
				//�v���C���[�̗̑�
			case BOSS_HP:
				//�v���C���[�̈ʒu���
				int boss_hp;
				boss_hp = boss.GetBossHP();
				//�ʒu���𕶎���ɕϊ�
				words = ToString(L"Boss HP : ", boss_hp);
				//��������摜�ɕϊ�
				SetWord(words, g_boss_word[i], MAX_WORD);
				break;
				//�\�E���Q�[�W
			case BOSS_STATE:
				//�v���C���[�̈ʒu���

				//�ʒu���𕶎���ɕϊ�


				switch (boss.GetNowBossState())
				{
				case wait_state:
					words = ToString(L"Boss State:wait_state              ");
					break;
				case panic_state:
					words = ToString(L"Boss State:panic_state             ");

					break;
				case walk_state:
					words = ToString(L"Boss State:walk_state              ");

					break;
				case jump_state:
					words = ToString(L"Boss State:jump_state              ");

					break;
				case charge_attack_state:
					words = ToString(L"Boss State:charge_attack_state     ");

					break;
				case shock_wave_state:
					words = ToString(L"Boss State:shock_wave_state        ");

					break;
				case create_mini_golem_state:
					words = ToString(L"Boss State:create_mini_golem_state ");
					break;

				default:
					words = ToString(L"Boss State:NULL");
					break;
				}

				//��������摜�ɕϊ�
				SetWord(words, g_boss_word[i], MAX_WORD);
				break;

			case BOSS_PLAYER_NEAR://�v���C���[���߂��ɂ��邩�ǂ���
			{
				bool near_flag = boss.GetPlayerisNearbyFlag();
				words = ToString(L"Boss_near : ", near_flag);
				//��������摜�ɕϊ�
				SetWord(words, g_boss_word[i], MAX_WORD);
			}
			break;

			case BOSS_ELAPSED_TIME:
				// �{�X�̌o�ߎ��Ԃ��擾
				float boss_elapsed_time = boss.GetBossElapsedTime();
				// �o�ߎ��Ԃ𕶎���ɕϊ�
				words = ToString(L"Boss Elapsed Time: ", boss_elapsed_time);
				// ��������摜�ɕϊ�
				SetWord(words, g_boss_word[i], MAX_WORD);
				break;
			}
		}



	}
}


void	DrawBossDebug()
{
	if (g_boss_open)
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
			case BOSS_POSITION:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_boss_word[i]);
				break;
			case BOSS_HP:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_boss_word[i]);
				break;
			case BOSS_STATE:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_boss_word[i]);
				break;
			case BOSS_PLAYER_NEAR:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_boss_word[i]);
				break;
			case BOSS_ELAPSED_TIME:
				first_word_position.x = (screen_rightup.x - screen_rightup.x / 4) * scale;
				first_word_position.y = (screen_rightup.y + distance_y * i) * scale;
				DrawWord(first_word_position, word_scales, word_rotate, g_boss_word[i]);
				break;
			}
		}




	}
}
