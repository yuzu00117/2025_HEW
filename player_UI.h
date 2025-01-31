#ifndef PLAYER_UI_H
#define PLAYER_UI_H

#include <DirectXMath.h> // �K�v�ȃw�b�_�[���C���N���[�h

class player_UI {
public:
    player_UI();  // �R���X�g���N�^
    ~player_UI(); // �f�X�g���N�^

    // �v���C���[�̏�����
    static void Initialize();
    static void Update();
    static void Draw();
    static void Finalize();

private:
    // �ÓI�����o�[�ϐ��̌^�𐳂����w��
    static DirectX::XMFLOAT2 player_ui_position;
    static DirectX::XMFLOAT2 player_ui_size;

    static DirectX::XMFLOAT2 gauge_only_position;
    static DirectX::XMFLOAT2 gauge_only_size;
    static float player_ui_alpha;
};

#endif // PLAYER_UI_H