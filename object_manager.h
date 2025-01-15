//-----------------------------------------------------------------------------------------------------
// #name object_manager
// #description オブジェクトを管理するためのファクトリーのイメージに近い
// #make 2024/12/04　永野義也
// #update 2024/12/13
// #comment 追加・修正予定
//          ・オブジェクトを作るごとに生成する感じ
//----------------------------------------------------------------------------------------------------


#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <vector>
#include <memory>
#include "wood.h"
#include"one-way_platform.h"
#include"sloping_block.h"
#include"rock.h"
#include"static_to_dynamic_block.h"
#include"movable_ground.h"
#include"enemy_static.h"
#include"enemy_dynamic.h"
#include"1-1_boss_field_block.h"

// オブジェクトの種類を定義
enum ObjectType {
    NULL_object,
    Object_Wood, // 木
    Object_Rock, // 岩
    Object_one_way_platform,//足場　したからしか乗れない
    Object_Static_to_Dynamic,//静的から動的に変更するオブジェクト
    Object_Movable_Ground,  //引っ張れる床 

    
    Object_Enemy_Static,//静的エネミー
    Object_Enemy_Dynamic,//動的エネミー
};

class Object{};

// オブジェクトを管理するクラス
class ObjectManager {
public:
    // シングルトンのインスタンス取得
    static ObjectManager& GetInstance();

    // 木を追加
    void AddWood(const b2Vec2& position, const b2Vec2& woodSize, const b2Vec2& anchorPointSize,const int&need_level);

    //岩を追加
    void AddRock(const b2Vec2& position, const float& radius, const int& need_anchor_level);
    //足場を追加
    void AddOne_way_platformList(const b2Vec2& position, const b2Vec2& local_position, const b2Vec2 &size);
    //傾斜ブロックの追加
    void AddSloping_block(const b2Vec2& position, const b2Vec2& size, const SlopingBlockAspect& aspect);
    //静的→動的のブロックの追加
    void AddStatic_to_Dynamic_block(const b2Vec2& position, const b2Vec2& size, const collider_type_Box_or_Circle& collider_type, const int& need_level);
    // 引っ張れる床を追加
    void AddMovable_Ground(const b2Vec2& position, const b2Vec2& groundSize, const b2Vec2& anchorPointSize, const int& need_level);
    //静的エネミー生成
    void AddEnemyStatic(b2Vec2 position, b2Vec2 body_size, float angle);
    //動的エネミー生成
    void AddEnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle);

    void AddBossFieldBlock(b2Vec2 position, b2Vec2 body_size, int block_hp, Boss_Room_Level level);

    // ID を使って木を検索
    wood* FindWoodByID(int id);
    //IDを使って　岩を検索
    rock* FindRockByID(int id);
    //IDを使って足場ブロックを検索
    one_way_platform* Findone_way_platformByID(int id);
    //IDを使って斜面ブロックを検索
    sloping_block* FindSloping_BlockByID(int id);
    //IDを使って静的→動的ブロックを追加
    static_to_dynamic_block* FindStatic_to_Dynamic_BlcokID(int id);
    //IDを使って引っ張れる床を検索
    movable_ground* FindMovable_GroundID(int id);
    //IDを使って静的エネミーを検索
    EnemyStatic* FindEnemyStaticByID(int id);
    //IDを使って動的エネミーを検索
    EnemyDynamic* FindEnemyDynamicByID(int id);

    boss_field_block* FindBossFieldBlock(int id);
    
    //IDとオブジェクトタイプでオブジェクトを検索
    Object* FindObjectByID_ObjectType(int id, ObjectType type);

    //指定の静的エネミーを削除
    void DestroyEnemyStatic(int id);
    //指定の動的エネミーを削除
    void DestroyEnemyDynamic(int id);


    // 全てのオブジェクトを初期化
    void InitializeAll();

    // 全てのオブジェクトを更新
    void UpdateAll();

    // 全てのオブジェクトを描画
    void DrawAll();

    // 全てのオブジェクトを破棄
    void FinalizeAll();

private:
    std::vector<std::unique_ptr<wood>> woodList;//木のリスト
    std::vector < std::unique_ptr<rock>>rockList;//岩のリスト
    std::vector<std::unique_ptr<one_way_platform>> one_way_platformList;// 足場のリスト
    std::vector<std::unique_ptr<sloping_block>> sloping_blockList;//斜面のリスト
    std::vector<std::unique_ptr<static_to_dynamic_block>> static_to_dynamic_blockList;//静的→動的ブロック挙動
    std::vector<std::unique_ptr<movable_ground>> movable_groundList;//木のリスト
    std::vector<std::unique_ptr<EnemyStatic>> enemy_staticList;//静的エネミーのリスト
    std::vector<std::unique_ptr<EnemyDynamic>> enemy_dynamicList;//静的エネミーのリスト

    std::vector<std::unique_ptr<boss_field_block>> boss_filed_block_List;//静的エネミーのリスト

    //ここにオブジェクトごとにリストを追加していく感じ


    ObjectManager() = default;
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;
};

#endif // OBJECT_MANAGER_H
