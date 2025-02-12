//-----------------------------------------------------------------------------------------------------
// #name player_stamina.h
// #description プレイヤーの体力値
// #make 2024/11/20			王泳心
// #update 2024/11/20
// #comment 追加・修正予定
//
//           ・残機と画面遷移追加されたら、体力0になった時の処理を追加
// 
//----------------------------------------------------------------------------------------------------


#pragma once

#ifndef PLAYER_STAMINA_H
#define	PLAYER_STAMINA_H

#define		MAX_STAMINA		(300.0)

class PlayerStamina
{
public:
	static void	Initialize();

	//今のプレイヤーの体力値をもらう
	static float	GetPlayerStaminaValue();

	//今のプレイヤーの体力値をマイナス、もしくはプラス（マイナスしたい場合は負数を渡してね！）
	static void	EditPlayerStaminaValue(float value);
	//今のプレイヤーの体力値を直接変更
	static void SetPlayerStaminaValueDirectly(float value);


	// プレイヤーの死亡フラグを取得
	static bool IsPlayerDead();

	static void SetPLayerDead(bool flag)
	{
		m_isdead = flag;
	}

	//アイテムによるダメージ回避あるかどうかを取得
	static bool	GetAvoidDamageOnce() { return m_avoid_damage_once; }
	//アイテムによるダメージ回避あるかどうかをセット
	static void	SetAvoidDamageOnce(bool flag) { m_avoid_damage_once = flag; }


private:
	PlayerStamina(){}
	~PlayerStamina(){}

	//プレイヤーの体力
	static  float	m_stamina;

	//アイテムによるダメージ回避あるかどうか
	static bool	m_avoid_damage_once;

	//プレイヤーの死亡フラグ
	static bool m_isdead;
};

#endif // !PLAYER_STAMINA_H

