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
	float m_speed = 1.0f;
	bool	m_sensed_player = false;	//プレイヤーを感知したかどうか
public:
	EnemyFloating() = default;
	EnemyFloating(b2Vec2 position, b2Vec2 body_size, float angle);
	~EnemyFloating() = default;

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	bool	GetIfSensedPlayer() { return m_sensed_player; }
	void	SetIfSensedPlayer(bool flag) { m_sensed_player = flag; }

	//移動
	void Move();

};

#endif // !ENEMY_FLOATING_H


