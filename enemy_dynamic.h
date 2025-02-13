//-----------------------------------------------------------------------------------------------------
// #name enemyDynamic.h
// #description 動的エネミー(プレイヤー追従)のヘッダーファイル
// #make 2024/11/20
// #update 2024/12/13
// #comment 追加・修正予定
//          ・
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_DYNAMIC_H
#define ENEMY_DYNAMIC_H

#include "enemy.h"
#include "field.h"
#include "player_stamina.h"

//==========マクロ定義==========//
#define ENEMY_DYNAMIC_LIFE (100)
#define ENEMY_DYNAMIC_DAMAGE (MAX_STAMINA/2)
#define ENEMY_DYNAMIC_SPIRIT_TYPE (Spirit_S)
#define ENEMY_DYNAMIC_SCORE (200)

enum ENEMY_DYNAMIC_STATE
{
	ENEMY_STATE_NULL,
	ENEMY_STATE_MOVE,
	ENEMY_STATE_ATTACK,
};

class EnemyDynamic :public Enemy
{
private:
	//エネミーの状態(動作)
	int m_state = ENEMY_STATE_NULL;
	int m_old_state = ENEMY_STATE_NULL;
	const float m_speed = 0.006f;
	const float m_jump_force = -0.1;
	float m_move_force;
	//左向き true : 右向き false
	bool m_direction = true;
	bool m_is_ground = false;
	int m_ground_cnt = 0;
	int m_old_ground_cnt = 0;
	int m_sensor_move_size = 0;

	//攻撃関連
	int m_attack_ID = -999;
	int m_attack_counter = 0;
	const int m_attack_birth = 30;
	const int m_attack_finish = 50;
	const int m_attack_cooltime = 60;
	int m_attack_cooltime_counter = 60;

	int m_anim_id = 0;

	//テスト用
	b2Vec2 m_size_sensor = b2Vec2(0.0, 0.0);
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
	//プレイヤーの位置を基に向きを決める
	void SetDirectionBasedOnPlayer();

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	//移動
	void Move();
	//攻撃
	void Attack();

	//センサーとプレイヤーが触れた時の処理
	void CollisionSensorPlayer();
	void SetIsGround(bool is_ground)
	{
		if (is_ground)
		{
			m_ground_cnt++;
		}
		else
		{
			m_ground_cnt--;
		}
	}
};

#endif	//ENEMY_DYNAMIC_H