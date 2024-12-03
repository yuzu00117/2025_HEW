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
void DrawSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale);
void DrawSpriteAnim(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int AnimCols, int AnimRows, int AnimPattern);

#endif//_SPRITE_H