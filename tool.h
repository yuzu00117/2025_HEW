//-----------------------------------------------------------------------------------------------------
// #name tools.h
// #description �c�[��
// #make 2024/11/22�@�i��`��
// #update 2024/11/22
// #comment �ǉ��E�C���\��
//          �E�K�v�ȃc�[�����ł�����ǉ����銴��
//----------------------------------------------------------------------------------------------------
#ifndef TOOL_H
#define TOOL_H

#include <DirectXMath.h>
#include"include/box2d/box2d.h"

using namespace DirectX;

// ==================================================
// Float2, Float3, Float4�ɕϊ�����֐��̐錾 [��]
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
 * @brief ��Βl��Ԃ��֐�
 * @param value ���ꂽ���l
 * @return ��Βl
 */
float ReturnAbsoluteValue(float value);
#define ZERO2f   XMFLOAT2(0.0f,0.0f)

#endif // TOOL_H