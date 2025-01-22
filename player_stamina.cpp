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

void	PlayerStamina::EditPlayerStaminaValue(float value)
{

	float anchor_spirit = AnchorSpirit::GetAnchorSpiritValue();
	if (value < 0 && anchor_spirit > 0)
	{
		AnchorSpirit::EditAnchorSpiritValue(value);
		return;
	}

	//�X�^�~�i���X�V
	m_stamina += value;

	if (m_stamina <= 0.0f)
	{
		//�����Ŏc�@�����炷����

		//��ʑJ�ڂ̂��߂Ƀv���C���[�̎��S�t���O�𗧂Ă�
		m_isdead = true;
	}

	//�̗͂��ő�̗͂𒴂������̏���
	else if (m_stamina > MAX_STAMINA)
	{
		//�]�������̓\�E���Q�[�W���v���X
		AnchorSpirit::EditAnchorSpiritValue(m_stamina - MAX_STAMINA);
		m_stamina = MAX_STAMINA;
	}
}

// �v���C���[�̎��S�t���O��Ԃ�
bool PlayerStamina::IsPlayerDead()
{
	return m_isdead;
}
