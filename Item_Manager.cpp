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
#include "Item_SpeedUp.h"
#include "world_box2d.h"
#include"Item_coin.h"
#include"Item_Coin_UI.h"


// シングルトンのインスタンス取得
ItemManager& ItemManager::GetInstance() {
	static ItemManager instance;
	return instance;
}


void	ItemManager::AddSpeedUp(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon, float Alpha)
{
	// 既存の引数コンストラクタを利用して生成
	m_SpeedUp_List.emplace_back(std::make_unique<ItemSpeedUp>(position, body_size, angle, shape_polygon, Alpha));
}
void	ItemManager::AddCoin(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon, float Alpha)
{
	
	m_Coin_List.emplace_back(std::make_unique<ItemCoin>(position, body_size, angle, shape_polygon, Alpha));
}

void ItemManager::AddSpirit(b2Vec2 position, b2Vec2 body_size, float angle, float recovery, float Alpha)
{
    // 既存の引数コンストラクタを利用して生成
    m_Spirit_List.emplace_back(std::make_unique<ItemSpirit>(position, body_size, angle, recovery, Alpha));
   //　新しく作ったものの初期化処理
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
	return nullptr; // 見つからない場合は nullptr を返す
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
}

// 全てのアイテムを更新
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
}

// 全てのアイテムを描画
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
}

// 全てのアイテムを破棄
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
    m_SpeedUp_List.clear(); // 動的配列をクリアしてメモリ解放
    m_Spirit_List.clear(); // 動的配列をクリアしてメモリ解放
}

void ItemManager::SetCollectSpirit(bool flag)
{
    for (auto& w : m_Spirit_List) {
        w->SetState(Spirit_Collecting);
    }

	m_Coin_List.clear(); 
}




