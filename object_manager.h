//-----------------------------------------------------------------------------------------------------
// #name object_manager
// #description 繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ邂｡逅�☆繧九◆繧√�繝輔ぃ繧ｯ繝医Μ繝ｼ縺ｮ繧､繝｡繝ｼ繧ｸ縺ｫ霑代＞
// #make 2024/12/04縲豌ｸ驥守ｾｩ荵
// #update 2024/12/13
// #comment 霑ｽ蜉繝ｻ菫ｮ豁｣莠亥ｮ
//          繝ｻ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ菴懊ｋ縺斐→縺ｫ逕滓�縺吶ｋ諢溘§
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
#include"enemy_attack.h"
#include"teleport_block.h"

// 繧ｪ繝悶ず繧ｧ繧ｯ繝医�遞ｮ鬘槭ｒ螳夂ｾｩ
enum ObjectType {
    NULL_object,
    Object_Wood, // 譛ｨ
    Object_Rock, // 蟯ｩ
    Object_one_way_platform,//雜ｳ蝣ｴ縲縺励◆縺九ｉ縺励°荵励ｌ縺ｪ縺
    Object_Static_to_Dynamic,//髱咏噪縺九ｉ蜍慕噪縺ｫ螟画峩縺吶ｋ繧ｪ繝悶ず繧ｧ繧ｯ繝
    Object_Movable_Ground,  //蠑輔▲蠑ｵ繧後ｋ蠎 
    
    Object_Enemy_Static, //髱咏噪繧ｨ繝阪Α繝ｼ
    Object_Enemy_Dynamic,//蜍慕噪繧ｨ繝阪Α繝ｼ
    Object_Enemy_Attack, //繧ｨ繝阪Α繝ｼ縺ｮ謾ｻ謦
    Object_teleport_block,//テレポートブロック

};

class Object{};

// 繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ邂｡逅�☆繧九け繝ｩ繧ｹ
class ObjectManager {
public:
    // 繧ｷ繝ｳ繧ｰ繝ｫ繝医Φ縺ｮ繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ蜿門ｾ
    static ObjectManager& GetInstance();

    // 譛ｨ繧定ｿｽ蜉
    void AddWood(const b2Vec2& position, const b2Vec2& woodSize, const b2Vec2& anchorPointSize,const int&need_level);

    //蟯ｩ繧定ｿｽ蜉
    void AddRock(const b2Vec2& position, const float& radius, const int& need_anchor_level);
    //雜ｳ蝣ｴ繧定ｿｽ蜉
    void AddOne_way_platformList(const b2Vec2& position, const b2Vec2& local_position, const b2Vec2 &size);
    //蛯ｾ譁懊ヶ繝ｭ繝�け縺ｮ霑ｽ蜉
    void AddSloping_block(const b2Vec2& position, const b2Vec2& size, const SlopingBlockAspect& aspect);
    //髱咏噪竊貞虚逧��繝悶Ο繝�け縺ｮ霑ｽ蜉
    void AddStatic_to_Dynamic_block(const b2Vec2& position, const b2Vec2& size, const collider_type_Box_or_Circle& collider_type, const int& need_level);
    // 蠑輔▲蠑ｵ繧後ｋ蠎翫ｒ霑ｽ蜉
    void AddMovable_Ground(const b2Vec2& position, const b2Vec2& groundSize, const b2Vec2& anchorPointSize, const int& need_level);
    //髱咏噪繧ｨ繝阪Α繝ｼ逕滓�
    void AddEnemyStatic(b2Vec2 position, b2Vec2 body_size, float angle);
    //蜍慕噪繧ｨ繝阪Α繝ｼ逕滓�
    void AddEnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle);
    //繧ｨ繝阪Α繝ｼ縺ｮ謾ｻ謦��逕滓�
    void AddEnemyAttack(b2Vec2 position, b2Vec2 body_size, float angle);

    // ID 繧剃ｽｿ縺｣縺ｦ譛ｨ繧呈､懃ｴ｢
    void AddTeleportBlock(b2Vec2 position, b2Vec2 size, b2Vec2 to_teleport_position);

    // ID を使って木を検索

    wood* FindWoodByID(int id);
    //ID繧剃ｽｿ縺｣縺ｦ縲蟯ｩ繧呈､懃ｴ｢
    rock* FindRockByID(int id);
    //ID繧剃ｽｿ縺｣縺ｦ雜ｳ蝣ｴ繝悶Ο繝�け繧呈､懃ｴ｢
    one_way_platform* Findone_way_platformByID(int id);
    //ID繧剃ｽｿ縺｣縺ｦ譁憺擇繝悶Ο繝�け繧呈､懃ｴ｢
    sloping_block* FindSloping_BlockByID(int id);
    //ID繧剃ｽｿ縺｣縺ｦ髱咏噪竊貞虚逧�ヶ繝ｭ繝�け繧定ｿｽ蜉
    static_to_dynamic_block* FindStatic_to_Dynamic_BlcokID(int id);
    //ID繧剃ｽｿ縺｣縺ｦ蠑輔▲蠑ｵ繧後ｋ蠎翫ｒ讀懃ｴ｢
    movable_ground* FindMovable_GroundID(int id);
    //ID繧剃ｽｿ縺｣縺ｦ髱咏噪繧ｨ繝阪Α繝ｼ繧呈､懃ｴ｢
    EnemyStatic* FindEnemyStaticByID(int id);
    //ID繧剃ｽｿ縺｣縺ｦ蜍慕噪繧ｨ繝阪Α繝ｼ繧呈､懃ｴ｢
    EnemyDynamic* FindEnemyDynamicByID(int id);
    //ID繧剃ｽｿ縺｣縺ｦ繧ｨ繝阪Α繝ｼ縺ｮ謾ｻ謦�ｒ讀懃ｴ｢
    EnemyAttack* FindEnemyAttackByID(int id);

    teleport_block* FindTeleportBlock(int id);

    
    //ID縺ｨ繧ｪ繝悶ず繧ｧ繧ｯ繝医ち繧､繝励〒繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ讀懃ｴ｢
    Object* FindObjectByID_ObjectType(int id, ObjectType type);

    //謖�ｮ壹�髱咏噪繧ｨ繝阪Α繝ｼ繧貞炎髯､
    void DestroyEnemyStatic(int id);
    //謖�ｮ壹�蜍慕噪繧ｨ繝阪Α繝ｼ繧貞炎髯､
    void DestroyEnemyDynamic(int id);
    //謖�ｮ壹�繧ｨ繝阪Α繝ｼ縺ｮ謾ｻ謦�ｒ蜑企勁
    void DestroyEnemyAttack(int id);


    // 蜈ｨ縺ｦ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ蛻晄悄蛹
    void InitializeAll();

    // 蜈ｨ縺ｦ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ譖ｴ譁ｰ
    void UpdateAll();

    // 蜈ｨ縺ｦ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ謠冗判
    void DrawAll();

    // 蜈ｨ縺ｦ縺ｮ繧ｪ繝悶ず繧ｧ繧ｯ繝医ｒ遐ｴ譽
    void FinalizeAll();

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
    std::vector<std::unique_ptr<teleport_block>> teleport_blockList;//テレポートブロック
    //ここにオブジェクトごとにリストを追加していく感じ



    ObjectManager() = default;
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;
};

#endif // OBJECT_MANAGER_H
