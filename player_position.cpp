//-----------------------------------------------------------------------------------------------------
// #name player_position.cpp
// #description �v���C���[�̃|�W�V�������擾���Čv�Z��`��ɗ��p
// #make 2024/11/22�@�@�i��`��
// #update 2024/11/22
// #comment �ǉ��E�C���\��
//          �E���ɂȂ�
//----------------------------------------------------------------------------------------------------



#include"include/box2d/box2d.h"
#include"player_position.h"

// �ÓI�����o�ϐ��̒�`�i1�񂾂��s���j
b2Vec2 PlayerPosition::player_position = b2Vec2(0.0f, 0.0f);

b2Vec2 PlayerPosition::GetPlayerPosition()
{
    return player_position;
}

void PlayerPosition::SetPlayerPosition(b2Vec2 player_pos)
{
    player_position = player_pos;
}