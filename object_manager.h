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
#include "main.h"
#include "wood.h"
#include"one-way_platform.h"
#include"sloping_block.h"
#include"rock.h"
#include"static_to_dynamic_block.h"
#include"movable_ground.h"
#include"enemy_static.h"
#include"enemy_dynamic.h"
#include"enemy_attack.h"
#include"enemy_floating.h"
#include"teleport_block.h"
#include"1-1_boss_field_block.h"
#include"Item_Coin_UI.h"
#include"1_1_boss_pillar.h"
#include"geyser.h"
#include"1_1_boss_carry_object_enemy_spawner.h"
#include"Change_Enemy_Filter_and_Body.h"

#include"bound_block.h"

#include"contact_block.h"


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
    Object_Enemy_Attack, //エネミーの攻撃
    Object_Enemy_Floating,//浮遊エネミー

    Object_Geyser,//間欠泉

    Object_teleport_block,//テレポートブロック

    Object_contact_block,//接触ブロック

    Object_Geyser_Water,
    Boss_core,//ボスのこあ
    Boss_pillar,//ボスの柱
    Boss_Carry_Object_Enemy,//ボス部屋のエネミーがオブジェクトを運ぶやつ
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
    //エネミーの攻撃の生成
    void AddEnemyAttack(b2Vec2 position, b2Vec2 body_size, float angle, int id);
    //浮遊エネミー生成
    void AddEnemyFloating(b2Vec2 position, b2Vec2 body_size, float angle);

    void AddGeyser(b2Vec2 GeyserPosition, b2Vec2 GeyserSize, b2Vec2 RangeFlyWaterSize,int splitting_x, int splitting_y, Boss_Room_Level level);

    void AddTeleportBlock(b2Vec2 position, b2Vec2 size, b2Vec2 to_teleport_position);

    void AddBossFieldBlock(b2Vec2 position, b2Vec2 body_size, int block_hp, Boss_Room_Level level);

    void AddBossPillar(b2Vec2 position, b2Vec2 size, int splitting_x, int splitting_y, Boss_Room_Level level);

    void AddBossCarryEnemySpawner(b2Vec2 position, b2Vec2 Size, Boss_Room_Level level, bool left);

    void AddBossCarryObjectEnemy(b2Vec2 position, b2Vec2 enemy_size, bool left, float enemy_speed, b2Vec2 object_size, int object_type, int anchor_level);

    void AddChangeEnemyFilterAndBody(b2Vec2 position, b2Vec2 size, b2Vec2 velocity, ID3D11ShaderResourceView* Textur, int texture_x, int texture_y,b2Vec2 vec);


    void AddBossBoundBlock(b2Vec2 position, b2Vec2 size, b2Vec2 vec, Boss_Room_Level level);

    void AddContactBlock(b2Vec2 Position, b2Vec2 block_size, Contact_Block_Type type, b2Vec2 num);


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
    //IDを使ってエネミーの攻撃を検索
    EnemyAttack* FindEnemyAttackByID(int id);
    //IDを使って浮遊エネミーを検索
    EnemyFloating* FindEnemyFloatingByID(int id);

    geyser* FindGeyserID(int id);
    

    teleport_block* FindTeleportBlock(int id);

    boss_field_block* FindBossFieldBlock(int id);

    boss_pillar* FindBossPillar(int id);

    boss_carry_object_spawner* FindBossCarryEnemySpawner(int id);

    boss_carry_object_enemy* FindBossCarryObjectEnemy(int id);

    change_enemy_filter_and_body* FindChangeEnemyFilterAndBody(int id);


    boss_bound_block* FindBossBoundBlock(int id);


    contact_block* FindContactBlock(int id);


    

    
    //IDとオブジェクトタイプでオブジェクトを検索
    Object* FindObjectByID_ObjectType(int id, ObjectType type);

    //指定の静的エネミーを削除
    void DestroyEnemyStatic(int id);
    //指定の動的エネミーを削除
    void DestroyEnemyDynamic(int id);
    //指定のエネミーの攻撃を削除
    void DestroyEnemyAttack(int id);
    //指定の浮遊エネミーを削除
    void DestroyEnemyFloating(int id);


    // 全てのオブジェクトを初期化
    void InitializeAll();

    // 全てのオブジェクトを更新
    void UpdateAll();

    // 全てのオブジェクトを描画
    void DrawAll();


    //全面に表示する　UI　エフェクトなど
    void DrawFront();

    // 全てのオブジェクトを破棄
    void FinalizeAll();

    //  引っ張る強さを更新
    void    SetPullingPower_With_Multiple(b2Vec2 multiple);

private:
    std::vector<std::unique_ptr<wood>> woodList;//木のリスト
    std::vector < std::unique_ptr<rock>>rockList;//岩のリスト
    std::vector<std::unique_ptr<one_way_platform>> one_way_platformList;// 足場のリスト
    std::vector<std::unique_ptr<sloping_block>> sloping_blockList;//斜面のリスト
    std::vector<std::unique_ptr<static_to_dynamic_block>> static_to_dynamic_blockList;//静的→動的ブロック挙動
    std::vector<std::unique_ptr<movable_ground>> movable_groundList;//木のリスト
    std::vector<std::unique_ptr<EnemyStatic>> enemy_staticList;//静的エネミーのリスト
    std::vector<std::unique_ptr<EnemyDynamic>> enemy_dynamicList;//動的エネミーのリスト
    std::vector<std::unique_ptr<EnemyAttack>> enemy_attackList;//エネミーの攻撃のリスト
    std::vector<std::unique_ptr<EnemyFloating>> enemy_floatingList;//浮遊エネミーのリスト

    std::vector<std::unique_ptr<teleport_block>> teleport_blockList;//テレポートブロック


    std::vector<std::unique_ptr<geyser>>geyserList;//間欠泉

    std::vector<std::unique_ptr<boss_field_block>> boss_field_blockList;//ボス部屋の床
    std::vector<std::unique_ptr<boss_pillar>> boss_pillarList;//ボスの部屋の柱

  
  
    std::vector<std::unique_ptr<boss_carry_object_spawner>> boss_carry_object_spawnerList;//ボスの部屋で出現するエネミーのスポナー

    std::vector<std::unique_ptr<boss_carry_object_enemy>> boss_carry_object_enemyList;//ボスの部屋で出現するエネミーのスポナー



    std::vector<std::unique_ptr<change_enemy_filter_and_body>>change_filter_boidy_enemy_list ;//撃墜演出されたエネミーのリスト

    std::vector<std::unique_ptr<boss_bound_block>>boss_bound_block_list;//バウンドブロックのリスト


    std::vector<std::unique_ptr<contact_block>>contact_block_list;
   
    //ここにオブジェクトごとにリストを追加していく感じ


    ObjectManager() = default;
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;
};

#endif // OBJECT_MANAGER_H
