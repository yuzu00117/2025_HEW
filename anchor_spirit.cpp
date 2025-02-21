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
bool AnchorSpirit::m_auto_heal = false;

void AnchorSpirit::Update()
{
	//������
	if (m_auto_heal && m_spirit != MAX_ANCHOR_SPIRIT)
	{
		EditAnchorSpiritValue(1.0f);
	}
}

void AnchorSpirit::Initialize()
{
	m_spirit = 50.0f;
	m_damage = 0.0f;
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
	//���̃\�E���l��ۑ����Ă���
	float prev_spirit = m_spirit;

		m_spirit += value;

		//�\�E���Q�[�W���}�C�i�X�ɂȂ�����
		// �]���̃_���[�W�̗͑͂ɍs��
		if (m_spirit < 0.0f)
		{
			PlayerStamina::EditPlayerStaminaValue(m_spirit);
			m_spirit = 0.0f;
		}

		//�\�E���l���ő�l�𒴂������̏���
		if (m_spirit > MAX_ANCHOR_SPIRIT)
		{
			m_spirit = MAX_ANCHOR_SPIRIT;
		}

	//�󂯂��_���[�W�̌v�Z
	m_damage = prev_spirit - m_spirit;
	if (m_damage < 0) { m_damage = 0.0f; }
	
}

void AnchorSpirit::SetAnchorSpiritValueDirectly(float value)
{
	//���̃\�E���l��ۑ����Ă���
	float prev_spirit = m_spirit;

	m_spirit = value;
	//�\�E���l���ő�l�𒴂������̏���
	if (m_spirit > MAX_ANCHOR_SPIRIT)
	{
		m_spirit = MAX_ANCHOR_SPIRIT;
	}
	//�\�E���l���}�C�i�X�ɂȂ������̏���
	if (m_spirit < 0)
	{
		m_spirit = 0.0f;
	}

	//�󂯂��_���[�W�̌v�Z
	m_damage = prev_spirit - m_spirit;
	if (m_damage < 0) { m_damage = 0.0f; }

}


