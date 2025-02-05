//-----------------------------------------------------------------------------------------------------
// #name collider_type.h
// #description コライダーの種類を定義するセット
// #make 2024/11/22
// #update 2024/11/22
// #comment 追加・修正内容
//          ・コライダーの種類を細分化するために修正を実施
//----------------------------------------------------------------------------------------------------



#ifndef COLLIDER_TYPE_H
#define COLLIDER_TYPE_H

#include <string>
#include"include/box2d/box2d.h"
#include"object_manager.h"
#include"Item_Manager.h"

// コライダータイプの列挙型
enum ColliderTypeList
{
    collider_player_body,
    collider_player_leg,
    collider_player_sensor,
    collider_effect_sensor,

    collider_anchor,
    collider_anchor_chain,
    collider_anchor_point,

    collider_ground,

    collider_object,
    collider_object_destroyer_of_enemy, 
    collider_teleport_block,

    collider_contact_block,

    collider_item,

    collider_geyser_water,

    collider_normal_attack_anchor,

    collider_enemy_dynamic,
    collider_enemy_static,
    collider_enemy_attack,
    collider_enemy_sensor,
    collider_enemy_sensor_move,
    collider_enemy_floating,
    collider_enemy_floating_sensor,

    collider_stage_select_point,
    collider_stage_select_player,

    collider_boss_field,

    collider_boss,
    collider_boss_senosr,
    collider_mini_golem,
    collider_shock_wave,
    collider_chage_attack,
    
    collider_object_carry_enemy,

    collider_blown_away_enemy,
};


// カスタムデータクラス：オブジェクトのコライダータイプ設定
class ObjectData {
public:
    ColliderTypeList collider_type; // コライダーのタイプ設定                     
    void* extra;                    // 拡張用途の設定

    // Constructor
    ObjectData(const ColliderTypeList type)
        : collider_type(type), extra(nullptr) {}


    b2Vec2 add_force = { 0.0f,0.0f }; // 追加のb2Vecデータ
    ObjectType object_name = NULL_object;           // オブジェクト用の識別データ
    ItemType Item_name;               // アイテム用の識別データ
    int id;                           // オブジェクト管理用ID

    int need_anchor_level;			  // 必要なアンカーレベル

    // IDを生成する関数
    static int GenerateID() {
        return current_id++; // 現在のIDを管理
    }

private:
    // 静的カウンタID
    static int current_id;
};

#endif // !COLLIDER_TYPE_H