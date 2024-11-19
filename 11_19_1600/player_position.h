#ifndef PLAYER_POSITION_H
#define PLAYER_POSITION_H

#include "tool.h"
#include"include/box2d/box2d.h"

class PlayerPostion
{
public:
    static b2Vec2 GetPlayerPostion();
    static void SetPlayerPostion(b2Vec2 player_pos);

private:
    static b2Vec2 player_postion;  // êÈåæÇÃÇ›
};

#endif // !PLAYER_POSITION_H

