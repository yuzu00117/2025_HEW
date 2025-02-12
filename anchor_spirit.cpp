//-----------------------------------------------------------------------------------------------------
// #name anchor_spirit.cpp
// #description アンカーのソウルゲージの値
// #make 2024/11/20			王泳心
// #update 2024/11/20
// #comment 追加・修正予定
//
//           ・アンカー追加されたら、ソウルゲージの値が一定値になったときの処理を追加する
// 
//----------------------------------------------------------------------------------------------------


#include"anchor_spirit.h"
#include"player_stamina.h"
#include"sound.h"


float AnchorSpirit::m_spirit = 50.0f;
bool AnchorSpirit::m_auto_heal = false;

void AnchorSpirit::Update()
{
	if (m_auto_heal)
	{
		EditAnchorSpiritValue(MAX_ANCHOR_SPIRIT);
	}
}

void AnchorSpirit::Initialize()
{
	m_spirit = 50.0f;
	m_auto_heal = false;
}

//今のソウルゲージの値をもらう
float	AnchorSpirit::GetAnchorSpiritValue()
{
	return m_spirit;
}




//今のソウルゲージの値をマイナス、もしくはプラス（マイナスしたい場合は負数を渡してね！）
void	AnchorSpirit::EditAnchorSpiritValue(float value)
{
	

	//プレイヤーの体力がMAXの時だけソウル値の更新できる
	if (PlayerStamina::GetPlayerStaminaValue() >= MAX_STAMINA)
	{
		m_spirit += value;

		if (m_spirit < 0.0f)
		{
			PlayerStamina::EditPlayerStaminaValue(m_spirit);
			m_spirit = 0.0f;
		}

		//ソウル値が最大値を超えた時の処理
		if (m_spirit > MAX_ANCHOR_SPIRIT)
		{
			m_spirit = MAX_ANCHOR_SPIRIT;
		}

	}
	//プレイヤーの体力がMAXじゃない時は更新処理をプレイヤーの体力でする
	else
	{
			PlayerStamina::EditPlayerStaminaValue(value);

	}//end_if(プレイヤーの体力がMAXじゃない)


	
}

void AnchorSpirit::SetAnchorSpiritValueDirectly(float value)
{
	m_spirit = value;
	if (m_spirit > 0)
	{
		PlayerStamina::SetPlayerStaminaValueDirectly(MAX_STAMINA);
	}
}


