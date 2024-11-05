//-----------------------------------------------------------------------------------------------------
// #name player.cpp
// #description �v���C���[
// #make 2024/11/04
// #update 2024/11/03
// #comment �ǉ��E�C���\��
//          �E����AGame.cpp�Ȃ��ŏ������Ƃ������v���C���[��new�����Ă���
// 
//          
//----------------------------------------------------------------------------------------------------

#ifndef PLAYER_H

#define PLAYER_H



#include"square_collider.h"
#include"include/box2d/box2d.h"
#include"texture.h"


class Player : public SquareCollider
{
public:
    Player(XMFLOAT2 p, XMFLOAT2 s, float angle, bool bFixed);
    ~Player();

    void Init();
    void Update();
    void Draw();
    void Uninit();

private:
    int Health = 3;  // HP
    ID3D11ShaderResourceView* m_p_Texture = InitTexture(L"asset\\texture\\sample_texture\\img_sample_texture_bule.png");
};

#endif // !PLAYER_H