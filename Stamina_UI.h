//-----------------------------------------------------------------------------------------------------
// #name Stamina_UI.h
// #description �̗͂�UI
// #make 2025/2/24�@���j�S
// #update 2025/2/24
// #comment �ǉ��E�C���\��
//      
//
// 
//----------------------------------------------------------------------------------------------------
#ifndef STAMINA_H
#define STAMINA_H

#include <DirectXMath.h> // �K�v�ȃw�b�_�[���C���N���[�h

class Stamina_UI
{
public:
    Stamina_UI();  // �R���X�g���N�^
    ~Stamina_UI(); // �f�X�g���N�^

    // �v���C���[�̏�����
    static void Initialize();
    static void Update();
    static void Draw();
    static void Finalize();


private:
    // �ÓI�����o�[�ϐ��̌^�𐳂����w��
    static DirectX::XMFLOAT2 player_icon_position;
    static DirectX::XMFLOAT2 player_icon_size;

    static DirectX::XMFLOAT2 heart_position[3];
    static DirectX::XMFLOAT2 heart_size[3];

};


#endif // !STAMINA_H

