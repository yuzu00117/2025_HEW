//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description 動的、静的エネミーの継承元、エネミークラスのcppファイル
// #make 2024/11/19
// #update 2024/12/13
// #comment 追加・修正予定
//          ・プレイヤーとの衝突判定を追加予定
//			・倒れてきたオブジェクトとの衝突判定を追加予定
//			・画面内にいないとUpdateしない処理を追加予定
//----------------------------------------------------------------------------------------------------

#include"include/box2d/box2d.h"
#include"enemy.h"
#include"world_box2d.h"
#include"texture.h"
#include"Xinput_controller.h"
#include"sprite.h"
#include"keyboard.h"
#include<Windows.h>
#include"player_stamina.h"
#include"anchor_spirit.h"

//エネミーがプレイヤーに触れた時の処理
void Enemy::CollisionPlayer()
{
	PlayerStamina::EditPlayerStaminaValue(-GetDamage());
	SetUse(false);
}

//エネミーが動いている状態のオブジェクトに触れた時の処理
void Enemy::CollisionPulledObject()
{
	AnchorSpirit::EditAnchorSpiritValue(GetSoulgage());
	SetUse(false);
}

//エネミーがセンサー内に入った時の処理
void Enemy::InPlayerSensor()
{
	SetInScreen(true);
}

//エネミーがセンサー外に出た時の処理
void Enemy::OutPlayerSensor()
{
	//センサー内で呼び出されることがあるので
	//if(GetBody()->GetPosition().x)
	//SetInScreen(false);
}