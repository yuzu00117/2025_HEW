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
	//���̃v���C���[�̗̑͒l�𒼐ڕύX
	static void SetPlayerStaminaValueDirectly(float value);


	// �v���C���[�̎��S�t���O���擾
	static bool IsPlayerDead();

	static void SetPLayerDead(bool flag)
	{
		m_isdead = flag;
	}

	//�A�C�e���ɂ��_���[�W������邩�ǂ������擾
	static bool	GetAvoidDamageOnce() { return m_avoid_damage_once; }
	//�A�C�e���ɂ��_���[�W������邩�ǂ������Z�b�g
	static void	SetAvoidDamageOnce(bool flag) { m_avoid_damage_once = flag; }


private:
	PlayerStamina(){}
	~PlayerStamina(){}

	//�v���C���[�̗̑�
	static  float	m_stamina;

	//�A�C�e���ɂ��_���[�W������邩�ǂ���
	static bool	m_avoid_damage_once;

	//�v���C���[�̎��S�t���O
	static bool m_isdead;
};

#endif // !PLAYER_STAMINA_H

