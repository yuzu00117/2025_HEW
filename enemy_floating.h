//-----------------------------------------------------------------------------------------------------
// #name enemy_floating.h
// #description ���V�G
// #make 2025/1/17�@���j�S
// #update 2025/1/17 
// #comment �ǉ��E�C���\��
// 
// 
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_FLOATING_H
#define ENEMY_FLOATING_H

#include "enemy.h"
#include "player_stamina.h"

//==========�}�N����`==========//
#define ENEMY_FLOATING_LIFE (100)
#define ENEMY_FLOATING_DAMAGE (MAX_STAMINA/2)
#define ENEMY_FLOATING_SOULGAGE (50)
#define ENEMY_FLOATING_SCORE (200)


class EnemyFloating : public Enemy
{
private:
	b2Body*	m_body = nullptr;
	float m_speed = 1.5f;
	bool	m_sensed_player = false;	//�v���C���[�����m�������ǂ���
public:
	EnemyFloating() = default;
	EnemyFloating(b2Vec2 position, b2Vec2 body_size, float angle);
	~EnemyFloating() = default;

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	
	//���V�G�̃Z���T�[���v���C���[�����m���Ă��邩�ǂ������擾
	bool	GetIfSensedPlayer() { return m_sensed_player; }
	//���V�G�̃Z���T�[���v���C���[�����m���Ă��邩�ǂ������Z�b�g
	void	SetIfSensedPlayer(bool flag) { 
		m_sensed_player = flag; 
		if (m_sensed_player == false)
		{
			//�ǔ��~�߂邽�߂Ɏc���Ă�ړ��ʂ�����
			GetBody()->SetLinearVelocity(b2Vec2(0.0f,0.0f));
		}
	}

	//�ړ�
	void Move();

};

#endif // !ENEMY_FLOATING_H


