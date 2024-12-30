//-----------------------------------------------------------------------------------------------------
// #name Item_Manager.cpp
// #description �A�C�e���̊Ǘ�(�t�@�N�g���[)
// #make 2024/12/28�@���j�S
// #update 2024/12/28
// #comment �ǉ��E�C���\��
//      
//
// 
//----------------------------------------------------------------------------------------------------

#include "Item_Manager.h"
#include "Item_SpeedUp.h"
#include "world_box2d.h"


// �V���O���g���̃C���X�^���X�擾
ItemManager& ItemManager::GetInstance() {
	static ItemManager instance;
	return instance;
}


void	ItemManager::AddSpeedUp(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon, float Alpha)
{
	// �����̈����R���X�g���N�^�𗘗p���Đ���
	m_SpeedUp_List.emplace_back(std::make_unique<ItemSpeedUp>(position, body_size, angle, shape_polygon, Alpha));
}

void ItemManager::AddSpirit(b2Vec2 position, b2Vec2 body_size, float angle, float recovery, bool shape_polygon, float Alpha)
{
    // �����̈����R���X�g���N�^�𗘗p���Đ���
    m_Spirit_List.emplace_back(std::make_unique<ItemSpirit>(position, body_size, angle, recovery, shape_polygon, Alpha));
   //�@�V������������̂̏���������
    auto& lastSpirit = *m_Spirit_List.back();
    lastSpirit.Initialize();
}


ItemSpeedUp* ItemManager::FindItem_SpeedUp_ByID(int ID)
{
	for (const auto& w : m_SpeedUp_List) {
		if (w->GetID() == ID) {
			return w.get();
		}
	}
	return nullptr; // ������Ȃ��ꍇ�� nullptr ��Ԃ�
}

ItemSpirit* ItemManager::FindItem_Spirit_ByID(int ID)
{
    for (const auto& w : m_Spirit_List) {
        if (w->GetID() == ID) {
            return w.get();
        }
    }
    return nullptr;
}


// �S�ẴA�C�e����������
void ItemManager::InitializeAll() {
    for (auto& w : m_SpeedUp_List) {
        w->Initialize();
    }
    for (auto& w : m_Spirit_List) {
        w->Initialize();
    }
}

// �S�ẴA�C�e�����X�V
void ItemManager::UpdateAll() {
    for (auto& w : m_SpeedUp_List) {
		w->Update();
    }
    for (auto& w : m_Spirit_List) {
        w->Update();
    }
}

// �S�ẴA�C�e����`��
void ItemManager::DrawAll() {
    for (auto& w : m_SpeedUp_List) {
        w->Draw();
    }
    for (auto& w : m_Spirit_List) {
        w->Draw();
    }
}

// �S�ẴA�C�e����j��
void ItemManager::FinalizeAll() {
    for (auto& w : m_SpeedUp_List) {
        w->Finalize();
    }
    for (auto& w : m_Spirit_List) {
        w->Finalize();
    }
    m_SpeedUp_List.clear(); // ���I�z����N���A���ă��������
    m_Spirit_List.clear(); // ���I�z����N���A���ă��������
}

void ItemManager::SetCollectSpirit(bool flag)
{
    for (auto& w : m_Spirit_List) {
        w->SetIfCollecting(flag);
    }

}




