//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description ���I�A�ÓI�G�l�~�[�̌p�����A�G�l�~�[�N���X�̃w�b�_�[�t�@�C��
// #make 2024/11/19
// #update 2024/12/13
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
	//�I�u�W�F�N�g�ŗLID
	int m_id = -999;

	//body
	b2Body* m_body = nullptr;
	//�T�C�Y
	b2Vec2 m_size = b2Vec2(0.0f, 0.0f);

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
	//�Z���T�[������
	bool m_in_screen;
public:
	Enemy() = default;
	Enemy(int life, int damage, int soulgage, int score, bool use, bool in_screen)
		:m_life(life), m_damage(damage), m_soulgage(soulgage), m_score(score), m_use(use), m_in_screen(false) {}
	virtual ~Enemy() = default;

	int GetID()
	{
		return m_id;
	}
	void SetID(int id)
	{
		m_id = id;
	}

	b2Body* GetBody()
	{
		return m_body;
	}
	void SetBody(b2Body* body)
	{
		m_body = body;
	}

	b2Vec2 GetSize()
	{
		return m_size;
	}
	void SetSize(b2Vec2 size)
	{
		m_size = size;
	}

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

	bool GetInScreen()
	{
		return m_in_screen;
	}
	void SetInScreen(bool in_screen)
	{
		m_in_screen = in_screen;
	}

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//�G�l�~�[���v���C���[�ɐG�ꂽ���̏���
	void CollisionPlayer();
	//�G�l�~�[�������Ă����Ԃ̃I�u�W�F�N�g�ɐG�ꂽ���̏���
	void CollisionPulledObject();
	//�G�l�~�[���Z���T�[���ɓ��������̏���
	void InPlayerSensor();
	//�G�l�~�[���Z���T�[�O�ɏo�����̏���
	void OutPlayerSensor();
};

#endif	//ENEMY_H