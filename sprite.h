//-----------------------------------------------------------------------------------------------------
// #name sprite.h
// #description　スプライト！
// #make 2024/11/02　　永野義也
// #update 2024/11/02
// #comment 追加・修正予定
// 
// 　　　　　・いうことないわ　授業通りよ！
// 　　　　　・キンキンのスプライトと自販機って絶滅したの？
//   
//          
//----------------------------------------------------------------------------------------------------
#ifndef _SPRITE_H
#define _SPRITE_H
#include "main.h"
#include "renderer.h"

//マクロ定義

//関数定義
void InitSprite(void);
void UninitSprite(void);

//	一枚の画像のすべてを描画
// (Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ)
void DrawSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Alpha = 1.0f);//Box２ｄように中身を変えている



//背景を描画している
void DrawBgSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Alpha=1.0f);

//上と基本的には同じだがスケールの調整が変更されていない旧型
void DrawSpriteOld(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Alpha = 1.0f);

//	一枚の画像を分割して、その中の一つだけを描画
//(Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ)
void DrawDividedSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int Pattern_ID, float Alpha = 1.0f);


//プレイヤーの描画用に使うよー　中身はボスと基本は同じ
void DrawDividedSpritePlayer(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int Pattern_ID, float Alpha, bool left);


void DrawDividedSpriteBoss(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int Pattern_ID, float Alpha, bool left);

//現在使っているスケールに合わせた

void DrawSplittingSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int Pattern_ID, float Alpha);

//　一枚の画像を分割して、その中の複数を同時に描画
// （ただし、同じ行のものだけ描画できる）
//  (Alpha値はデフォルトで1.0、変更がなければ値を渡さなくてもいいよ)
// （よくわからなかったらサムに聞いてちょー）
void DrawSerialDividedSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int StartPattern_ID, int EndPattern_ID, float Alpha = 1.0f);

//ソウルゲージ描画用
//TotalRowsがないのは、どんな時でもRowsは1だと想定してるからである
void DrawGaugeSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int GaugeBottom_Pattern_ID, int Value_Pattern_ID, float Alpha = 1.0f);

#endif//_SPRITE_H