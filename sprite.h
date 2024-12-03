//-----------------------------------------------------------------------------------------------------
// #name sprite.h
// #description�@�X�v���C�g�I
// #make 2024/11/02�@�@�i��`��
// #update 2024/11/02
// #comment �ǉ��E�C���\��
// 
// �@�@�@�@�@�E�������ƂȂ���@���ƒʂ��I
// �@�@�@�@�@�E�L���L���̃X�v���C�g�Ǝ��̋@���Đ�ł����́H
//   
//          
//----------------------------------------------------------------------------------------------------
#ifndef _SPRITE_H
#define _SPRITE_H
#include "main.h"
#include "renderer.h"

//�}�N����`

//�֐���`
void InitSprite(void);
void UninitSprite(void);
void DrawSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale);
void DrawSpriteAnim(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int AnimCols, int AnimRows, int AnimPattern);

#endif//_SPRITE_H