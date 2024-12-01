//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description 静的エネミーのヘッダーファイル
// #make 2024/11/19
// #update 2024/11/29
// #comment 追加・修正予定
//          ・
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_STATIC_H
#define ENEMY_STATIC_H

//==========マクロ定義==========//
#define ENEMY_STATIC_LIFE (100)
#define ENEMY_STATIC_DAMAGE (1)
#define ENEMY_STATIC_SOULGAGE (10)

#include "enemy.h"
#include "field.h"

class EnemyStatic :public Enemy, public Field
{
private:

public:
	EnemyStatic() = default;
	EnemyStatic(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture);
	~EnemyStatic() {  };

	virtual void UpdateEnemy();
};

#endif	//ENEMY_STATIC_H