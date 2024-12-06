//-----------------------------------------------------------------------------------------------------
// #name enemy.h
// #description 動的、静的エネミーの継承元、エネミークラスのcppファイル
// #make 2024/11/19
// #update 2024/11/29
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