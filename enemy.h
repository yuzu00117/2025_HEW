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

//==========マクロ定義==========//
#define ENEMY_MAX (10)

#include"world_box2d.h"

class Enemy
{
private:
	//エネミーの体力
	int m_life;
	//エネミーが与えるダメージ
	int m_damage;
	//エネミーを倒した際に獲るソウルゲージ
	int m_soulgage;
public:
	Enemy() = default;
	Enemy(int life, int damage, int soulgage)
		:m_life(life), m_damage(damage), m_soulgage(soulgage) {}
	virtual ~Enemy() = default;

	static void SetEnemy(Enemy* enemy);

	int GetLife()
	{
		return m_life;
	}
	void SetLife(int life)
	{
		m_life = life;
	}

	int GetDamage()
	{
		return m_damage;
	}
	void SetDamage(int damage)
	{
		m_life = damage;
	}

	int GetSoulgage()
	{
		return m_soulgage;
	}
	void SetSoulgage(int soulgage)
	{
		m_life = soulgage;
	}

	static void Update();
	virtual void UpdateEnemy() = 0;
	bool CheckEnemy();
	static void Finalize();
};

#endif	//ENEMY_H