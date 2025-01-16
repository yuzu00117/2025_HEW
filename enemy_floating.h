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
#define ENEMY_FLOATING_SOULGAGE (50)
#define ENEMY_FLOATING_SCORE (200)


class EnemyFloating : public Enemy
{
private:
	b2Body*	m_body = nullptr;
	float m_speed = 1.5f;
	bool	m_sensed_player = false;	//プレイヤーを感知したかどうか
public:
	EnemyFloating() = default;
	EnemyFloating(b2Vec2 position, b2Vec2 body_size, float angle);
	~EnemyFloating() = default;

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;
	
	//浮遊敵のセンサーがプレイヤーを検知しているかどうかを取得
	bool	GetIfSensedPlayer() { return m_sensed_player; }
	//浮遊敵のセンサーがプレイヤーを検知しているかどうかをセット
	void	SetIfSensedPlayer(bool flag) { 
		m_sensed_player = flag; 
		if (m_sensed_player == false)
		{
			//追尾止めるために残ってる移動量を消す
			GetBody()->SetLinearVelocity(b2Vec2(0.0f,0.0f));
		}
	}

	//移動
	void Move();

};

#endif // !ENEMY_FLOATING_H


