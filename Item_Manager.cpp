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
#include"Item_Coin.h"
#include"Item_Coin_UI.h"


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


void	ItemManager::AddCoin(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon, float Alpha)
{
	// �����̈����R���X�g���N�^�𗘗p���Đ���
	m_Coin_List.emplace_back(std::make_unique<ItemCoin>(position, body_size, angle, shape_polygon, Alpha));
}

void ItemManager::AddJewel(b2Vec2 position, b2Vec2 body_size, float angle, Jewel_Type type, bool shape_polygon, float Alpha)
{
    // �����̈����R���X�g���N�^�𗘗p���Đ���
    m_Jewel_List.emplace_back(std::make_unique<ItemJewel>(position, body_size, angle, type, shape_polygon, Alpha));
}

void ItemManager::AddSpirit(b2Vec2 position, b2Vec2 body_size, float angle, float recovery, float Alpha)
{
    // �����̈����R���X�g���N�^�𗘗p���Đ���
    m_Spirit_List.emplace_back(std::make_unique<ItemSpirit>(position, body_size, angle, recovery, Alpha));
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

ItemCoin* ItemManager::FindItem_Coin_ByID(int ID)
{
	for (const auto& w : m_Coin_List) {
		if (w->GetID() == ID) {
			return w.get();
		}
	}
	return nullptr; // ������Ȃ��ꍇ�� nullptr ��Ԃ�
}

ItemJewel* ItemManager::FindItem_Jewel_ByID(int ID)
{
    for (const auto& w : m_Jewel_List) {
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
	for (auto& w : m_Coin_List) {
		w->Initialize();
	}
	Item_Coin_UI::Initialize();
    for (auto& w : m_Jewel_List) {
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
    for (auto& w : m_Coin_List) {
        w->Update();
    }
    for (auto& w : m_Jewel_List) {
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
	for (auto& w : m_Coin_List) {
		w->Draw();
	}
	Item_Coin_UI::Draw();
    for (auto& w : m_Jewel_List) {
        w->Draw();
    }
}

// �S�ẴA�C�e����j��
void ItemManager::FinalizeAll() {
    for (auto& w : m_SpeedUp_List) {
        w->Finalize();
    }
	for (auto& w : m_Coin_List) {
		w->Finalize();
	}
	Item_Coin_UI::Finalize();

    for (auto& w : m_Spirit_List) {
        w->Finalize();
    }
    for (auto& w : m_Jewel_List) {
        w->Finalize();
    }

    m_SpeedUp_List.clear(); // ���I�z����N���A���ă��������
    m_Spirit_List.clear(); // ���I�z����N���A���ă��������
    m_Jewel_List.clear(); // ���I�z����N���A���ă��������
}

void ItemManager::SetCollectSpirit(bool flag)
{
    for (auto& w : m_Spirit_List) {
        w->SetState(Spirit_Collecting);
    }

	m_Coin_List.clear(); 
}

void ItemManager::UseAllJewel()
{
    for (auto& w : m_Jewel_List) {
        w->Function();
    }

    m_Jewel_List.clear();

}




