//-----------------------------------------------------------------------------------------------------
// #name tools.h
// #description ツール
// #make 2024/11/22　永野義也
// #update 2024/11/22
// #comment 追加・修正予定
//          ・必要なツールができたら追加する感じ
//----------------------------------------------------------------------------------------------------
#ifndef TOOL_H
#define TOOL_H

#include <DirectXMath.h>
#include"include/box2d/box2d.h"

using namespace DirectX;

// ==================================================
// Float2, Float3, Float4に変換する関数の宣言 [◆]
// ==================================================
XMFLOAT2 toFloat2(const XMFLOAT3& f);
XMFLOAT2 toFloat2(const XMFLOAT4& f);
XMFLOAT2 toFloat2(const XMINT2& f);
XMFLOAT2 toFloat2(const b2Vec2& f);

XMFLOAT3 toFloat3(const XMFLOAT2& f);
XMFLOAT3 toFloat3(const XMFLOAT4& f);
XMFLOAT3 toFloat3(const XMINT2& f);
XMFLOAT3 toFloat3(const b2Vec2& f);

XMFLOAT4 toFloat4(const XMFLOAT2& f);
XMFLOAT4 toFloat4(const XMFLOAT3& f);
XMFLOAT4 toFloat4(const XMINT2& f);
XMFLOAT4 toFloat4(const b2Vec2& f);



/**
 * @brief 絶対値を返す関数
 * @param value 入れたい値
 * @return 絶対値
 */
float ReturnAbsoluteValue(float value);
#define ZERO2f   XMFLOAT2(0.0f,0.0f)

#endif // TOOL_H