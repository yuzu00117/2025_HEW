//-----------------------------------------------------------------------------------------------------
// #name collider_type.h
// #description コライダーの種類をセット
// #make 2024/11/22　永野義也
// #update 2024/11/22
// #comment 追加・修正予定
//          ・コライダーの種類を増やしたいときに変更してね
//----------------------------------------------------------------------------------------------------


#ifndef COLLIDER_TYPE_H
#define COLLIDER_TYPE_H

#include <string>
#include"include/box2d/box2d.h"
#include"object_manager.h"

enum ColliderTypeList//フィクスチャの接触判定で使う　新たなあたり判定の種類を追加したい場合ここに足して　下でセットしてね
{
    collider_player_body,
    collider_player_leg,

    collider_player_sensor,

    collider_anchor,
    collider_anchor_chain,

    collider_ground,

    collider_object,

    collider_anchor_point,

    collider_normal_attack_anchor,

    collider_enemy_dynamic,
    collider_enemy_static,
};


// カスタムデータクラス　ここでコライダーのタイプをセット
class ObjectData {
public:
    ColliderTypeList collider_type; // コライダーのタイプをセット                     
    void* extra;                    // 追加情報あるならセット

    // Constructor
    ObjectData(const ColliderTypeList type)
        : collider_type(type), extra(nullptr) {}


    b2Vec2 add_force = { 0.0f,0.0f };// 追加のb2vecデータ
    ObjectType object_name;  // 追加の文字列データ
    int id;//オブジェクトを管理するためのID

    int need_anchor_level;

    // ID を生成する関数
    static int GenerateID() {
        return current_id++;//全てのIDを管理
    }

private:
    // 静的カウンタ ID
    static int current_id;

  


};




#endif // !COLLIDER_TYPE_H
