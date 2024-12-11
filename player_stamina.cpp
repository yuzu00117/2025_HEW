#include "player_stamina.h"
#include "anchor_spirit.h"
#include "scene.h"



// �ÓI�����o�ϐ��̒�`�i1�񂾂��s���j
float	PlayerStamina::m_stamina = 300.0f;


float	PlayerStamina::GetPlayerStaminaValue()
{
	return m_stamina;
}

void	PlayerStamina::EditPlayerStaminaValue(float value)
{
	m_stamina += value;

	if (m_stamina <= 0.0f)
	{
		//�����Ŏc�@�����炷����
		



		//�����ŉ�ʑJ��

		ChangeScene(SCENE_GAMEOVER);

	}

	//�̗͂��ő�̗͂𒴂������̏���
	if (m_stamina > MAX_STAMINA)
	{
		//�]�������̓\�E���Q�[�W���v���X
		AnchorSpirit::EditAnchorSpiritValue(m_stamina - MAX_STAMINA);
		m_stamina = MAX_STAMINA;
	}
}


