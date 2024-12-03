//-----------------------------------------------------------------------------------------------------
// #name player_position.h
// #description プレイヤーのポジションを取得して計算や描画に利用
// #make 2024/11/22　　永野義也
// #update 2024/11/22
// #comment 追加・修正予定
//          ・特になし
//----------------------------------------------------------------------------------------------------

#ifndef PLAYER_POSITION_H
#define PLAYER_POSITION_H

#include "tool.h"
#include"include/box2d/box2d.h"

class PlayerPosition
{
public:
    static b2Vec2 GetPlayerPosition();
    static void SetPlayerPosition(b2Vec2 player_pos);

private:
    static b2Vec2 player_position;  // 宣言のみ
};

#endif // !PLAYER_POSITION_H

