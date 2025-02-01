//-----------------------------------------------------------------------------------------------------
// #name impact_effect.h
// #description 衝突の時の衝撃を取得してエフェクトを表示する
// #make 2025/02/01　永野義也
// #update 2024/02/01
// #comment 追加・修正予定
//          ・衝突時のエフェクトに変更があれば変更
//----------------------------------------------------------------------------------------------------
#ifndef IMPACT_EFFECT_H
#define IMPACT_EFFECT_H

#include"include/box2d/box2d.h"
#include<vector>
// **衝突エフェクトのデータ**
struct ImpactEffect {
    b2Vec2 pos;    // エフェクトの座標
    float angle;   // エフェクトの回転角度（ラジアン）
    float scale;   // エフェクトの大きさ
    int lifeTime;  // エフェクトの寿命（フレーム数）
    int type;      // エフェクトの種類（0: 土煙, 1: 火花, 2: 爆発 など）

    // **コンストラクタ**
    ImpactEffect(b2Vec2 position, float effectAngle, float effectScale, int effectType)
        : pos(position), angle(effectAngle), scale(effectScale), lifeTime(30), type(effectType) {}
};

// **グローバル変数として `impactEffects` を宣言（定義は impact_effect.cpp に移動）**
extern std::vector<ImpactEffect> impactEffects;


void InitImpactEffect();
void UpdateImpactEffects();
void DrawImpactEffects(float scale);
void FinalizeImpactEffects();


#endif // !IMPACT_EFFECT_H
