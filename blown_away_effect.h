
//-----------------------------------------------------------------------------------------------------
// #name blown_away__effect.h
// #description 敵がプレイヤーのセンサーからでたら演出を発生させる
// #make 2025/02/03　永野義也
// #update 2024/02/03
// #comment 追加・修正予定
//          
//----------------------------------------------------------------------------------------------------
#ifndef BLOWN_AWAY_EFFECT_H
#define BLOWN_AWAY_EFFECT_H

#include"include/box2d/box2d.h"
#include<vector>
// **衝突エフェクトのデータ**
struct Blown_Away_Effect {
    b2Vec2 pos;    // エフェクトの座標
    float angle;   // エフェクトの回転角度（ラジアン）
    float scale;   // エフェクトの大きさ
    int lifeTime;  // エフェクトの寿命（フレーム数）
    int type;      // エフェクトの種類（0: 土煙, 1: 火花, 2: 爆発 など）

    // **コンストラクタ**
    Blown_Away_Effect(b2Vec2 position, float effectAngle, float effectScale, int effectType)
        : pos(position), angle(effectAngle), scale(effectScale), lifeTime(30), type(effectType) {}
};

// **グローバル変数として `impactEffects` を宣言（定義は impact_effect.cpp に移動）**
extern std::vector<Blown_Away_Effect> blown_away_Effects;


void InitBlownAwayEffect();
void UpdateBlownAwayEffects();
void DrawBlownAwayEffects(float scale);
void FinalizeBlownAwayEffects();


#endif // !BLOWN_AWAY_EFFECT_H
