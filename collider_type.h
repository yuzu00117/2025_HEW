//-----------------------------------------------------------------------------------------------------
// #name collider_type.h
// #description �R���C�_�[�̎�ނ�Z�b�g
// #make 2024/11/22�@�i��`��
// #update 2024/11/22
// #comment �ǉ��E�C���\��
//          �E�R���C�_�[�̎�ނ𑝂₵�����Ƃ��ɕύX���Ă�
//----------------------------------------------------------------------------------------------------


#ifndef COLLIDER_TYPE_H
#define COLLIDER_TYPE_H

#include <string>
#include"include/box2d/box2d.h"
#include"object_manager.h"
#include"Item_Manager.h"

enum ColliderTypeList//�t�B�N�X�`���̐ڐG����Ŏg���@�V���Ȃ����蔻��̎�ނ�ǉ��������ꍇ�����ɑ����ā@���ŃZ�b�g���Ă�
{
    collider_player_body,
    collider_player_leg,
    collider_player_sensor,

    collider_anchor,
    collider_anchor_chain,
    collider_anchor_point,

    collider_ground,



    collider_object,

    collider_item,

    collider_normal_attack_anchor,

    collider_enemy_dynamic,
    collider_enemy_static,



    collider_stage_select_point,
    collider_stage_select_player,

};


// �J�X�^���f�[�^�N���X�@�����ŃR���C�_�[�̃^�C�v��Z�b�g
class ObjectData {
public:
    ColliderTypeList collider_type; // �R���C�_�[�̃^�C�v��Z�b�g                     
    void* extra;                    // �ǉ���񂠂�Ȃ�Z�b�g

    // Constructor
    ObjectData(const ColliderTypeList type)
        : collider_type(type), extra(nullptr) {}


    b2Vec2 add_force = { 0.0f,0.0f };// �ǉ���b2vec�f�[�^
    ObjectType object_name;  // �ǉ��̕�����f�[�^(�I�u�W�F�N�g�p)
    ItemType Item_name; //  �ǉ��̕�����f�[�^�i�A�C�e���p�j
    int id;//�I�u�W�F�N�g��Ǘ����邽�߂�ID

    int need_anchor_level;

    // ID �𐶐�����֐�
    static int GenerateID() {
        return current_id++;//�S�Ă�ID��Ǘ�
    }

private:
    // �ÓI�J�E���^ ID
    static int current_id;




};




#endif // !COLLIDER_TYPE_H
