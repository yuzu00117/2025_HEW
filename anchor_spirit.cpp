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


float AnchorSpirit::m_spirit = 50.0f;

void AnchorSpirit::Initialize()
{
	m_spirit = 50.0f;
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
			m_spirit = 0.0f;
			//後でこの行にオブジェクトのレベルを取得する処理を追加
			//今はオブジェクトのレベルを暫定object_levelに
			int	object_level = 1;
			//プレイヤーの体力を消費する処理
			switch (object_level)
			{
			case 1:
				PlayerStamina::EditPlayerStaminaValue(STAMINA_DAMAGE_BY_OBJECT_LEVEL_1);
				break;
			case 2:
				PlayerStamina::EditPlayerStaminaValue(STAMINA_DAMAGE_BY_OBJECT_LEVEL_2);
				break;
			default:
				break;
			}
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
		//ここ仕様はちょっと後で梅に確認
		//もしプラスの処理の場合
		if (value > 0)
		{
			//プラスされる値をプレイヤーの体力に入れる
			PlayerStamina::EditPlayerStaminaValue(value);
		}
		else
		{
			//後でこの行にオブジェクトのレベルを取得する処理を追加
			//今はオブジェクトのレベルを暫定object_levelに
			int	object_level = 1;
			//プレイヤーの体力を消費する処理
			switch (object_level)
			{
			case 1:
				PlayerStamina::EditPlayerStaminaValue(STAMINA_DAMAGE_BY_OBJECT_LEVEL_1);
				break;
			case 2:
				PlayerStamina::EditPlayerStaminaValue(STAMINA_DAMAGE_BY_OBJECT_LEVEL_2);
				break;
			default:
				break;
			}


		}//end_if(value<0)



	}//end_if(プレイヤーの体力がMAXじゃない)


	
}

