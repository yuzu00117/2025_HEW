//-----------------------------------------------------------------------------------------------------
// #name player_stamina.h
// #description �v���C���[�̗̑͒l
// #make 2024/11/20			���j�S
// #update 2024/11/20
// #comment �ǉ��E�C���\��
//
//           �E�c�@�Ɖ�ʑJ�ڒǉ����ꂽ��A�̗�0�ɂȂ������̏�����ǉ�
// 
//----------------------------------------------------------------------------------------------------


#pragma once

#ifndef PLAYER_STAMINA_H
#define	PLAYER_STAMINA_H

#define		MAX_STAMINA		(300.0)

class PlayerStamina
{
public:
	static void	Initialize();

	//���̃v���C���[�̗̑͒l�����炤
	static float	GetPlayerStaminaValue();

	//���̃v���C���[�̗̑͒l���}�C�i�X�A�������̓v���X�i�}�C�i�X�������ꍇ�͕�����n���ĂˁI�j
	static void	EditPlayerStaminaValue(float value);

	// �v���C���[�̎��S�t���O���擾
	static bool IsPlayerDead();

private:
	PlayerStamina(){}
	~PlayerStamina(){}

	//�v���C���[�̗̑�
	static  float	m_stamina;

	//�v���C���[�̎��S�t���O
	static bool m_isdead;
};

#endif // !PLAYER_STAMINA_H

