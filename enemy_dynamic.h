//-----------------------------------------------------------------------------------------------------
// #name enemyDynamic.h
// #description 動的エネミー(プレイヤー追従)のヘッダーファイル
// #make 2024/11/20
// #update 2024/11/22
// #comment 追加・修正予定
//          ・
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_DYNAMIC_H
#define ENEMY_DYNAMIC_H

#include "enemy.h"

class EnemyDynamic :public Enemy
{
private:
	float m_speed = 0.1f;
public:
	EnemyDynamic() = default;
	EnemyDynamic(b2Vec2 size, b2Vec2 position, bool dynamic)
		:Enemy(size, position, dynamic) {}
	~EnemyDynamic() = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
};

#endif	//ENEMY_DYNAMIC_H