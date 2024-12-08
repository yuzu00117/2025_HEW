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
void ObjectManager::AddWood(const b2Vec2& position, const b2Vec2& woodSize, const b2Vec2& anchorPointSize,const int& need_level) {
    // ������ 3 �����R���X�g���N�^�𗘗p���Đ���
    woodList.emplace_back(std::make_unique<wood>(position, woodSize, anchorPointSize,need_level));
}

void ObjectManager::AddRock(const b2Vec2& position, const float& radius, const int& need_anchor_level)
{
    rockList.emplace_back(std::make_unique<rock>(position, radius, need_anchor_level));
}

// �����ǉ�
void ObjectManager::AddOne_way_platformList(const b2Vec2& position, const b2Vec2& local_position,const b2Vec2 &size) {
    // ������ 3 �����R���X�g���N�^�𗘗p���Đ���
    one_way_platformList.emplace_back(std::make_unique<one_way_platform>(position, local_position, size));
}
// �X�΂�ǉ�
void ObjectManager::AddSloping_block(const b2Vec2& position, const b2Vec2& size, const SlopingBlockAspect& aspect) {
    // ������ 3 �����R���X�g���N�^�𗘗p���Đ���
    sloping_blockList.emplace_back(std::make_unique<sloping_block>(position, size, aspect));
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

//ID���g���Ė؂�����
one_way_platform* ObjectManager::Findone_way_platformByID(int id) {
    for (const auto& w : one_way_platformList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // ������Ȃ��ꍇ�� nullptr ��Ԃ�
}


sloping_block* ObjectManager::FindSloping_BlockByID(int id) {
    for (const auto& w : sloping_blockList) {
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

    for (auto& w : rockList) {
        w->Initialize();
    }


    for (auto& w : one_way_platformList) {
        w->Initialize();
    }

    for (auto& w : sloping_blockList) {
        w->Initialize();
    }
}

// �S�Ă̖؂��X�V
void ObjectManager::UpdateAll() {
    for (auto& w : woodList) {
        w->Update();
    }


    for (auto& w : rockList) {
        w->Update();
    }

    for (auto& w : one_way_platformList) {
        w->Update();
    }

    for (auto& w : sloping_blockList) {
        w->Update();
    }
}

// �S�Ă̖؂�`��
void ObjectManager::DrawAll() {
    for (auto& w : woodList) {
        w->Draw();
    }

    for (auto& w : rockList) {
        w->Draw();
    }

    for (auto& w : one_way_platformList) {
        w->Draw();
    }

    for (auto& w : sloping_blockList) {
        w->Draw();
    }

}

// �S�Ă̖؂�j��
void ObjectManager::FinalizeAll() {
    for (auto& w : woodList) {
        w->Finalize();
    }
    for (auto& w : rockList) {
        w->Finalize();
    }
    for (auto& w : one_way_platformList) {
        w->Finalize();
    }

    for (auto& w : sloping_blockList) {
        w->Finalize();
    }


    woodList.clear(); // ���I�z����N���A���ă��������
    rockList.clear();
    one_way_platformList.clear();
    sloping_blockList.clear();
}


