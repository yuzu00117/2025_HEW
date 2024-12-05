//-----------------------------------------------------------------------------------------------------
// #name object_manager.cpp
// #description �I�u�W�F�N�g�̊Ǘ�������CPP�@
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�\�̃t�@�N�g���[���Ă�����ˁ[
//          �E����̃C���X�^���X�̊Ǘ������ȁ[�@�����Y��
//          �E�ꂩ���O�Ƀt�@�N�g���[�m�肽�������߂�
//----------------------------------------------------------------------------------------------------


#include"object_manager.h"



// �V���O���g���̃C���X�^���X�擾
ObjectManager& ObjectManager::GetInstance() {
    static ObjectManager instance;
    return instance;
}

// �؂�ǉ�
void ObjectManager::AddWood(const b2Vec2& position, const b2Vec2& woodSize, const b2Vec2& anchorPointSize,const bool& right) {
    // ������ 3 �����R���X�g���N�^�𗘗p���Đ���
    woodList.emplace_back(std::make_unique<wood>(position, woodSize, anchorPointSize,right));
}


// ID ���g���Ė؂�����
wood* ObjectManager::FindWoodByID(int id) {
    for (const auto& w : woodList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // ������Ȃ��ꍇ�� nullptr ��Ԃ�
}

// �S�Ă̖؂�������
void ObjectManager::InitializeAll() {
    for (auto& w : woodList) {
        w->Initialize();
    }
}

// �S�Ă̖؂��X�V
void ObjectManager::UpdateAll() {
    for (auto& w : woodList) {
        w->Update();
    }
}

// �S�Ă̖؂�`��
void ObjectManager::DrawAll() {
    for (auto& w : woodList) {
        w->Draw();
    }
}

// �S�Ă̖؂�j��
void ObjectManager::FinalizeAll() {
    for (auto& w : woodList) {
        w->Finalize();
    }
    woodList.clear(); // ���I�z����N���A���ă��������
}


