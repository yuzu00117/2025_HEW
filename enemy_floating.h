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
#include <vector>

//==========�}�N����`==========//
#define ENEMY_FLOATING_LIFE (100)
#define ENEMY_FLOATING_DAMAGE (MAX_STAMINA/2)
#define ENEMY_FLOATING_SPIRIT_TYPE (Spirit_M)
#define ENEMY_FLOATING_SCORE (200)

enum ENEMY_FLOATING_STATE
{
	ENEMY_FLOATING_STATE_NULL,
	ENEMY_FLOATING_STATE_IDLE,
	ENEMY_FLOATING_STATE_MOVE,
	ENEMY_FLOATING_STATE_EXPLODE,
	ENEMY_FLOATING_STATE_DIE,
};


class EnemyFloating : public Enemy
{
private:
	float m_speed = 0.2f;
	bool	m_sensed_player = false;	//�v���C���[�����m�������ǂ���
	float m_anim_id = 0;

	//�G�l�~�[�̏��(����)
	ENEMY_FLOATING_STATE m_state;

	std::unique_ptr<class ObjectData> m_object_data;
	std::unique_ptr<class ObjectData> m_sensor_data;
public:
	EnemyFloating() = default;
	EnemyFloating(b2Vec2 position, b2Vec2 body_size, float angle);
	~EnemyFloating() = default;

	//�t�B���^�[��ϊ��ł���
	void updateFixtureFilter(const std::string& category, const std::vector<std::string>& includeMasks);


	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	ENEMY_FLOATING_STATE GetState()
	{
		return m_state;
	}
	void SetState(ENEMY_FLOATING_STATE state);

	
	//���V�G�̃Z���T�[���v���C���[�����m���Ă��邩�ǂ������擾
	bool	GetIfSensedPlayer() { return m_sensed_player; }
	//���V�G�̃Z���T�[���v���C���[�����m���Ă��邩�ǂ������Z�b�g
	void	SetIfSensedPlayer(bool flag) { 
		m_sensed_player = flag; 
		SetState(ENEMY_FLOATING_STATE_MOVE);
		if (m_sensed_player == false)
		{
			SetState(ENEMY_FLOATING_STATE_IDLE);
			//�ǔ��~�߂邽�߂Ɏc���Ă�ړ��ʂ�����
			GetBody()->SetLinearVelocity(b2Vec2(0.0f,0.0f));
		}
	}
	//�ړ�
	void Move();

	

};

#endif // !ENEMY_FLOATING_H


