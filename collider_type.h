#ifndef COLLIDER_TYPE_H
#define COLLIDER_TYPE_H



enum ColliderTypeList
{
    collider_player,

    collider_player_sensor,

    collider_anchor,

    collider_ground,
    collider_wall,

    collider_anchor_point,

    collider_enemy_dynamic,
    collider_enemy_static,
};


// �J�X�^���f�[�^�N���X
class ObjectData {
public:
    ColliderTypeList collider_type; // �R���C�_�[�̃^�C�v���Z�b�g                     
    void* extra;                    // �ǉ���񂠂�Ȃ�Z�b�g

    // Constructor
    ObjectData(const ColliderTypeList type)
        : collider_type(type), extra(nullptr) {}
};




#endif // !COLLIDER_TYPE_H
