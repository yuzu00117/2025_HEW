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
float AnchorSpirit::m_damage = 0.0f;
float AnchorSpirit::m_increase = 0.0f;
bool AnchorSpirit::m_auto_heal = false;

void AnchorSpirit::Update()
{
	//自動回復
	if (m_auto_heal && m_spirit != MAX_ANCHOR_SPIRIT)
	{
		EditAnchorSpiritValue(1.0f);
	}
	//ダメージ分ゲージの数値を一気に減らす
	if (m_damage > 0)
	{
		m_damage -= 0.5f;
	}
	//増加はゲージの数値を徐々に増やす
	if (m_increase > 0)
	{
		m_increase -= 1.0f;
		m_spirit += 1.0f;
	}

	//ソウル値が最大値を超えた時の処理
	if (m_spirit > MAX_ANCHOR_SPIRIT)
	{
		m_spirit = MAX_ANCHOR_SPIRIT;
	}
	//ソウル値がマイナスになった時の処理
	if (m_spirit < 0)
	{
		// 余分のダメージは体力に行く
		PlayerStamina::EditPlayerStaminaValue(m_spirit);
		m_spirit = 0.0f;
	}
}

void AnchorSpirit::Initialize()
{
	m_spirit = 50.0f;
	m_damage = 0.0f;
	m_increase = 0.0f;
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
	//ダメージ（消費）処理
	if (value < 0)
	{
		m_damage += -value;
		m_spirit += value;
		//ソウル値がマイナスになった時の処理
		if (m_spirit < 0)
		{
			// 余分のダメージは体力に行く
			PlayerStamina::EditPlayerStaminaValue(m_spirit);
			m_spirit = 0.0f;
		}

	}
	//　増加処理
	else if (value > 0)
	{
		m_increase += value;
	}

	
}

void AnchorSpirit::SetAnchorSpiritValueDirectly(float value)
{
	//変わった差
	float change = value - m_spirit;
	
	//増加処理
	if (change > 0)
	{
		m_increase = change;

	}
	//ダメージ（消費）処理
	else
	{
		m_damage += -change;
		m_spirit = value;
		//ソウル値がマイナスになった時の処理
		if (m_spirit < 0)
		{
			// 余分のダメージは体力に行く
			PlayerStamina::EditPlayerStaminaValue(m_spirit);
			m_spirit = 0.0f;
		}

	}

}


