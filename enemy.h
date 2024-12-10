//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description 動的、静的エネミーの継承元、エネミークラスのヘッダーファイル
// #make 2024/11/19
// #update 2024/12/04
// #comment 追加・修正予定
//          
//           
//----------------------------------------------------------------------------------------------------

#ifndef ENEMY_H
#define ENEMY_H

//==========マクロ定義==========//
#define ENEMY_MAX (50)

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
	//エネミーを倒した際に獲るスコア
	int m_score;
	
	//使用中判定
	bool m_use;
	//センサー内判定
	bool m_in_screen;
public:
	Enemy() = default;
	Enemy(int life, int damage, int soulgage, int score, bool use, bool in_screen)
		:m_life(life), m_damage(damage), m_soulgage(soulgage), m_score(score), m_use(use), m_in_screen(false) {}
	virtual ~Enemy() = default;

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

	bool GetUse()
	{
		return m_use;
	}
	void SetUse(bool use)
	{
		m_use = use;
	}

	bool GetInScreen()
	{
		return m_in_screen;
	}
	void SetInScreen(bool in_screen)
	{
		m_in_screen = in_screen;
	}

	virtual void UpdateEnemy() = 0;
};

#endif	//ENEMY_H