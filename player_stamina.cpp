//-----------------------------------------------------------------------------------------------------
// #name player_stamina.cpp
// #description �v���C���[�̗̑͒l
// #make 2024/11/20			���j�S
// #update 2024/11/20
// #comment �ǉ��E�C���\��
//           �E�c�@�Ɖ�ʑJ�ڒǉ����ꂽ��A�̗�0�ɂȂ������̏�����ǉ�
//           �E2025/01/21 yuzu00117 �̗͂�0�ɂȂ����Ƃ��Ƀ��U���g��ʂɑJ�ڂ��鏈����ǉ�
// 
//----------------------------------------------------------------------------------------------------


#include	"player_stamina.h"
#include	"anchor_spirit.h"
#include	"scene.h"
#include	"sound.h"
#include	"Item_Manager.h"
#include	"player.h"


// �ÓI�����o�ϐ��̒�`�i1�񂾂��s���j
float	PlayerStamina::m_stamina = 300.0f;
bool PlayerStamina::m_isdead = false; // �v���C���[�̎��S�t���O


void PlayerStamina::Initialize()
{
	m_stamina = 300.0f;
	m_isdead = false; // �v���C���[�̎��S�t���O��������
}

float	PlayerStamina::GetPlayerStaminaValue()
{
	return m_stamina;
}

void PlayerStamina::SetPlayerStaminaValueDirectly(float value)
{
	//�̗͒l���ő�l�𒴂������̏���
	m_stamina = value;
	if (m_stamina > MAX_STAMINA) {

		m_stamina = MAX_STAMINA;
	}
	if (m_stamina <= 0)
	{
		m_stamina = 0.0f;
		//�����Ŏc�@�����炷����
		app_atomex_start(Player_Dead_Sound);

		//��ʑJ�ڂ̂��߂Ƀv���C���[�̎��S�t���O�𗧂Ă�
		m_isdead = true;
	}
}


void	PlayerStamina::EditPlayerStaminaValue(float value)
{
	//�A�C�e���ɂ��_���[�W���
	if (value < 0)
	{
		Player& player = Player::GetInstance();
		ItemManager& item_manager = ItemManager::GetInstance();
		ItemBarrier* barrier = item_manager.FindItem_Barrier_ByOwnerBody(player.GetOutSidePlayerBody());

		if (barrier)
		{
			barrier->SetState(Barrier_Break);
			return;
		}
	}

	//�X�^�~�i���X�V
	m_stamina += value;

	if (m_stamina <= 0.0f)
	{
		//�����Ŏc�@�����炷����
		app_atomex_start(Player_Dead_Sound);

		//��ʑJ�ڂ̂��߂Ƀv���C���[�̎��S�t���O�𗧂Ă�
		m_isdead = true;
	}

	//�̗͂��ő�̗͂𒴂������̏���
	else if (m_stamina > MAX_STAMINA)
	{
		m_stamina = MAX_STAMINA;
	}
}

// �v���C���[�̎��S�t���O��Ԃ�
bool PlayerStamina::IsPlayerDead()
{
	return m_isdead;
}
