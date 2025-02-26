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



//�w�i��`�悵�Ă���
void DrawBgSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Alpha=1.0f);

//��Ɗ�{�I�ɂ͓��������X�P�[���̒������ύX����Ă��Ȃ����^
void DrawSpriteOld(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, float Alpha = 1.0f);

//	�ꖇ�̉摜�𕪊����āA���̒��̈������`��
//(Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������)
void DrawDividedSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int Pattern_ID, float Alpha = 1.0f);


//�v���C���[�̕`��p�Ɏg����[�@���g�̓{�X�Ɗ�{�͓���
void DrawDividedSpritePlayer(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int Pattern_ID, float Alpha, bool left);


void DrawDividedSpriteBoss(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int Pattern_ID, float Alpha, bool left);

//���ݎg���Ă���X�P�[���ɍ��킹��

void DrawSplittingSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int Pattern_ID, float Alpha);

//�@�ꖇ�̉摜�𕪊����āA���̒��̕����𓯎��ɕ`��
// �i�������A�����s�̂��̂����`��ł���j
//  (Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������)
// �i�悭�킩��Ȃ�������T���ɕ����Ă���[�j
void DrawSerialDividedSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int TotalRows, int StartPattern_ID, int EndPattern_ID, float Alpha = 1.0f);

//�\�E���Q�[�W�`��p
//TotalRows���Ȃ��̂́A�ǂ�Ȏ��ł�Rows��1���Ƒz�肵�Ă邩��ł���
void DrawGaugeSprite(XMFLOAT2 Position, float Rotation, XMFLOAT2 Scale, int TotalCols, int GaugeBottom_Pattern_ID, int Value_Pattern_ID, float Alpha = 1.0f);

#endif//_SPRITE_H