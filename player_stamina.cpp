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
#include	"sound.h"
#include	"Item_Manager.h"
#include	"player.h"


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

void PlayerStamina::SetPlayerStaminaValueDirectly(float value)
{
	//体力値が最大値を超えた時の処理
	m_stamina = value;
	if (m_stamina > MAX_STAMINA) {

		m_stamina = MAX_STAMINA;
	}
	if (m_stamina <= 0)
	{
		m_stamina = 0.0f;
		//ここで残機を減らす処理
		app_atomex_start(Player_Dead_Sound);

		//画面遷移のためにプレイヤーの死亡フラグを立てる
		m_isdead = true;
	}
}


void	PlayerStamina::EditPlayerStaminaValue(float value)
{
	//アイテムによるダメージ回避
	if (value < 0)
	{
		Player& player = Player::GetInstance();
		ItemManager& item_manager = ItemManager::GetInstance();
		ItemBarrier* barrier = item_manager.FindItem_Barrier_ByOwnerBody(player.GetOutSidePlayerBody());

		if (barrier)
		{
			barrier->SetState(Barrier_Break);
			return;
		}
	}

	//スタミナを更新
	m_stamina += value;

	if (m_stamina <= 0.0f)
	{
		//ここで残機を減らす処理
		app_atomex_start(Player_Dead_Sound);

		//画面遷移のためにプレイヤーの死亡フラグを立てる
		m_isdead = true;
	}

	//体力が最大体力を超えた時の処理
	else if (m_stamina > MAX_STAMINA)
	{
		m_stamina = MAX_STAMINA;
	}
}

// プレイヤーの死亡フラグを返す
bool PlayerStamina::IsPlayerDead()
{
	return m_isdead;
}
