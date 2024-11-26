//-----------------------------------------------------------------------------------------------------
// #name player_position.cpp
// #description プレイヤーのポジションを取得して計算や描画に利用
// #make 2024/11/22　　永野義也
// #update 2024/11/22
// #comment 追加・修正予定
//          ・特になし
//----------------------------------------------------------------------------------------------------



#include"include/box2d/box2d.h"
#include"player_position.h"

// 静的メンバ変数の定義（1回だけ行う）
b2Vec2 PlayerPosition::player_position = b2Vec2(0.0f, 0.0f);

b2Vec2 PlayerPosition::GetPlayerPosition()
{
    return player_position;
}

void PlayerPosition::SetPlayerPosition(b2Vec2 player_pos)
{
    player_position = player_pos;
}