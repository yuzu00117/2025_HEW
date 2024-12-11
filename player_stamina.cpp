#include "player_stamina.h"
#include "anchor_spirit.h"
#include "scene.h"



// 静的メンバ変数の定義（1回だけ行う）
float	PlayerStamina::m_stamina = 300.0f;


float	PlayerStamina::GetPlayerStaminaValue()
{
	return m_stamina;
}

void	PlayerStamina::EditPlayerStaminaValue(float value)
{
	m_stamina += value;

	if (m_stamina <= 0.0f)
	{
		//ここで残機を減らす処理
		



		//ここで画面遷移

		ChangeScene(SCENE_GAMEOVER);

	}

	//体力が最大体力を超えた時の処理
	if (m_stamina > MAX_STAMINA)
	{
		//余った分はソウルゲージをプラス
		AnchorSpirit::EditAnchorSpiritValue(m_stamina - MAX_STAMINA);
		m_stamina = MAX_STAMINA;
	}
}


