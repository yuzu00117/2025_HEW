//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description ���I�A�ÓI�G�l�~�[�̌p�����A�G�l�~�[�N���X�̃w�b�_�[�t�@�C��
// #make 2024/11/19
// #update 2024/11/22
// #comment �ǉ��E�C���\��
//          �E�v���C���[�ƏՓ˂ő̗͂����֐��m�ǉ�
//			�E�|�ꂽ�I�u�W�F�N�g�Ɋ������܂�ď����鏈���m�ǉ�
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_H
#define ENEMY_H

//==========�}�N����`==========//
#define ENEMY_MAX (10)

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
public:
	Enemy() = default;
	Enemy(int life, int damage, int soulgage)
		:m_life(life), m_damage(damage), m_soulgage(soulgage) {}
	virtual ~Enemy() = default;

	static void SetEnemy(Enemy* enemy);

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

	static void Update();
	virtual void UpdateEnemy() = 0;
	bool CheckEnemy();
	static void Finalize();
};

#endif	//ENEMY_H