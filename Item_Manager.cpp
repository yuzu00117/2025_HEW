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




void	ItemManager::AddCoin(b2Vec2 position, b2Vec2 body_size, float angle, bool respawning, bool shape_polygon, float Alpha)
{
    if (respawning) {
        return;
    }
	// �����̈����R���X�g���N�^�𗘗p���Đ���
	m_Coin_List.emplace_back(std::make_unique<ItemCoin>(position, body_size, angle, shape_polygon, Alpha));
}

void ItemManager::AddJewel(b2Vec2 position, b2Vec2 body_size, float angle, Jewel_Type type, bool respawning, bool shape_polygon, float Alpha)
{
    if (respawning) {
        return;
    }
    // �����̈����R���X�g���N�^�𗘗p���Đ���
    m_Jewel_List.emplace_back(std::make_unique<ItemJewel>(position, body_size, angle, type, shape_polygon, Alpha));
}

void ItemManager::AddSavePoint(b2Vec2 position, b2Vec2 body_size, float angle, bool respawning, bool shape_polygon, float Alpha)
{
    if (respawning) {
        return;
    }
    // �����̈����R���X�g���N�^�𗘗p���Đ���
    m_SavePoint_List.emplace_back(std::make_unique<ItemSavePoint>(position, body_size, angle, shape_polygon, Alpha));
}

void ItemManager::AddSpirit(b2Vec2 position, b2Vec2 body_size, float angle, SpiritType type, bool respawning, float Alpha)
{
    if (respawning) {
        return;
    }
    // �����̈����R���X�g���N�^�𗘗p���Đ���
    m_Spirit_List.emplace_back(std::make_unique<ItemSpirit>(position, body_size, angle, type, Alpha));
   //�@�V������������̂̏���������
    auto& lastSpirit = *m_Spirit_List.back();
    lastSpirit.Initialize();
}

void ItemManager::AddHealing(b2Vec2 position, b2Vec2 body_size, float angle, bool respawning)
{
    if (respawning)
    {
        return;
    }
    m_Healing_List.emplace_back(std::make_unique<ItemHealing>(position, body_size, angle));
}

void ItemManager::AddBarrier(b2Vec2 position, b2Vec2 body_size, float angle, const b2Body* owner_body, float Alpha)
{
    m_Barrier_List.emplace_back(std::make_unique<ItemBarrier>(position, body_size, angle, owner_body));
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

ItemSavePoint* ItemManager::FindItem_SavePoint(int ID)
{
    for (const auto& w : m_SavePoint_List) {
        if (w->GetID() == ID)
        {
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

ItemHealing* ItemManager::FindItem_Healing(int id)
{
    for (const auto& w : m_Healing_List) {
        if (w->GetID() == id){
            return w.get();
        }
    }
    return nullptr;
}

ItemBarrier* ItemManager::FindItem_Barrier_ByID(int id)
{
    for (const auto& w : m_Barrier_List) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr;
}

ItemBarrier* ItemManager::FindItem_Barrier_ByOwnerBody(const b2Body* owner)
{
    for (const auto& w : m_Barrier_List) {
        if (w->GetOwnerBody() == owner) {
            return w.get();
        }
    }
    return nullptr;
}

// �S�ẴA�C�e����������
void ItemManager::InitializeAll() 
{
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
    for (const auto& w : m_Healing_List) {
        w->Initialize();
    }
    for (const auto& w : m_Barrier_List) {
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
    for (const auto& w : m_Healing_List) {
        w->Update();
    }
    for (auto& w : m_Barrier_List) {
        w->Update();
    }
    m_Barrier_List.remove_if([](const auto& p) {return p->GetBody() == nullptr; });
    
}

// �S�ẴA�C�e����`��
void ItemManager::DrawAll() {
	for (auto& w : m_Coin_List) {
		w->Draw();
	}
    for (const auto& w : m_Healing_List) {
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

    for (auto& w : m_Barrier_List) {
        w->Draw();
    }
}


// �S�ẴA�C�e����j��
void ItemManager::FinalizeAll() 
{
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

    for (const auto& w : m_Healing_List) {
        w->Finalize();
    }
    for (const auto& w : m_Barrier_List) {
        w->Finalize();
    }
    m_Spirit_List.clear(); // ���I�z����N���A���ă��������
    m_Jewel_List.clear(); // ���I�z����N���A���ă��������
    m_SavePoint_List.clear(); // ���I�z����N���A���ă��������
    m_Healing_List.clear();
    m_Barrier_List.clear();
}

//���X�|�����̏I������
void ItemManager::Finalize_WhenRespawn()
{
   for (auto& w : m_Spirit_List) {
       w->Finalize();
   }
   for (auto& w : m_Jewel_List) {
       w->Finalize();
   }
   for (auto& w : m_Healing_List) {
       w->Finalize();
   }
   for (auto& w : m_Barrier_List) {
       w->Finalize();
   }
   for (auto& w : m_Coin_List) {
       w->Finalize();
   }
   m_Spirit_List.clear(); // ���I�z����N���A���ă��������
   m_Healing_List.clear();  //���I�z����N���A���ă��������
   m_Barrier_List.clear(); //���I�z����N���A���ă��������
}

void ItemManager::Finalize_WhenNextStage()
{
    for (auto& w : m_Coin_List) {
        w->Finalize();
    }

    for (auto& w : m_Spirit_List) {
        w->Finalize();
    }
    for (auto& w : m_SavePoint_List) {
        w->Finalize();
    }
    for (auto& w : m_Jewel_List) {
        w->Finalize();
    }
    for (const auto& w : m_Healing_List) {
        w->Finalize();
    }
    for (auto& w : m_Barrier_List) {
        w->Finalize();
    }
    m_Spirit_List.clear(); // ���I�z����N���A���ă��������
    m_SavePoint_List.clear(); // ���I�z����N���A���ă��������
    m_Healing_List.clear(); //���I�z����N���A���ă��������
    m_Barrier_List.clear(); //���I�z����N���A���ă��������
}

void ItemManager::Finalize_WhenRespawn_SavePoint_GamePause()
{
    for (auto& w : m_Spirit_List) {
        w->Finalize();
    }
    for (auto& w : m_Jewel_List) {
        w->Finalize();
    }
    for (auto& w : m_Healing_List) {
        w->Finalize();
    }
    for (auto& w : m_Barrier_List) {
        w->Finalize();
    }
    for (auto& w : m_Coin_List) {
        w->Finalize();
    }
    m_Spirit_List.clear(); // ���I�z����N���A���ă��������
    m_Healing_List.clear();  //���I�z����N���A���ă��������
    m_Barrier_List.clear(); //���I�z����N���A���ă��������

}

void ItemManager::Finalize_WhenRespawn_Initial_GamePause()
{
}


void ItemManager::UseAllJewel()
{
    int count = 0;
    for (auto& w : m_Jewel_List) {
        if (w->SearchIfJewelHaveGotByPlayer() == true && w->GetIfFunctioned() == false)
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

    //m_Jewel_List.remove_if([](const auto& p) {return p->GetBody() == nullptr; });

}

void ItemManager::Initialize_WhenRespawn()
{
    for (auto& w : m_Spirit_List) {
        w->Initialize();
    }
    for (const auto& w : m_Healing_List) {
        w->Initialize();
    }
    for (auto& w : m_Barrier_List) {
        w->Initialize();
    }

    //���ʑ[�u������A�C�e���������݂����ȏ������Ă�A���ʂ̃A�C�e���͏��Spirit_List�݂����ɂ��Ώ\��
    //----------------------------------------------------

    for (auto& w : m_Coin_List) {
        if (w->GetDestory() != true)
        {
            w->Initialize();
            w->CreateBody();
        }
    }

    for (auto& w : m_Jewel_List) {
        if (w->GetDestory() != true)
        {
            w->Initialize();
            w->CreateBody();
        }
    }

    for (auto& w : m_SavePoint_List) {
        w->CreateBody();
    }

}

void ItemManager::Initialize_WhenNextStage()
{
    for (auto& w : m_Spirit_List) {
        w->Initialize();
    }
    for (auto& w : m_Coin_List) {
        w->Initialize();
    }
    for (auto& w : m_SavePoint_List) {
        w->Initialize();
    }
    for (auto& w : m_Jewel_List) {
        w->Initialize();
    }
    for (auto& w : m_Healing_List) {
        w->Initialize();
    }
    for (auto& w : m_Barrier_List) {
        w->Initialize();
    }
}

void ItemManager::Initialize_WhenRespawn_SavePoint_GamePause()
{
    for (auto& w : m_Spirit_List) {
        w->Initialize();
    }
    for (const auto& w : m_Healing_List) {
        w->Initialize();
    }
    for (auto& w : m_Barrier_List) {
        w->Initialize();
    }

    //���ʑ[�u������A�C�e���������݂����ȏ������Ă�A���ʂ̃A�C�e���͏��Spirit_List�݂����ɂ��Ώ\��
    //----------------------------------------------------

    for (auto& w : m_Coin_List) {
        if (w->GetIfRegisteredToSavePoint() == false) {
            w->Initialize();
            w->CreateBody();
        }
    }
    
    for (auto& w : m_Jewel_List) {
        w->Initialize();
        if (w->GetIfRegisteredToSavePoint() == false) {
            w->Initialize();
            w->CreateBody();
        }
    }

    for (auto& w : m_SavePoint_List) {
        w->CreateBody();
    }
}

void ItemManager::Initialize_WhenRespawn_Initial_GamePause()
{
}

void ItemManager::SetJewelRegistered_ToSavePoint()
{
    for (auto& w : m_Jewel_List)
    {
        if(w->GetBody() == nullptr)
        {
            w->SetIfRegisteredToSavePoint(true);
        }

    }
}

void ItemManager::SetCoinRegistered_ToSavePoint()
{
    for (auto& w : m_Coin_List)
    {
        if (w->GetBody() == nullptr)
        {
            w->SetIfRegisteredToSavePoint(true);
        }

    }
}






