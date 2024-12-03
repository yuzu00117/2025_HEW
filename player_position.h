//-----------------------------------------------------------------------------------------------------
// #name player_position.h
// #description �v���C���[�̃|�W�V�������擾���Čv�Z��`��ɗ��p
// #make 2024/11/22�@�@�i��`��
// #update 2024/11/22
// #comment �ǉ��E�C���\��
//          �E���ɂȂ�
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
    static b2Vec2 player_position;  // �錾�̂�
};

#endif // !PLAYER_POSITION_H

