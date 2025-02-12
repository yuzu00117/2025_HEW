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
bool AnchorSpirit::m_auto_heal = false;

void AnchorSpirit::Update()
{
	if (m_auto_heal)
	{
		EditAnchorSpiritValue(MAX_ANCHOR_SPIRIT);
	}
}

void AnchorSpirit::Initialize()
{
	m_spirit = 50.0f;
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
	

	//�v���C���[�̗̑͂�MAX�̎������\�E���l�̍X�V�ł���
	if (PlayerStamina::GetPlayerStaminaValue() >= MAX_STAMINA)
	{
		m_spirit += value;

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

	}
	//�v���C���[�̗̑͂�MAX����Ȃ����͍X�V�������v���C���[�̗̑͂ł���
	else
	{
			PlayerStamina::EditPlayerStaminaValue(value);

	}//end_if(�v���C���[�̗̑͂�MAX����Ȃ�)


	
}

void AnchorSpirit::SetAnchorSpiritValueDirectly(float value)
{
	m_spirit = value;
	if (m_spirit > 0)
	{
		PlayerStamina::SetPlayerStaminaValueDirectly(MAX_STAMINA);
	}
}


