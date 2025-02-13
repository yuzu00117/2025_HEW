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
#include "world_box2d.h"
#include"Item_Coin.h"
#include"Item_Coin_UI.h"
#include"anchor_spirit.h"


// �V���O���g���̃C���X�^���X�擾
ItemManager& ItemManager::GetInstance() {
	static ItemManager instance;
	return instance;
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

void ItemManager::AddSavePoint(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon, float Alpha)
{
    // �����̈����R���X�g���N�^�𗘗p���Đ���
    m_SavePoint_List.emplace_back(std::make_unique<ItemSavePoint>(position, body_size, angle, shape_polygon, Alpha));
}

void ItemManager::AddSpirit(b2Vec2 position, b2Vec2 body_size, float angle, SpiritType type, float Alpha)
{
    // �����̈����R���X�g���N�^�𗘗p���Đ���
    m_Spirit_List.emplace_back(std::make_unique<ItemSpirit>(position, body_size, angle, type, Alpha));
   //�@�V������������̂̏���������
    auto& lastSpirit = *m_Spirit_List.back();
    lastSpirit.Initialize();
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

ItemSavePoint* ItemManager::FindItem_SavePoint_ByID(int ID)
{
    for (const auto& w : m_SavePoint_List) {
        if (w->GetID() == ID) {
            return w.get();
        }
    }
    return nullptr;
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
    for (auto& w : m_Spirit_List) {
        w->Initialize();
    }
	for (auto& w : m_Coin_List) {
		w->Initialize();
	}
    for (auto& w : m_SavePoint_List) {
        w->Initialize();
    }
	Item_Coin_UI::Initialize();
    for (auto& w : m_Jewel_List) {
        w->Initialize();
    }
}

// �S�ẴA�C�e�����X�V
void ItemManager::UpdateAll() {
    for (auto& w : m_Spirit_List) {
        w->Update();
    }
    for (auto& w : m_Coin_List) {
        w->Update();
    }
    for (auto& w : m_Jewel_List) {
        w->Update();
    }
 
    for (auto& w : m_SavePoint_List) {
        w->Update();
    }
}

// �S�ẴA�C�e����`��
void ItemManager::DrawAll() {
	for (auto& w : m_Coin_List) {
		w->Draw();
	}
	
}

// �S�ẴA�C�e����`��
void ItemManager::DrawFront() {
 
    //�����O��ɕ`��
    for (auto& w : m_Spirit_List) {
        w->Draw();
    }

    for (auto& w : m_Jewel_List) {
        w->Draw();
    }

    for (auto& w : m_SavePoint_List) {
        w->Draw();
    }

    for (auto& w : m_Coin_List) {
        w->DrawEffect();
    }
    Item_Coin_UI::Draw();
}


// �S�ẴA�C�e����j��
void ItemManager::FinalizeAll() {
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

    for (auto& w : m_SavePoint_List) {
        w->Finalize();
    }
    m_Spirit_List.clear(); // ���I�z����N���A���ă��������
    m_Jewel_List.clear(); // ���I�z����N���A���ă��������
    m_SavePoint_List.clear(); // ���I�z����N���A���ă��������
}


void ItemManager::UseAllJewel()
{
    int count = 0;
    for (auto& w : m_Jewel_List) {
        if (w->GetBody() == nullptr)
        {
            w->Function();
            count++;
        }
    }

    //��ΑS���������Ă���Ȃ�
    if (count == 3)
    {
        AnchorSpirit::SetIfAutoHeal(true);
    }

    m_Jewel_List.remove_if([](const auto& p) {return p->GetBody() == nullptr; });

}




