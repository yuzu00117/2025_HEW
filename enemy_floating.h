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
#include <vector>

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
	ENEMY_FLOATING_STATE_EXPLODE,
	ENEMY_FLOATING_STATE_DIE,
};


class EnemyFloating : public Enemy
{
private:
	float m_speed = 0.2f;
	bool	m_sensed_player = false;	//プレイヤーを感知したかどうか
	float m_anim_id = 0;

	//エネミーの状態(動作)
	ENEMY_FLOATING_STATE m_state;

	std::unique_ptr<class ObjectData> m_object_data;
	std::unique_ptr<class ObjectData> m_sensor_data;
public:
	EnemyFloating() = default;
	EnemyFloating(b2Vec2 position, b2Vec2 body_size, float angle);
	~EnemyFloating() = default;

	//フィルターを変換できる
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

	
	//浮遊敵のセンサーがプレイヤーを検知しているかどうかを取得
	bool	GetIfSensedPlayer() { return m_sensed_player; }
	//浮遊敵のセンサーがプレイヤーを検知しているかどうかをセット
	void	SetIfSensedPlayer(bool flag) { 
		m_sensed_player = flag; 
		SetState(ENEMY_FLOATING_STATE_MOVE);
		if (m_sensed_player == false)
		{
			SetState(ENEMY_FLOATING_STATE_IDLE);
			//追尾止めるために残ってる移動量を消す
			GetBody()->SetLinearVelocity(b2Vec2(0.0f,0.0f));
		}
	}
	//移動
	void Move();

	

};

#endif // !ENEMY_FLOATING_H


