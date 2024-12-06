//-----------------------------------------------------------------------------------------------------
// #name object_manager.cpp
// #description オブジェクトの管理をするCPP　
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//          ・噂のファクトリーってやつかもねー
//          ・これのインスタンスの管理いいなー　がち綺麗
//          ・一か月前にファクトリー知りたかっためう
//----------------------------------------------------------------------------------------------------


#include"object_manager.h"



// シングルトンのインスタンス取得
ObjectManager& ObjectManager::GetInstance() {
    static ObjectManager instance;
    return instance;
}

// 木を追加
void ObjectManager::AddWood(const b2Vec2& position, const b2Vec2& woodSize, const b2Vec2& anchorPointSize,const bool& right) {
    // 既存の 3 引数コンストラクタを利用して生成
    woodList.emplace_back(std::make_unique<wood>(position, woodSize, anchorPointSize,right));
}

void ObjectManager::AddRock(const b2Vec2& position, const float& radius, const int& need_anchor_level)
{
    rockList.emplace_back(std::make_unique<rock>(position, radius, need_anchor_level));
}
// 足場を追加
void ObjectManager::AddOne_way_platformList(const b2Vec2& position, const b2Vec2& local_position,const b2Vec2 &size) {
    // 既存の 3 引数コンストラクタを利用して生成
    one_way_platformList.emplace_back(std::make_unique<one_way_platform>(position, local_position, size));
}
// 傾斜を追加
void ObjectManager::AddSloping_block(const b2Vec2& position, const b2Vec2& size, const SlopingBlockAspect& aspect) {
    // 既存の 3 引数コンストラクタを利用して生成
    sloping_blockList.emplace_back(std::make_unique<sloping_block>(position, size, aspect));
}


// ID を使って木を検索
wood* ObjectManager::FindWoodByID(int id) {
    for (const auto& w : woodList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}

// ID を使って岩を検索
rock* ObjectManager::FindRockByID(int id) {
    for (const auto& w : rockList) {

//IDを使って木を検索
one_way_platform* ObjectManager::Findone_way_platformByID(int id) {
    for (const auto& w : one_way_platformList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}

// 全てのオブジェクトを初期化

sloping_block* ObjectManager::FindSloping_BlockByID(int id) {
    for (const auto& w : sloping_blockList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}
      
// 全ての木を初期化
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
}

// 全てのオブジェクトを更新
void ObjectManager::UpdateAll() {
    for (auto& w : woodList) {
        w->Update();
    }

    for (auto& w : rockList) {

    for (auto& w : one_way_platformList) {
        w->Update();
    }

    for (auto& w : sloping_blockList) {
        w->Update();
    }
}

// 全てのオブジェクトを描画
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
}

// 全てのオブジェクトを破棄
void ObjectManager::FinalizeAll() {
    for (auto& w : woodList) {
        w->Finalize();
    }

    for (auto& w : rockList) {
        w->Finalize();
    }
    woodList.clear(); // 動的配列をクリアしてメモリ解放
    rockList.clear();

    for (auto& w : one_way_platformList) {
        w->Finalize();
    }

    for (auto& w : sloping_blockList) {
        w->Finalize();
    }


    woodList.clear(); // 動的配列をクリアしてメモリ解放
    one_way_platformList.clear();
    sloping_blockList.clear();
}


