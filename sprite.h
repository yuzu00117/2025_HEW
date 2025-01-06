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

//	�ꖇ�̉摜�̂��ׂĂ�`��
// (Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������)
void DrawSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Alpha = 1.0f);//Box�Q���悤�ɒ��g��ς��Ă���

//��Ɗ�{�I�ɂ͓��������X�P�[���̒������ύX����Ă��Ȃ����^
void DrawSpriteOld(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Alpha = 1.0f);

//	�ꖇ�̉摜�𕪊����āA���̒��̈������`��
//(Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������)
void DrawDividedSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int Pattern_ID, float Alpha = 1.0f);

//�����\���Ńt�B�[���h�Ɏg����
void DrawDividedSpriteBoss(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int Pattern_ID, float Alpha = 1.0f);


//�@�ꖇ�̉摜�𕪊����āA���̒��̕����𓯎��ɕ`��
// �i�������A�����s�̂��̂����`��ł���j
//  (Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������)
// �i�悭�킩��Ȃ�������T���ɕ����Ă���[�j
void DrawSerialDividedSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int StartPattern_ID, int EndPattern_ID, float Alpha = 1.0f);

#endif//_SPRITE_H