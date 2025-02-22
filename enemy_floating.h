//-----------------------------------------------------------------------------------------------------
// #name enemy_floating.h
// #description 浮遊敵
// #make 2025/1/17　王泳心
// #update 2025/1/17 
// #comment 追加・修正予定
// 
// 
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_FLOATING_H
#define ENEMY_FLOATING_H

#include "enemy.h"
#include "player_stamina.h"

//==========マクロ定義==========//
#define ENEMY_FLOATING_LIFE (100)
#define ENEMY_FLOATING_DAMAGE (MAX_STAMINA/2)
#define ENEMY_FLOATING_SPIRIT_TYPE (Spirit_M)
#define ENEMY_FLOATING_SCORE (200)

enum ENEMY_FLOATING_STATE
{
	ENEMY_FLOATING_STATE_NULL,
	ENEMY_FLOATING_STATE_IDLE,
	ENEMY_FLOATING_STATE_MOVE,
	ENEMY_FLOATING_STATE_DIE,
};


class EnemyFloating : public Enemy
{
private:
	b2Body*	m_body = nullptr;
	float m_speed = 0.2f;
	bool	m_sensed_player = false;	//プレイヤーを感知したかどうか
	int m_anim_id = 0;

	int m_attack_cooling_time = 0;	//攻撃クールタイム

	//エネミーの状態(動作)
	ENEMY_FLOATING_STATE m_state = ENEMY_FLOATING_STATE_IDLE;

public:
	EnemyFloating() = default;
	EnemyFloating(b2Vec2 position, b2Vec2 body_size, float angle);
	~EnemyFloating() = default;

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	ENEMY_FLOATING_STATE GetState()
	{
		return m_state;
	}
	void SetState(ENEMY_FLOATING_STATE state)
	{
		m_state = state;
		if (m_state == ENEMY_FLOATING_STATE_DIE)
		{
			m_anim_id = 0;
		}
	}
	
	//浮遊敵のセンサーがプレイヤーを検知しているかどうかを取得
	bool	GetIfSensedPlayer() { return m_sensed_player; }
	//浮遊敵のセンサーがプレイヤーを検知しているかどうかをセット
	void	SetIfSensedPlayer(bool flag) { 
		m_sensed_player = flag; 
		m_state = ENEMY_FLOATING_STATE_MOVE;
		if (m_sensed_player == false)
		{
			m_state = ENEMY_FLOATING_STATE_IDLE;
			//追尾止めるために残ってる移動量を消す
			GetBody()->SetLinearVelocity(b2Vec2(0.0f,0.0f));
		}
	}

	//攻撃クールタイムを取得
	int	GetAttactCoolingTime() { return m_attack_cooling_time; }
	//攻撃クールタイムをセット
	void	SetAttactCoolingTime(int time) { m_attack_cooling_time = time; }

	//移動
	void Move();

};

#endif // !ENEMY_FLOATING_H


