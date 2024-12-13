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
#define ENEMY_DYNAMIC_SOULGAGE (100)
#define ENEMY_DYNAMIC_SCORE (200)

class EnemyDynamic :public Enemy
{
private:
	float m_speed = 0.005f;
public:
	EnemyDynamic() = default;
	EnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle);
	~EnemyDynamic() = default;

	void Initialize() override;
	void Finalize() override;
	void Update() override;
	void Draw() override;

	//移動
	void Move();
};

#endif	//ENEMY_DYNAMIC_H