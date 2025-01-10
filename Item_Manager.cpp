#include "Item_Manager.h"
#include "Item_SpeedUp.h"
#include "world_box2d.h"


// シングルトンのインスタンス取得
ItemManager& ItemManager::GetInstance() {
	static ItemManager instance;
	return instance;
}


void	ItemManager::AddSpeedUp(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon, float Alpha)
{
	// 既存の引数コンストラクタを利用して生成
	m_SpeedUp_List.emplace_back(std::make_unique<ItemSpeedUp>(position, body_size, angle, shape_polygon, Alpha));
    ID_SpeedUp_List++;
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


// 全てのアイテムを初期化
void ItemManager::InitializeAll() {
    for (auto& w : m_SpeedUp_List) {
        w->Initialize();
    }
}

// 全てのアイテムを更新
void ItemManager::UpdateAll() {
    for (auto& w : m_SpeedUp_List) {
        bool destory = w->Update();
        if (destory)
        {
            //ボディの情報を消す
            b2World* world = Box2dWorld::GetInstance().GetBox2dWorldPointer();
            world->DestroyBody(w->GetBody());
            //リストの中の情報を消す
            m_SpeedUp_List.erase(m_SpeedUp_List.begin() + ID_SpeedUp_List);
        }
    }
}

// 全てのアイテムを描画
void ItemManager::DrawAll() {
    for (auto& w : m_SpeedUp_List) {
        w->Draw();
    }
}

// 全てのアイテムを破棄
void ItemManager::FinalizeAll() {
    for (auto& w : m_SpeedUp_List) {
        w->Finalize();
    }
    m_SpeedUp_List.clear(); // 動的配列をクリアしてメモリ解放
}



