//-----------------------------------------------------------------------------------------------------
// #name enemyDynamic.h
// #description 動的エネミー(プレイヤー追従)のヘッダーファイル
// #make 2024/11/20
// #update 2024/11/29
// #comment 追加・修正予定
//          ・
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_DYNAMIC_H
#define ENEMY_DYNAMIC_H

#include "enemy.h"
#include "field.h"

//==========マクロ定義==========//
#define ENEMY_DYNAMIC_LIFE (100)
#define ENEMY_DYNAMIC_DAMAGE (MAX_STAMINA/2)
#define ENEMY_DYNAMIC_SOULGAGE (10)
#define ENEMY_DYNAMIC_SCORE (200)

class EnemyDynamic :public Enemy, public Field
{
private:
	float m_speed = 0.1f;
public:
	EnemyDynamic() = default;
	EnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture);
	~EnemyDynamic() = default;

	static void Update();
	virtual void UpdateEnemy();
	static void CollisionPlayer(b2Body* collision_enemy);
	static void CollisionAnchorPoint(b2Body* collision_enemy);
};

#endif	//ENEMY_DYNAMIC_H