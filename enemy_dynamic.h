//-----------------------------------------------------------------------------------------------------
// #name enemyDynamic.h
// #description 動的エネミー(プレイヤー追従)のヘッダーファイル
// #make 2024/11/20
// #update 2024/12/04
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

class EnemyDynamic :public Enemy, public Field
{
private:
	float m_speed = 0.005f;
public:
	EnemyDynamic() = default;
	EnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture);
	~EnemyDynamic() = default;

	static void Update();
	virtual void UpdateEnemy();
	//エネミーがプレイヤーに触れた時の処理
	static void CollisionPlayer(b2Body* collision_enemy);
	//エネミーが動いている状態のオブジェクトに触れた時の処理
	static void CollisionPulledObject(b2Body* collision_enemy);
	//エネミーがセンサー内に入った時の処理
	static void InPlayerSensor(b2Body* collision_enemy);
	//エネミーがセンサー外に出た時の処理
	static void OutPlayerSensor(b2Body* collision_enemy);
	static void Finalize();
};

#endif	//ENEMY_DYNAMIC_H