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