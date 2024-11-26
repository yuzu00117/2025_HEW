//-----------------------------------------------------------------------------------------------------
// #name collider_type.h
// #description �R���C�_�[�̎�ނ��Z�b�g
// #make 2024/11/22�@�i��`��
// #update 2024/11/22
// #comment �ǉ��E�C���\��
//          �E�R���C�_�[�̎�ނ𑝂₵�����Ƃ��ɕύX���Ă�
//----------------------------------------------------------------------------------------------------


#ifndef COLLIDER_TYPE_H
#define COLLIDER_TYPE_H



enum ColliderTypeList//�t�B�N�X�`���̐ڐG����Ŏg���@�V���Ȃ����蔻��̎�ނ�ǉ��������ꍇ�����ɑ����ā@���ŃZ�b�g���Ă�
{
    collider_player,

    collider_player_sensor,

    collider_anchor,
    collider_anchor_chain,

    collider_ground,
    collider_wall,

    collider_anchor_point,

};


// �J�X�^���f�[�^�N���X�@�����ŃR���C�_�[�̃^�C�v���Z�b�g
class ObjectData {
public:
    ColliderTypeList collider_type; // �R���C�_�[�̃^�C�v���Z�b�g                     
    void* extra;                    // �ǉ���񂠂�Ȃ�Z�b�g

    // Constructor
    ObjectData(const ColliderTypeList type)
        : collider_type(type), extra(nullptr) {}
};




#endif // !COLLIDER_TYPE_H
