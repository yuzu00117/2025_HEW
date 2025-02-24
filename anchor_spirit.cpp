//-----------------------------------------------------------------------------------------------------
// #name anchor_spirit.cpp
// #description �A���J�[�̃\�E���Q�[�W�̒l
// #make 2024/11/20			���j�S
// #update 2024/11/20
// #comment �ǉ��E�C���\��
//
//           �E�A���J�[�ǉ����ꂽ��A�\�E���Q�[�W�̒l�����l�ɂȂ����Ƃ��̏�����ǉ�����
// 
//----------------------------------------------------------------------------------------------------


#include"anchor_spirit.h"
#include"player_stamina.h"
#include"sound.h"


float AnchorSpirit::m_spirit = 50.0f;
float AnchorSpirit::m_damage = 0.0f;
float AnchorSpirit::m_increase = 0.0f;
bool AnchorSpirit::m_auto_heal = false;

void AnchorSpirit::Update()
{
	//������
	if (m_auto_heal && m_spirit != MAX_ANCHOR_SPIRIT)
	{
		EditAnchorSpiritValue(1.0f);
	}
	//�_���[�W���Q�[�W�̐��l����C�Ɍ��炷
	if (m_damage > 0)
	{
		m_damage -= 0.5f;
	}
	//�����̓Q�[�W�̐��l�����X�ɑ��₷
	if (m_increase > 0)
	{
		m_increase -= 1.0f;
		m_spirit += 1.0f;
	}

	//�\�E���l���ő�l�𒴂������̏���
	if (m_spirit > MAX_ANCHOR_SPIRIT)
	{
		m_spirit = MAX_ANCHOR_SPIRIT;
	}
	//�\�E���l���}�C�i�X�ɂȂ������̏���
	if (m_spirit < 0)
	{
		// �]���̃_���[�W�̗͑͂ɍs��
		PlayerStamina::EditPlayerStaminaValue(m_spirit);
		m_spirit = 0.0f;
	}
}

void AnchorSpirit::Initialize()
{
	m_spirit = 50.0f;
	m_damage = 0.0f;
	m_increase = 0.0f;
	m_auto_heal = false;
}

//���̃\�E���Q�[�W�̒l�����炤
float	AnchorSpirit::GetAnchorSpiritValue()
{
	return m_spirit;
}




//���̃\�E���Q�[�W�̒l���}�C�i�X�A�������̓v���X�i�}�C�i�X�������ꍇ�͕�����n���ĂˁI�j
void	AnchorSpirit::EditAnchorSpiritValue(float value)
{
	//�_���[�W�i����j����
	if (value < 0)
	{
		m_damage += -value;
		m_spirit += value;
		//�\�E���l���}�C�i�X�ɂȂ������̏���
		if (m_spirit < 0)
		{
			// �]���̃_���[�W�̗͑͂ɍs��
			PlayerStamina::EditPlayerStaminaValue(m_spirit);
			m_spirit = 0.0f;
		}

	}
	//�@��������
	else if (value > 0)
	{
		m_increase += value;
	}

	
}

void AnchorSpirit::SetAnchorSpiritValueDirectly(float value)
{
	//�ς������
	float change = value - m_spirit;
	
	//��������
	if (change > 0)
	{
		m_increase = change;

	}
	//�_���[�W�i����j����
	else
	{
		m_damage += -change;
		m_spirit = value;
		//�\�E���l���}�C�i�X�ɂȂ������̏���
		if (m_spirit < 0)
		{
			// �]���̃_���[�W�̗͑͂ɍs��
			PlayerStamina::EditPlayerStaminaValue(m_spirit);
			m_spirit = 0.0f;
		}

	}

}


