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

void ObjectManager::AddRock(const b2Vec2& position, const float& radius, const int& need_anchor_level)
{
    rockList.emplace_back(std::make_unique<rock>(position, radius, need_anchor_level));
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


// ID ���g���Ċ������
rock* ObjectManager::FindRockByID(int id) {
    for (const auto& w : rockList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // ������Ȃ��ꍇ�� nullptr ��Ԃ�
}

// �S�ẴI�u�W�F�N�g��������
void ObjectManager::InitializeAll() {
    for (auto& w : woodList) {
        w->Initialize();
    }
    for (auto& w : rockList) {
        w->Initialize();
    }
    
}

// �S�ẴI�u�W�F�N�g���X�V
void ObjectManager::UpdateAll() {
    for (auto& w : woodList) {
        w->Update();
    }
    for (auto& w : rockList) {
        w->Update();
    }
}

// �S�ẴI�u�W�F�N�g��`��
void ObjectManager::DrawAll() {
    for (auto& w : woodList) {
        w->Draw();
    }
    for (auto& w : rockList) {
        w->Draw();
    }
}

// �S�ẴI�u�W�F�N�g��j��
void ObjectManager::FinalizeAll() {
    for (auto& w : woodList) {
        w->Finalize();
    }

    for (auto& w : rockList) {
        w->Finalize();
    }
    woodList.clear(); // ���I�z����N���A���ă��������
    rockList.clear();
}


