//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description �ÓI�G�l�~�[�̃w�b�_�[�t�@�C��
// #make 2024/11/19
// #update 2025/02/10
// #comment �ǉ��E�C���\��
//          �E
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_STATIC_H
#define ENEMY_STATIC_H

#include "enemy.h"
#include "field.h"
#include "player_stamina.h"

//==========�}�N����`==========//
#define ENEMY_STATIC_LIFE (100)
#define ENEMY_STATIC_DAMAGE (MAX_STAMINA/2)
#define ENEMY_STATIC_SOULGAGE (100)
#define ENEMY_STATIC_SCORE (100)

enum ENEMY_STATIC_STATE
{
	ENEMY_STATIC_STATE_NULL,
	ENEMY_STATIC_STATE_IDLE,
	ENEMY_STATIC_STATE_ATTACK,
};

class EnemyStatic :public Enemy
{
private:
	//�G�l�~�[�̏��(����)
	int m_state = ENEMY_STATIC_STATE_NULL;
	int m_old_state = ENEMY_STATIC_STATE_NULL;

	//������ true : �E���� false
	bool m_direction = true;

	//�U���֘A
	int m_attack_ID = -999;
	int m_attack_counter = 0;
	const int m_attack_birth = 30;
	const int m_attack_finish = 50;
	const int m_attack_cooltime = 60;
	int m_attack_cooltime_counter = 60;

	int m_anim_id = 0;

	//�e�X�g�p
	b2Vec2 m_size_sensor = b2Vec2(0.0, 0.0);
public:
	EnemyStatic() = default;
	EnemyStatic(b2Vec2 position, b2Vec2 body_size, float angle);
	~EnemyStatic() = default;
	

	int GetState()
	{
		return m_state;
	}
	void SetState(int state)
	{
		m_state = state;
	}

	bool GetDirection()
	{
		return m_direction;
	}
	void SetDirection(bool direction)
	{
		m_direction = direction;
	}
	//�v���C���[�̈ʒu����Ɍ��������߂�
	void SetDirectionBasedOnPlayer();

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	//�ҋ@
	void Idle();
	//�U��
	void Attack();

	//�Z���T�[�ƃv���C���[���G�ꂽ���̏���
	void CollisionSensorPlayer();
};

#endif	//ENEMY_STATIC_H