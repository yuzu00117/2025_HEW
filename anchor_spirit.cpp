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


float AnchorSpirit::m_spirit = 50.0f;

void AnchorSpirit::Initialize()
{
	m_spirit = 50.0f;
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
			m_spirit = 0.0f;
			//��ł��̍s�ɃI�u�W�F�N�g�̃��x�����擾���鏈����ǉ�
			//���̓I�u�W�F�N�g�̃��x�����b��object_level��
			int	object_level = 1;
			//�v���C���[�̗̑͂�����鏈��
			switch (object_level)
			{
			case 1:
				PlayerStamina::EditPlayerStaminaValue(STAMINA_DAMAGE_BY_OBJECT_LEVEL_1);
				break;
			case 2:
				PlayerStamina::EditPlayerStaminaValue(STAMINA_DAMAGE_BY_OBJECT_LEVEL_2);
				break;
			default:
				break;
			}
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
		//�����d�l�͂�����ƌ�Ŕ~�Ɋm�F
		//�����v���X�̏����̏ꍇ
		if (value > 0)
		{
			//�v���X�����l���v���C���[�̗̑͂ɓ����
			PlayerStamina::EditPlayerStaminaValue(value);
		}
		else
		{
			//��ł��̍s�ɃI�u�W�F�N�g�̃��x�����擾���鏈����ǉ�
			//���̓I�u�W�F�N�g�̃��x�����b��object_level��
			int	object_level = 1;
			//�v���C���[�̗̑͂�����鏈��
			switch (object_level)
			{
			case 1:
				PlayerStamina::EditPlayerStaminaValue(STAMINA_DAMAGE_BY_OBJECT_LEVEL_1);
				break;
			case 2:
				PlayerStamina::EditPlayerStaminaValue(STAMINA_DAMAGE_BY_OBJECT_LEVEL_2);
				break;
			default:
				break;
			}


		}//end_if(value<0)



	}//end_if(�v���C���[�̗̑͂�MAX����Ȃ�)


	
}

