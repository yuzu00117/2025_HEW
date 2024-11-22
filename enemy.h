//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description 動的、静的エネミーの継承元、エネミークラスのヘッダーファイル
// #make 2024/11/19
// #update 2024/11/22
// #comment 追加・修正予定
//          ・プレイヤーと衝突で体力を削る関数ノ追加
//			・倒れたオブジェクトに巻き込まれて消える処理ノ追加
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_H
#define ENEMY_H

#include"world_box2d.h"

class Enemy
{
private:
	//エネミーのBodyをもつ
	b2Body* m_body;

	//描画用のBodyのサイズもっておく　
	b2Vec2 m_size;
public:
	Enemy() = default;
	Enemy(b2Vec2 body_size, b2Vec2 position, bool dynamic);
	virtual ~Enemy() = default;

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Finalize();

	b2Body* GetEnemyBody()
	{
		return m_body;
	}
	void SetEnemyBody(b2Body* body)
	{
		m_body = body;
	}

	b2Vec2 GetEnemySize()
	{
		return m_size;
	}
	void SetEnemySize(b2Vec2 size)
	{
		m_size = size;
	}
	
	//ここでやられ判定付ける
};

#endif	//ENEMY_H