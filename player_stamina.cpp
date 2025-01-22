//-----------------------------------------------------------------------------------------------------
// #name player_stamina.cpp
// #description プレイヤーの体力値
// #make 2024/11/20			王泳心
// #update 2024/11/20
// #comment 追加・修正予定
//           ・残機と画面遷移追加されたら、体力0になった時の処理を追加
//           ・2025/01/21 yuzu00117 体力が0になったときにリザルト画面に遷移する処理を追加
// 
//----------------------------------------------------------------------------------------------------


#include	"player_stamina.h"
#include	"anchor_spirit.h"
#include	"scene.h"


// 静的メンバ変数の定義（1回だけ行う）
float	PlayerStamina::m_stamina = 300.0f;
bool PlayerStamina::m_isdead = false; // プレイヤーの死亡フラグ


void PlayerStamina::Initialize()
{
	m_stamina = 300.0f;
	m_isdead = false; // プレイヤーの死亡フラグを初期化
}

float	PlayerStamina::GetPlayerStaminaValue()
{
	return m_stamina;
}

void	PlayerStamina::EditPlayerStaminaValue(float value)
{

	float anchor_spirit = AnchorSpirit::GetAnchorSpiritValue();
	if (value < 0 && anchor_spirit > 0)
	{
		AnchorSpirit::EditAnchorSpiritValue(value);
		return;
	}

	//スタミナを更新
	m_stamina += value;

	if (m_stamina <= 0.0f)
	{
		//ここで残機を減らす処理

		//画面遷移のためにプレイヤーの死亡フラグを立てる
		m_isdead = true;
	}

	//体力が最大体力を超えた時の処理
	else if (m_stamina > MAX_STAMINA)
	{
		//余った分はソウルゲージをプラス
		AnchorSpirit::EditAnchorSpiritValue(m_stamina - MAX_STAMINA);
		m_stamina = MAX_STAMINA;
	}
}

// プレイヤーの死亡フラグを返す
bool PlayerStamina::IsPlayerDead()
{
	return m_isdead;
}
