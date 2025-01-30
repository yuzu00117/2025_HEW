#ifndef PLAYER_UI_H
#define PLAYER_UI_H

#include <DirectXMath.h> // �K�v�ȃw�b�_�[���C���N���[�h
#include "Item_Jewel.h"

class player_UI {
public:
    player_UI();  // �R���X�g���N�^
    ~player_UI(); // �f�X�g���N�^

    // �v���C���[�̏�����
    static void Initialize();
    static void Update();
    static void Draw();
    static void Finalize();

    static DirectX::XMFLOAT2 GetRingPosition() { return m_ring_position; }

    static  void    SetJewelCollected(Jewel_Type type, bool flag) {
        switch (type)
        {
        case BLUE:
            m_blue_jewel_collected = flag;
            break;
        case RED:
            m_red_jewel_collected = flag;
            break;
        case YELLOW:
            m_yellow_jewel_collected = flag;
            break;
        }
    }


private:
    // �ÓI�����o�[�ϐ��̌^�𐳂����w��
    static DirectX::XMFLOAT2 player_ui_position;
    static DirectX::XMFLOAT2 player_ui_size;
    static float player_ui_alpha;

    static bool    m_blue_jewel_collected;
    static bool    m_red_jewel_collected;
    static bool    m_yellow_jewel_collected;

    static DirectX::XMFLOAT2 m_ring_position;
};

#endif // PLAYER_UI_H