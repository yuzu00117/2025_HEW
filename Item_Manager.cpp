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




void	ItemManager::AddCoin(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon, float Alpha)
{
	// 既存の引数コンストラクタを利用して生成
	m_Coin_List.emplace_back(std::make_unique<ItemCoin>(position, body_size, angle, shape_polygon, Alpha));
}

void ItemManager::AddJewel(b2Vec2 position, b2Vec2 body_size, float angle, Jewel_Type type, bool shape_polygon, float Alpha)
{
    // 既存の引数コンストラクタを利用して生成
    m_Jewel_List.emplace_back(std::make_unique<ItemJewel>(position, body_size, angle, type, shape_polygon, Alpha));
}

void ItemManager::AddSavePoint(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon, float Alpha)
{
    // 既存の引数コンストラクタを利用して生成
    m_SavePoint_List.emplace_back(std::make_unique<ItemSavePoint>(position, body_size, angle, shape_polygon, Alpha));
}

void ItemManager::AddSpirit(b2Vec2 position, b2Vec2 body_size, float angle, SpiritType type, float Alpha)
{
    // 既存の引数コンストラクタを利用して生成
    m_Spirit_List.emplace_back(std::make_unique<ItemSpirit>(position, body_size, angle, type, Alpha));
   //　新しく作ったものの初期化処理
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


// 全てのアイテムを初期化
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
}

// 全てのアイテムを描画
void ItemManager::DrawAll() {
	for (auto& w : m_Coin_List) {
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
}


// 全てのアイテムを破棄
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
    m_Spirit_List.clear(); // 動的配列をクリアしてメモリ解放
    m_Jewel_List.clear(); // 動的配列をクリアしてメモリ解放
    m_SavePoint_List.clear(); // 動的配列をクリアしてメモリ解放
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

    //宝石全部回収されているなら
    if (count == 3)
    {
        AnchorSpirit::SetIfAutoHeal(true);
    }

    m_Jewel_List.remove_if([](const auto& p) {return p->GetBody() == nullptr; });

}




