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

// �I�u�W�F�N�g�̎�ނ��`
enum ObjectType {
    NULL_object,
    Object_Wood, // ��
    Object_Rock, // ��
    Object_one_way_platform//����@�������炵�����Ȃ�
    
};


// �I�u�W�F�N�g���Ǘ�����N���X
class ObjectManager {
public:
    // �V���O���g���̃C���X�^���X�擾
    static ObjectManager& GetInstance();

    // �؂�ǉ�
    void AddWood(const b2Vec2& position, const b2Vec2& woodSize, const b2Vec2& anchorPointSize,const bool&right);

    void AddOne_way_platformList(const b2Vec2& position, const b2Vec2& local_position, const b2Vec2 &size);

    void AddSloping_block(const b2Vec2& position, const b2Vec2& size, const SlopingBlockAspect& aspect);

    // ID ���g���Ė؂�����
    wood* FindWoodByID(int id);

    one_way_platform* Findone_way_platformByID(int id);

    sloping_block* FindSloping_BlockByID(int id);

    // �S�ẴI�u�W�F�N�g��������
    void InitializeAll();

    // �S�ẴI�u�W�F�N�g���X�V
    void UpdateAll();

    // �S�ẴI�u�W�F�N�g��`��
    void DrawAll();

    // �S�ẴI�u�W�F�N�g��j��
    void FinalizeAll();

private:
    std::vector<std::unique_ptr<wood>> woodList;
    std::vector<std::unique_ptr<one_way_platform>> one_way_platformList;// ����̃��X�g
    std::vector<std::unique_ptr<sloping_block>> sloping_blockList;
    

    //�����ɃI�u�W�F�N�g���ƂɃ��X�g��ǉ����Ă����������˂��[


    ObjectManager() = default;
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;
};

#endif // OBJECT_MANAGER_H
