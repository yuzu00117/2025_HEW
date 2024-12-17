//-----------------------------------------------------------------------------------------------------
// #name object_manager.cpp
// #description オブジェクトの管理をするCPP　
// #make 2024/12/04　永野義也
// #update 2024/12/13
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
void ObjectManager::AddWood(const b2Vec2& position, const b2Vec2& woodSize, const b2Vec2& anchorPointSize,const int& need_level) {
    // 既存の 3 引数コンストラクタを利用して生成
    woodList.emplace_back(std::make_unique<wood>(position, woodSize, anchorPointSize,need_level));
}

//岩を追加
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

//静的→動的ブロック
void ObjectManager::AddStatic_to_Dynamic_block(const b2Vec2& position, const b2Vec2& size, const collider_type_Box_or_Circle& collider_type, const int& need_level) {
    // 既存の 3 引数コンストラクタを利用して生成
    static_to_dynamic_blockList.emplace_back(std::make_unique<static_to_dynamic_block>(position,size,collider_type,need_level));
}

void ObjectManager::AddMovable_Ground(const b2Vec2& position, const b2Vec2& groundSize, const b2Vec2& anchorPointSize, const int& need_level){
    // 既存の 3 引数コンストラクタを利用して生成
    movable_groundList.emplace_back(std::make_unique<movable_ground>(position, groundSize, anchorPointSize, need_level));
}

//静的エネミー生成
void ObjectManager::AddEnemyStatic(b2Vec2 position, b2Vec2 body_size, float angle)
{
    enemy_staticList.emplace_back(std::make_unique<EnemyStatic>(position, body_size, angle));
}
//動的エネミー生成
void ObjectManager::AddEnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle)
{
    enemy_dynamicList.emplace_back(std::make_unique<EnemyDynamic>(position, body_size, angle));
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
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}

//IDを使って木を検索
one_way_platform* ObjectManager::Findone_way_platformByID(int id) {
    for (const auto& w : one_way_platformList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}

//IDを使って傾斜ブロックを検索
sloping_block* ObjectManager::FindSloping_BlockByID(int id) {
    for (const auto& w : sloping_blockList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}

//IDを使って静的→動的ブロックを検索
static_to_dynamic_block* ObjectManager::FindStatic_to_Dynamic_BlcokID(int id) {
    for (const auto& w : static_to_dynamic_blockList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}

movable_ground* ObjectManager::FindMovable_GroundID(int id)
{
    for (const auto& w : movable_groundList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr;
}

//IDを使って静的エネミーを検索
EnemyStatic* ObjectManager::FindEnemyStaticByID(int id)
{
    for (auto& w : enemy_staticList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}
//IDを使って動的エネミーを検索
EnemyDynamic* ObjectManager::FindEnemyDynamicByID(int id)
{
    for (auto& w : enemy_dynamicList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}

//指定の静的エネミーを削除
void ObjectManager::DestroyEnemyStatic(int id)
{
    int cnt = 0;
    for (auto& w : enemy_staticList) {
        if (w->GetID() == id) {
            enemy_staticList.erase(enemy_staticList.begin() + cnt);
            break;
        }
        ++cnt;
    }
}
//指定の動的エネミーを削除
void ObjectManager::DestroyEnemyDynamic(int id)
{
    int cnt = 0;
    for (auto& w : enemy_dynamicList) {
        if (w->GetID() == id) {
            enemy_dynamicList.erase(enemy_dynamicList.begin() + cnt);
            break;
        }
        ++cnt;
    }
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

    for (auto& w : static_to_dynamic_blockList) {
        w->Initialize();
    }

    for (auto& w : enemy_staticList) {
        w->Initialize();
    }

    for (auto& w : enemy_dynamicList) {
        w->Initialize();
    }
}

// 全ての木を更新
void ObjectManager::UpdateAll() {
    for (auto& w : woodList) {
        w->Update();
    }


    for (auto& w : rockList) {
        w->Update();
    }

    for (auto& w : one_way_platformList) {
        w->Update();
    }

    for (auto& w : sloping_blockList) {
        w->Update();
    }

    for (auto& w : static_to_dynamic_blockList) {
        w->Update();
    }

    for (auto& w : movable_groundList) {
        w->Update();
    }


    for (auto& w : enemy_staticList) {
        if(w)
        {
            w->Update();
        }
    }

    for (auto& w : enemy_dynamicList) {
        if(w)
        {
            w->Update();
        }
    }
}

// 全ての木を描画
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

    for (auto& w : static_to_dynamic_blockList) {
        w->Draw();
    }

    for (auto& w : movable_groundList) {
        w->Draw();
    }

    for (auto& w : enemy_staticList) {
        w->Draw();
    }

    for (auto& w : enemy_dynamicList) {
        w->Draw();
    }
}

// 全ての木を破棄
void ObjectManager::FinalizeAll() {
    for (auto& w : woodList) {
        w->Finalize();
    }
    for (auto& w : rockList) {
        w->Finalize();
    }
    for (auto& w : one_way_platformList) {
        w->Finalize();
    }

    for (auto& w : sloping_blockList) {
        w->Finalize();
    }

    for (auto& w : static_to_dynamic_blockList) {
        w->Finalize();
    }

    for (auto& w : movable_groundList) {
        w->Finalize();
    }

    for (auto& w : enemy_staticList) {
        w->Finalize();
    }


    woodList.clear(); // 動的配列をクリアしてメモリ解放
    rockList.clear();
    one_way_platformList.clear();
    sloping_blockList.clear();
    static_to_dynamic_blockList.clear();
    movable_groundList.clear();
    enemy_staticList.clear();
    enemy_dynamicList.clear();

}


