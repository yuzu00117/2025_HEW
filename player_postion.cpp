#include"include/box2d/box2d.h"
#include"player_position.h"

// 静的メンバ変数の定義（1回だけ行う）
b2Vec2 PlayerPostion::player_postion = b2Vec2(0.0f, 0.0f);

b2Vec2 PlayerPostion::GetPlayerPostion()
{
    return player_postion;
}

void PlayerPostion::SetPlayerPostion(b2Vec2 player_pos)
{
    player_postion = player_pos;
}