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
//エネミーの攻撃の生成
void ObjectManager::AddEnemyAttack(b2Vec2 position, b2Vec2 body_size, float angle, int id)
{
    enemy_attackList.emplace_back(std::make_unique<EnemyAttack>(position, body_size, angle, id));
}

//浮遊エネミー生成
void ObjectManager::AddEnemyFloating(b2Vec2 position, b2Vec2 body_size, float angle)
{
    enemy_floatingList.emplace_back(std::make_unique<EnemyFloating>(position, body_size, angle));
}

//間欠泉の生成
void ObjectManager::AddGeyser(b2Vec2 GeyserPosition, b2Vec2 GeyserSize, b2Vec2 RangeFlyWaterSize, int splitting_x, int splitting_y, Boss_Room_Level level)
{
    geyserList.emplace_back(std::make_unique<geyser>(GeyserPosition, GeyserSize, RangeFlyWaterSize, splitting_x, splitting_y, level));
}

//テレポートブロックの生成
void ObjectManager::AddTeleportBlock(b2Vec2 position, b2Vec2 body_size, b2Vec2 to_teleport_point)
{
    teleport_blockList.emplace_back(std::make_unique<teleport_block>(position, body_size, to_teleport_point));

}
//ボスの部屋の床を追加
void ObjectManager::AddBossFieldBlock(b2Vec2 position, b2Vec2 body_size, int block_hp, Boss_Room_Level level)
{
    boss_field_blockList.emplace_back(std::make_unique<boss_field_block>(position, body_size, block_hp, level));
}

//ボスの部屋の柱を追加
void ObjectManager::AddBossPillar(b2Vec2 position, b2Vec2 size, int splitting_x, int splitting_y, Boss_Room_Level level)
{
    boss_pillarList.emplace_back(std::make_unique<boss_pillar>(position, size, splitting_x, splitting_y,level));
}

void ObjectManager::AddBossCarryEnemySpawner(b2Vec2 position, b2Vec2 Size, Boss_Room_Level level, bool left)
{
    boss_carry_object_spawnerList.emplace_back(std::make_unique<boss_carry_object_spawner>(position, Size, level, left));
}

void ObjectManager::AddBossCarryObjectEnemy(b2Vec2 position, b2Vec2 enemy_size, bool left, float enemy_speed, b2Vec2 object_size, int object_type, int anchor_level)
{
    boss_carry_object_enemyList.emplace_back(std::make_unique<boss_carry_object_enemy>(position, enemy_size, left, enemy_speed, object_size, object_type, anchor_level));
}


void ObjectManager::AddChangeEnemyFilterAndBody(b2Vec2 position, b2Vec2 size, b2Vec2 velocity, ID3D11ShaderResourceView* Texture, float speed, float angle)
{
    change_filter_boidy_enemy_list.emplace_back(std::make_unique<change_enemy_filter_and_body>(position, size, velocity, Texture, speed, angle));
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
//IDを使ってエネミーの攻撃を検索
EnemyAttack* ObjectManager::FindEnemyAttackByID(int id)
{
    for (auto& w : enemy_attackList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}

//IDを使って浮遊エネミーを検索
EnemyFloating* ObjectManager::FindEnemyFloatingByID(int id)
{
    for (auto& w : enemy_floatingList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}

//IDを使って使って間欠泉を検索
geyser* ObjectManager::FindGeyserID(int id)
{
    for (auto& w : geyserList) {

        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}



//IDを使って使ってテレポートブロックを検索
teleport_block* ObjectManager::FindTeleportBlock(int id)
{
    for (auto& w : teleport_blockList) {

        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}
//IDを使ってボスの部屋の床を検索
boss_field_block* ObjectManager::FindBossFieldBlock(int id)
{
    for (auto& w : boss_field_blockList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}

//IDを使ってボスの部屋の柱を検索
boss_pillar* ObjectManager::FindBossPillar(int id)
{
    for (auto& w : boss_pillarList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}


//IDを使ってボスのオブジェクトエネミーのスポナー
boss_carry_object_spawner* ObjectManager::FindBossCarryEnemySpawner(int id)
{
    for (auto& w : boss_carry_object_spawnerList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}

//IDを使ってボスのオブジェクトエネミー
boss_carry_object_enemy* ObjectManager::FindBossCarryObjectEnemy(int id)
{
    for (auto& w : boss_carry_object_enemyList) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}


//IDを使って撃墜演出用の
change_enemy_filter_and_body* ObjectManager::FindChangeEnemyFilterAndBody(int id)
{
    for (auto& w : change_filter_boidy_enemy_list) {
        if (w->GetID() == id) {
            return w.get();
        }
    }
    return nullptr; // 見つからない場合は nullptr を返す
}










Object* ObjectManager::FindObjectByID_ObjectType(int id, ObjectType type)
{
    switch (type)
    {
    case Object_Wood: // 木
        break;
    case Object_Rock: // 岩
        break;
    case Object_one_way_platform://足場　したからしか乗れない
        break;
    case Object_Static_to_Dynamic://静的から動的に変更するオブジェクト
        break;
    case Object_Movable_Ground:  //引っ張れる床 
        break;
    case Object_Enemy_Static://静的エネミー
        break;
    case Object_Enemy_Dynamic://動的エネミー
        break;
    default:
        break;
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
//指定のエネミーの攻撃を削除
void ObjectManager::DestroyEnemyAttack(int id)
{
    int cnt = 0;
    for (auto& w : enemy_attackList) {
        if (w->GetID() == id) {
            enemy_attackList.erase(enemy_attackList.begin() + cnt);
            break;
        }
        ++cnt;
    }
}

void ObjectManager::DestroyEnemyFloating(int id)
{
    int cnt = 0;
    for (auto& w : enemy_floatingList) {
        if (w->GetID() == id) {
            enemy_floatingList.erase(enemy_floatingList.begin() + cnt);
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

    for (auto& w : movable_groundList) {
        w->Initialize();
    }

    for (auto& w : enemy_staticList) {
        w->Initialize();
    }

    for (auto& w : enemy_dynamicList) {
        w->Initialize();
    }

    for (auto& w : enemy_attackList) {
        w->Initialize();
    }

    for (auto& w : enemy_floatingList) {
        w->Initialize();
    }

    for (auto& w : teleport_blockList) {
        w->Initialize();
    }

    for (auto& w : geyserList) {
        w->Initialize();
    }


    for (auto& w : boss_field_blockList) {
        w->Initialize();
    }

    for (auto& w : boss_pillarList) {
        w->Initialize();
    }

    for (auto& w : boss_carry_object_enemyList) {
        w->Initialize();
    }

    for (auto& w : boss_carry_object_spawnerList) {
        w->Initialize();
    }


    for (auto& w : change_filter_boidy_enemy_list) {
        w->Initialize();
    }
    Item_Coin_UI::Initialize();
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

    for (auto& w : enemy_attackList) {
        if (w)
        {
            w->Update();
        }
    }
  
    for (auto& w : enemy_floatingList) {
        if (w)
        {
            w->Update();
        }
    }

    for (auto& w : teleport_blockList) {

        w->Update();
    }

    for (auto& w : geyserList) {
        w->Update();
    }



    for (auto& w : boss_field_blockList) {

        w->Update();
    }

    for (auto& w : boss_pillarList) {
        w->Update();
    }

    for (auto& w : boss_carry_object_enemyList) {
        w->Update();
    }

    for (auto& w : boss_carry_object_spawnerList) {
        w->Update();
    }

    for (auto& w : change_filter_boidy_enemy_list) {
        w->Update();
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


    for (auto& w : enemy_attackList) {
        w->Draw();
    }

    for (auto& w : enemy_floatingList) {
        w->Draw();
    }


    for (auto& w : geyserList) {
        w->Draw();
    }

  
    for (auto& w : teleport_blockList) {


        w->Draw();
    }


    for (auto& w : boss_field_blockList) 
    {

        w->Draw();
    }

    for (auto& w : boss_pillarList) {
        w->Draw();
    }
    

    for (auto& w : boss_carry_object_enemyList) {
        w->Draw();
    }

    for (auto& w : boss_carry_object_spawnerList) {
        w->Draw();
    }

    for (auto& w : change_filter_boidy_enemy_list) {
        w->Draw();
    }
    Item_Coin_UI::Draw();
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

    for (auto& w : enemy_dynamicList) {
        w->Finalize();
    }

    for (auto& w : enemy_attackList) {
        w->Finalize();
    }

    for (auto& w : teleport_blockList) {
        w->Finalize();
    }

    for (auto& w : geyserList) {
        w->Finalize();
    }

    for (auto& w : boss_field_blockList) {
        w->Finalize();
    }

    for (auto& w : enemy_floatingList) {
        w->Finalize();
    }

    for (auto& w : boss_pillarList) {
        w->Finalize();
    }

    for (auto& w : boss_carry_object_enemyList) {
        w->Finalize();
    }

    for (auto& w : boss_carry_object_spawnerList) {
        w->Finalize();
    }

    for (auto& w : change_filter_boidy_enemy_list) {
        w->Finalize();
    }

    Item_Coin_UI::Finalize();



    woodList.clear(); // 動的配列をクリアしてメモリ解放
    rockList.clear();
    one_way_platformList.clear();
    sloping_blockList.clear();
    static_to_dynamic_blockList.clear();
    movable_groundList.clear();
    enemy_staticList.clear();
    enemy_dynamicList.clear();
    enemy_attackList.clear();
  
    enemy_floatingList.clear();

    teleport_blockList.clear();

    geyserList.clear();

    boss_field_blockList.clear();

    boss_pillarList.clear();

    boss_carry_object_enemyList.clear();

    boss_carry_object_spawnerList.clear();

    change_filter_boidy_enemy_list.clear();


}


