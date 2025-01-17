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
#define		STAMINA_DAMAGE_BY_OBJECT_LEVEL_1	(-50.0)
#define		STAMINA_DAMAGE_BY_OBJECT_LEVEL_2	(-100.0)

class PlayerStamina
{
public:
	static void	Initialize();

	//今のプレイヤーの体力値をもらう
	static float	GetPlayerStaminaValue();

	//今のプレイヤーの体力値をマイナス、もしくはプラス（マイナスしたい場合は負数を渡してね！）
	static void	EditPlayerStaminaValue(float value);
private:
	PlayerStamina(){}
	~PlayerStamina(){}

	//プレイヤーの体力
	static  float	m_stamina;
};

#endif // !PLAYER_STAMINA_H

