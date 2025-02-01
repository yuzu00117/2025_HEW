//-----------------------------------------------------------------------------------------------------
// #name impact_effect.h
// #description �Փ˂̎��̏Ռ����擾���ăG�t�F�N�g��\������
// #make 2025/02/01�@�i��`��
// #update 2024/02/01
// #comment �ǉ��E�C���\��
//          �E�Փˎ��̃G�t�F�N�g�ɕύX������ΕύX
//----------------------------------------------------------------------------------------------------
#ifndef IMPACT_EFFECT_H
#define IMPACT_EFFECT_H

#include"include/box2d/box2d.h"
#include<vector>
// **�Փ˃G�t�F�N�g�̃f�[�^**
struct ImpactEffect {
    b2Vec2 pos;    // �G�t�F�N�g�̍��W
    float angle;   // �G�t�F�N�g�̉�]�p�x�i���W�A���j
    float scale;   // �G�t�F�N�g�̑傫��
    int lifeTime;  // �G�t�F�N�g�̎����i�t���[�����j
    int type;      // �G�t�F�N�g�̎�ށi0: �y��, 1: �Ή�, 2: ���� �Ȃǁj

    // **�R���X�g���N�^**
    ImpactEffect(b2Vec2 position, float effectAngle, float effectScale, int effectType)
        : pos(position), angle(effectAngle), scale(effectScale), lifeTime(30), type(effectType) {}
};

// **�O���[�o���ϐ��Ƃ��� `impactEffects` ��錾�i��`�� impact_effect.cpp �Ɉړ��j**
extern std::vector<ImpactEffect> impactEffects;


void InitImpactEffect();
void UpdateImpactEffects();
void DrawImpactEffects(float scale);
void FinalizeImpactEffects();


#endif // !IMPACT_EFFECT_H
