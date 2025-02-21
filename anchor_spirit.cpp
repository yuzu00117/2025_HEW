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
bool AnchorSpirit::m_auto_heal = false;

void AnchorSpirit::Update()
{
	//自動回復
	if (m_auto_heal && m_spirit != MAX_ANCHOR_SPIRIT)
	{
		EditAnchorSpiritValue(1.0f);
	}
}

void AnchorSpirit::Initialize()
{
	m_spirit = 50.0f;
	m_damage = 0.0f;
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
	//今のソウル値を保存しておく
	float prev_spirit = m_spirit;

		m_spirit += value;

		//ソウルゲージがマイナスになったら
		// 余分のダメージは体力に行く
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

	//受けたダメージの計算
	m_damage = prev_spirit - m_spirit;
	if (m_damage < 0) { m_damage = 0.0f; }
	
}

void AnchorSpirit::SetAnchorSpiritValueDirectly(float value)
{
	//今のソウル値を保存しておく
	float prev_spirit = m_spirit;

	m_spirit = value;
	//ソウル値が最大値を超えた時の処理
	if (m_spirit > MAX_ANCHOR_SPIRIT)
	{
		m_spirit = MAX_ANCHOR_SPIRIT;
	}
	//ソウル値がマイナスになった時の処理
	if (m_spirit < 0)
	{
		m_spirit = 0.0f;
	}

	//受けたダメージの計算
	m_damage = prev_spirit - m_spirit;
	if (m_damage < 0) { m_damage = 0.0f; }

}


