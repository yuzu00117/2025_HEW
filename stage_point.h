//-----------------------------------------------------------------------------------------------------
// #name stage_point.h
// #description �X�e�[�W�Z���N�g������ۂɎg�p����A�X�e�[�W�̃|�C���g�̂����蔻��̃{�f�B������Ă�
// #make 2024/12/27�@�@�i��`��
// #update 2024/12/27
// #comment �ǉ��E�C���\��
//                      �E���ɂȂ�
//           
//----------------------------------------------------------------------------------------------------
#ifndef STAGE_POINT_H
#define STAGE_POINT_H

#include "include/box2d/box2d.h"
#include"sprite.h"

class StagePoint {
public:
    StagePoint();
    ~StagePoint();

    // ������
    void Initialize(b2World* world, float x, float y, float size,int id);

    // �X�V�i�K�v�Ȃ�j
    void Update();

    // �`��
    void Draw();

    // �Փ˃`�F�b�N�p
    bool IsPlayerColliding(b2Body* playerBody);

    // �I������
    void Finalize();

    int GetID()const
    {
        return id;
    }

private:
    int id;
    b2Body* m_body; // �X�e�[�W�|�C���g��Box2D�{�f�B
    float m_size;   // �X�e�[�W�|�C���g�̃T�C�Y
    b2World* m_world; // �Q�Ɨp
};

#endif // !STAGE_POINT_H
