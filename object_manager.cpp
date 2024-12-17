//-----------------------------------------------------------------------------------------------------
// #name object_manager.cpp
// #description �I�u�W�F�N�g�̊Ǘ�������CPP�@
// #make 2024/12/04�@�i��`��
// #update 2024/12/13
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

//���ǉ�
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

//�ÓI�����I�u���b�N
void ObjectManager::AddStatic_to_Dynamic_block(const b2Vec2& position, const b2Vec2& size, const collider_type_Box_or_Circle& collider_type, const int& need_level) {
    // ������ 3 �����R���X�g���N�^�𗘗p���Đ���
    static_to_dynamic_blockList.emplace_back(std::make_unique<static_to_dynamic_block>(position,size,collider_type,need_level));
}

void ObjectManager::AddMovable_Ground(const b2Vec2& position, const b2Vec2& groundSize, const b2Vec2& anchorPointSize, const int& need_level){
    // ������ 3 �����R���X�g���N�^�𗘗p���Đ���
    movable_groundList.emplace_back(std::make_unique<movable_ground>(position, groundSize, anchorPointSize, need_level));
}

//�ÓI�G�l�~�[����
void ObjectManager::AddEnemyStatic(b2Vec2 position, b2Vec2 body_size, float angle)
{
    enemy_staticList.emplace_back(std::make_unique<EnemyStatic>(position, body_size, angle));
}
//���I�G�l�~�[����
void ObjectManager::AddEnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle)
{
    enemy_dynamicList.emplace_back(std::make_unique<EnemyDynamic>(position, body_size, angle));
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

//ID���g���ČX�΃u���b�N������
sloping_block* ObjectManager::FindSloping_BlockByID(int id) {
    for (const auto& w : sloping_blockList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // ������Ȃ��ꍇ�� nullptr ��Ԃ�
}

//ID���g���ĐÓI�����I�u���b�N������
static_to_dynamic_block* ObjectManager::FindStatic_to_Dynamic_BlcokID(int id) {
    for (const auto& w : static_to_dynamic_blockList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // ������Ȃ��ꍇ�� nullptr ��Ԃ�
}

movable_ground* ObjectManager::FindMovable_GroundID(int id)
{
    for (const auto& w : movable_groundList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr;
}

//ID���g���ĐÓI�G�l�~�[������
EnemyStatic* ObjectManager::FindEnemyStaticByID(int id)
{
    for (auto& w : enemy_staticList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // ������Ȃ��ꍇ�� nullptr ��Ԃ�
}
//ID���g���ē��I�G�l�~�[������
EnemyDynamic* ObjectManager::FindEnemyDynamicByID(int id)
{
    for (auto& w : enemy_dynamicList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // ������Ȃ��ꍇ�� nullptr ��Ԃ�
}

//�w��̐ÓI�G�l�~�[���폜
void ObjectManager::DestroyEnemyStatic(int id)
{
    int cnt = 0;
    for (auto& w : enemy_staticList) {
        if (w->GetID() == id) {
            enemy_staticList.erase(enemy_staticList.begin() + cnt);
            break;
        }
        ++cnt;
    }
}
//�w��̓��I�G�l�~�[���폜
void ObjectManager::DestroyEnemyDynamic(int id)
{
    int cnt = 0;
    for (auto& w : enemy_dynamicList) {
        if (w->GetID() == id) {
            enemy_dynamicList.erase(enemy_dynamicList.begin() + cnt);
            break;
        }
        ++cnt;
    }
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

    for (auto& w : static_to_dynamic_blockList) {
        w->Initialize();
    }

    for (auto& w : enemy_staticList) {
        w->Initialize();
    }

    for (auto& w : enemy_dynamicList) {
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

    for (auto& w : static_to_dynamic_blockList) {
        w->Update();
    }

    for (auto& w : movable_groundList) {
        w->Update();
    }


    for (auto& w : enemy_staticList) {
        if(w)
        {
            w->Update();
        }
    }

    for (auto& w : enemy_dynamicList) {
        if(w)
        {
            w->Update();
        }
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

    for (auto& w : static_to_dynamic_blockList) {
        w->Draw();
    }

    for (auto& w : movable_groundList) {
        w->Draw();
    }

    for (auto& w : enemy_staticList) {
        w->Draw();
    }

    for (auto& w : enemy_dynamicList) {
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

    for (auto& w : static_to_dynamic_blockList) {
        w->Finalize();
    }

    for (auto& w : movable_groundList) {
        w->Finalize();
    }

    for (auto& w : enemy_staticList) {
        w->Finalize();
    }


    woodList.clear(); // ���I�z����N���A���ă��������
    rockList.clear();
    one_way_platformList.clear();
    sloping_blockList.clear();
    static_to_dynamic_blockList.clear();
    movable_groundList.clear();
    enemy_staticList.clear();
    enemy_dynamicList.clear();

}


