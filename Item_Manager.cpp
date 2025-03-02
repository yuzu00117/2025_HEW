//-----------------------------------------------------------------------------------------------------
// #name Item_Manager.cpp
// #description アイテムの管理(ファクトリー)
// #make 2024/12/28　王泳心
// #update 2024/12/28
// #comment 追加・修正予定
//      
//
// 
//----------------------------------------------------------------------------------------------------

#include "Item_Manager.h"
#include "world_box2d.h"
#include"Item_Coin.h"
#include"Item_Coin_UI.h"
#include"anchor_spirit.h"


// シングルトンのインスタンス取得
ItemManager& ItemManager::GetInstance() {
	static ItemManager instance;
	return instance;
}




void	ItemManager::AddCoin(b2Vec2 position, b2Vec2 body_size, float angle, bool respawning, bool shape_polygon, float Alpha)
{
    if (respawning) {
        return;
    }
	// 既存の引数コンストラクタを利用して生成
	m_Coin_List.emplace_back(std::make_unique<ItemCoin>(position, body_size, angle, shape_polygon, Alpha));
}

void ItemManager::AddJewel(b2Vec2 position, b2Vec2 body_size, float angle, Jewel_Type type, bool respawning, bool shape_polygon, float Alpha)
{
    if (respawning) {
        return;
    }
    // 既存の引数コンストラクタを利用して生成
    m_Jewel_List.emplace_back(std::make_unique<ItemJewel>(position, body_size, angle, type, shape_polygon, Alpha));
}

void ItemManager::AddSavePoint(b2Vec2 position, b2Vec2 body_size, float angle, bool respawning, bool shape_polygon, float Alpha)
{
    if (respawning) {
        return;
    }
    // 既存の引数コンストラクタを利用して生成
    m_SavePoint_List.emplace_back(std::make_unique<ItemSavePoint>(position, body_size, angle, shape_polygon, Alpha));
}

void ItemManager::AddSpirit(b2Vec2 position, b2Vec2 body_size, float angle, SpiritType type, bool respawning, float Alpha)
{
    if (respawning) {
        return;
    }
    // 既存の引数コンストラクタを利用して生成
    m_Spirit_List.emplace_back(std::make_unique<ItemSpirit>(position, body_size, angle, type, Alpha));
   //　新しく作ったものの初期化処理
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
	return nullptr; // 見つからない場合は nullptr を返す
}

ItemJewel* ItemManager::FindItem_Jewel_ByID(int ID)
{
    for (const auto& w : m_Jewel_List) {
        if (w->GetID() == ID) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
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

// 全てのアイテムを初期化
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

// 全てのアイテムを更新
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

// 全てのアイテムを描画
void ItemManager::DrawAll() {
	for (auto& w : m_Coin_List) {
		w->Draw();
	}
    for (const auto& w : m_Healing_List) {
        w->Draw();
    }
}

// 全てのアイテムを描画
void ItemManager::DrawFront() {
 
    //魂も前列に描画
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


// 全てのアイテムを破棄
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
    m_Spirit_List.clear(); // 動的配列をクリアしてメモリ解放
    m_Jewel_List.clear(); // 動的配列をクリアしてメモリ解放
    m_SavePoint_List.clear(); // 動的配列をクリアしてメモリ解放
    m_Healing_List.clear();
    m_Barrier_List.clear();
}

//リスポン時の終了処理
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
   m_Spirit_List.clear(); // 動的配列をクリアしてメモリ解放
   m_Healing_List.clear();  //動的配列をクリアしてメモリ解放
   m_Barrier_List.clear(); //動的配列をクリアしてメモリ解放
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
    m_Spirit_List.clear(); // 動的配列をクリアしてメモリ解放
    m_SavePoint_List.clear(); // 動的配列をクリアしてメモリ解放
    m_Healing_List.clear(); //動的配列をクリアしてメモリ解放
    m_Barrier_List.clear(); //動的配列をクリアしてメモリ解放
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
    m_Spirit_List.clear(); // 動的配列をクリアしてメモリ解放
    m_Healing_List.clear();  //動的配列をクリアしてメモリ解放
    m_Barrier_List.clear(); //動的配列をクリアしてメモリ解放

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

    //宝石全部回収されているなら
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

    //特別措置がいるアイテムだけ下みたいな処理してる、普通のアイテムは上のSpirit_Listみたいにやれば十分
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

    //特別措置がいるアイテムだけ下みたいな処理してる、普通のアイテムは上のSpirit_Listみたいにやれば十分
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






