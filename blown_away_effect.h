
//-----------------------------------------------------------------------------------------------------
// #name blown_away__effect.h
// #description �G���v���C���[�̃Z���T�[����ł��牉�o�𔭐�������
// #make 2025/02/03�@�i��`��
// #update 2024/02/03
// #comment �ǉ��E�C���\��
//          
//----------------------------------------------------------------------------------------------------
#ifndef BLOWN_AWAY_EFFECT_H
#define BLOWN_AWAY_EFFECT_H

#include"include/box2d/box2d.h"
#include<vector>
// **�Փ˃G�t�F�N�g�̃f�[�^**
struct Blown_Away_Effect {
    b2Vec2 pos;    // �G�t�F�N�g�̍��W
    float angle;   // �G�t�F�N�g�̉�]�p�x�i���W�A���j
    float scale;   // �G�t�F�N�g�̑傫��
    int lifeTime;  // �G�t�F�N�g�̎����i�t���[�����j
    int type;      // �G�t�F�N�g�̎�ށi0: �y��, 1: �Ή�, 2: ���� �Ȃǁj

    // **�R���X�g���N�^**
    Blown_Away_Effect(b2Vec2 position, float effectAngle, float effectScale, int effectType)
        : pos(position), angle(effectAngle), scale(effectScale), lifeTime(30), type(effectType) {}
};

// **�O���[�o���ϐ��Ƃ��� `impactEffects` ��錾�i��`�� impact_effect.cpp �Ɉړ��j**
extern std::vector<Blown_Away_Effect> blown_away_Effects;


void InitBlownAwayEffect();
void UpdateBlownAwayEffects();
void DrawBlownAwayEffects(float scale);
void FinalizeBlownAwayEffects();


#endif // !BLOWN_AWAY_EFFECT_H
