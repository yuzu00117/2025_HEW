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



enum ColliderTypeList//フィクスチャの接触判定で使う　新たなあたり判定の種類を追加したい場合ここに足して　下でセットしてね
{
    collider_player,

    collider_player_sensor,

    collider_anchor,
    collider_anchor_chain,

    collider_ground,
    collider_wall,

    collider_anchor_point,

};


// カスタムデータクラス　ここでコライダーのタイプをセット
class ObjectData {
public:
    ColliderTypeList collider_type; // コライダーのタイプをセット                     
    void* extra;                    // 追加情報あるならセット

    // Constructor
    ObjectData(const ColliderTypeList type)
        : collider_type(type), extra(nullptr) {}
};




#endif // !COLLIDER_TYPE_H
