//-----------------------------------------------------------------------------------------------------
// #name enemyDynamic.h
// #description ���I�G�l�~�[(�v���C���[�Ǐ])�̃w�b�_�[�t�@�C��
// #make 2024/11/20
// #update 2024/12/13
// #comment �ǉ��E�C���\��
//          �E
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_DYNAMIC_H
#define ENEMY_DYNAMIC_H

#include "enemy.h"
#include "player_stamina.h"

//==========�}�N����`==========//
#define ENEMY_DYNAMIC_LIFE (100)
#define ENEMY_DYNAMIC_DAMAGE (MAX_STAMINA/2)
#define ENEMY_DYNAMIC_SOULGAGE (100)
#define ENEMY_DYNAMIC_SCORE (200)

enum ENEMY_DYNAMIC_STATE
{
	ENEMY_STATE_NULL,
	ENEMY_STATE_MOVE,
	ENEMY_STATE_ATTACK,
	ENEMY_STATE_DESTROYED,
};

class EnemyDynamic :public Enemy
{
private:
	//�G�l�~�[�Ǐ��(����)
	int m_state = ENEMY_STATE_NULL;
	const float m_speed = 0.01f;
	//������ true : �E���� false
	bool m_direction = true;

	int m_attack_ID = -999;
	int m_attack_counter = 0;
	const int m_attack_birth = 30;
	const int m_attack_finish = 45;

	b2Vec2 m_size_sensor = b2Vec2(0.0,0.0);
public:
	EnemyDynamic() = default;
	EnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle);
	~EnemyDynamic() = default;

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

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	//�ړ�
	void Move();
	//�U��
	void Attack();

	//�Z���T�[�ƃv���C���[���G�ꂽ���̏���
	void CollisionSensorPlayer();
};

#endif	//ENEMY_DYNAMIC_H