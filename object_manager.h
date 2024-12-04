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

// �I�u�W�F�N�g�̎�ނ��`
enum ObjectType {
    Object_Wood, // ��
    Object_Rock, // ��
};

// �I�u�W�F�N�g���Ǘ�����N���X
class ObjectManager {
public:
    // �V���O���g���̃C���X�^���X�擾
    static ObjectManager& GetInstance();

    // �؂�ǉ�
    void AddWood(const b2Vec2& position, const b2Vec2& woodSize, const b2Vec2& anchorPointSize,const bool&right);

    // ID ���g���Ė؂�����
    wood* FindWoodByID(int id);

    // �S�ẴI�u�W�F�N�g��������
    void InitializeAll();

    // �S�ẴI�u�W�F�N�g���X�V
    void UpdateAll();

    // �S�ẴI�u�W�F�N�g��`��
    void DrawAll();

    // �S�ẴI�u�W�F�N�g��j��
    void FinalizeAll();

private:
    std::vector<std::unique_ptr<wood>> woodList; // �؂̃��X�g

    //�����ɃI�u�W�F�N�g���ƂɃ��X�g��ǉ����Ă����������˂��[


    ObjectManager() = default;
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;
};

#endif // OBJECT_MANAGER_H
