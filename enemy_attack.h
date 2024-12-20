//-----------------------------------------------------------------------------------------------------
// #name enemyAttack.h
// #description エネミーの攻撃のヘッダーファイル
// #make 2024/12/20
// #update 2024/12/20
// #comment 追加・修正予定
//          ・ステータス調整
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_ATTACK_H
#define ENEMY_ATTACK_H

#include "enemy.h"
#include "player_stamina.h"

//==========マクロ定義==========//
#define ENEMY_ATTACK_DAMAGE (100)

class EnemyAttack
{
private:
	//オブジェクト固有ID
	int m_id = -999;

	//body
	b2Body* m_body = nullptr;
	//サイズ
	b2Vec2 m_size = b2Vec2(0.0f, 0.0f);

	int m_damage = ENEMY_ATTACK_DAMAGE;
	int m_frame = 15;
	int m_count = 0;
	bool m_hit = false;
public:
	EnemyAttack() = default;
	EnemyAttack(b2Vec2 position, b2Vec2 body_size, float angle);
	~EnemyAttack() = default;

	int GetID()
	{
		return m_id;
	}
	void SetID(int id)
	{
		m_id = id;
	}

	b2Body* GetBody()
	{
		return m_body;
	}
	void SetBody(b2Body* body)
	{
		m_body = body;
	}

	b2Vec2 GetSize()
	{
		return m_size;
	}
	void SetSize(b2Vec2 size)
	{
		m_size = size;
	}

	int GetDamage()
	{
		return m_damage;
	}
	void SetDamage(int damage)
	{
		m_damage = damage;
	}

	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	//攻撃がプレイヤーに触れた時の処理
	void CollisionPlayer();
};

#endif	//ENEMY_ATTACK