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
    static b2Vec2 player_position;  // êÈåæÇÃÇ›
};

#endif // !PLAYER_POSITION_H

