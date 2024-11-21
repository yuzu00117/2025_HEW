//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description 静的エネミーのヘッダーファイル
// #make 2024/11/19
// #update 2024/11/22
// #comment 追加・修正予定
//          ・
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_STATIC_H
#define ENEMY_STATIC_H

#include "enemy.h"

class EnemyStatic :public Enemy
{
private:

public:
	EnemyStatic() = default;
	EnemyStatic(b2Vec2 size, b2Vec2 position, bool dynamic)
		:Enemy(size, position, dynamic) {}
	~EnemyStatic() = default;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
};

#endif	//ENEMY_STATIC_H