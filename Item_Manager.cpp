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


ItemSpeedUp* ItemManager::FindItem_SpeedUp_ByID(int ID)
{
	for (const auto& w : m_SpeedUp_List) {
		if (w->GetID() == ID) {
			return w.get();
		}
	}
	return nullptr; // ������Ȃ��ꍇ�� nullptr ��Ԃ�
}


// �S�ẴA�C�e����������
void ItemManager::InitializeAll() {
    for (auto& w : m_SpeedUp_List) {
        w->Initialize();
    }
}

// �S�ẴA�C�e�����X�V
void ItemManager::UpdateAll() {
    for (auto& w : m_SpeedUp_List) {
		w->Update();
    }
}

// �S�ẴA�C�e����`��
void ItemManager::DrawAll() {
    for (auto& w : m_SpeedUp_List) {
        w->Draw();
    }
}

// �S�ẴA�C�e����j��
void ItemManager::FinalizeAll() {
    for (auto& w : m_SpeedUp_List) {
        w->Finalize();
    }
    m_SpeedUp_List.clear(); // ���I�z����N���A���ă��������
}




