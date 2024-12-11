//-----------------------------------------------------------------------------------------------------
// #name object_manager
// #description �I�u�W�F�N�g���Ǘ����邽�߂̃t�@�N�g���[�̃C���[�W�ɋ߂�
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�I�u�W�F�N�g����邲�Ƃɐ������銴��
//----------------------------------------------------------------------------------------------------


#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <vector>
#include <memory>
#include "wood.h"
#include"one-way_platform.h"
#include"sloping_block.h"
#include"rock.h"
#include"static_to_dynamic_block.h"

// �I�u�W�F�N�g�̎�ނ��`
enum ObjectType {
    NULL_object,
    Object_Wood, // ��
    Object_Rock, // ��
    Object_one_way_platform,//����@�������炵�����Ȃ�
    Object_Static_to_Dynamic//�ÓI���瓮�I�ɕύX����I�u�W�F�N�g
    

};


// �I�u�W�F�N�g���Ǘ�����N���X
class ObjectManager {
public:
    // �V���O���g���̃C���X�^���X�擾
    static ObjectManager& GetInstance();

    // �؂�ǉ�
    void AddWood(const b2Vec2& position, const b2Vec2& woodSize, const b2Vec2& anchorPointSize,const int&need_level);

    //���ǉ�
    void AddRock(const b2Vec2& position, const float& radius, const int& need_anchor_level);
    //�����ǉ�
    void AddOne_way_platformList(const b2Vec2& position, const b2Vec2& local_position, const b2Vec2 &size);
    //�X�΃u���b�N�̒ǉ�
    void AddSloping_block(const b2Vec2& position, const b2Vec2& size, const SlopingBlockAspect& aspect);
    //�ÓI�����I�̃u���b�N�̒ǉ�
    void AddStatic_to_Dynamic_block(const b2Vec2& position, const b2Vec2& size, const collider_type_Box_or_Circle& collider_type, const int& need_level);

    // ID ���g���Ė؂�����
    wood* FindWoodByID(int id);
    //ID���g���ā@�������
    rock* FindRockByID(int id);
    //ID���g���đ���u���b�N������
    one_way_platform* Findone_way_platformByID(int id);
    //ID���g���ĎΖʃu���b�N������
    sloping_block* FindSloping_BlockByID(int id);
    //ID���g���ĐÓI�����I�u���b�N��ǉ�
    static_to_dynamic_block* FindStatic_to_Dynamic_BlcokID(int id);


    // �S�ẴI�u�W�F�N�g��������
    void InitializeAll();

    // �S�ẴI�u�W�F�N�g���X�V
    void UpdateAll();

    // �S�ẴI�u�W�F�N�g��`��
    void DrawAll();

    // �S�ẴI�u�W�F�N�g��j��
    void FinalizeAll();

private:
    std::vector<std::unique_ptr<wood>> woodList;//�؂̃��X�g
    std::vector < std::unique_ptr<rock>>rockList;//��̃��X�g
    std::vector<std::unique_ptr<one_way_platform>> one_way_platformList;// ����̃��X�g
    std::vector<std::unique_ptr<sloping_block>> sloping_blockList;//�Ζʂ̃��X�g
    std::vector<std::unique_ptr<static_to_dynamic_block>> static_to_dynamic_blockList;//�ÓI�����I�u���b�N����
    

    //�����ɃI�u�W�F�N�g���ƂɃ��X�g��ǉ����Ă����������˂��[


    ObjectManager() = default;
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;
};

#endif // OBJECT_MANAGER_H
