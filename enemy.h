//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description ���I�A�ÓI�G�l�~�[�̌p�����A�G�l�~�[�N���X�̃w�b�_�[�t�@�C��
// #make 2024/11/19
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_H
#define ENEMY_H

//==========�}�N����`==========//
#define ENEMY_MAX (100)

#include"world_box2d.h"

class Enemy
{
private:
	//�G�l�~�[�̗̑�
	int m_life;
	//�G�l�~�[���^����_���[�W
	int m_damage;
	//�G�l�~�[��|�����ۂɊl��\�E���Q�[�W
	int m_soulgage;
	//�G�l�~�[��|�����ۂɊl��X�R�A
	int m_score;
	//�g�p������
	bool m_use;
public:
	Enemy() = default;
	Enemy(int life, int damage, int soulgage, int score, bool use)
		:m_life(life), m_damage(damage), m_soulgage(soulgage), m_score(score), m_use(use) {}
	virtual ~Enemy() = default;

	int GetLife()
	{
		return m_life;
	}
	void SetLife(int life)
	{
		m_life = life;
	}

	int GetDamage()
	{
		return m_damage;
	}
	void SetDamage(int damage)
	{
		m_life = damage;
	}

	int GetSoulgage()
	{
		return m_soulgage;
	}
	void SetSoulgage(int soulgage)
	{
		m_life = soulgage;
	}

	bool GetUse()
	{
		return m_use;
	}
	void SetUse(bool use)
	{
		m_use = use;
	}

	virtual void UpdateEnemy() = 0;
};

#endif	//ENEMY_H