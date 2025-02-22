//-----------------------------------------------------------------------------------------------------
// #name stage_select.h
// #description �X�e�[�W�Z���N�g�̊Ǘ������Ă�w�b�_�[
// #make 2024/12/27�@�@�i��`��
// #update 2024/12/27
// #comment �ǉ��E�C���\��
//                      �E�̂̂���̏オ���Ă��瑫������
//           
//----------------------------------------------------------------------------------------------------

#ifndef STAGE_SELECT_H
#define STAGE_SELECT_H

#include"include/box2d/box2d.h"
#include"scene.h"

class StageSelectScene :public Scene {

public:
    void Initialize() override;

    void Update() override;

    void Draw()override;

    void Finalize() override;


    

private:
    b2World* m_world;


    //�R�C���̕\���ɂ��������_���֐�
    int g_coin_effect_random_number;
    int g_coin_effect_random_number1;
    int g_coin_effect_random_number2;
    int g_coin_effect_random_number3;
    int g_coin_effect_random_number4;


    // �R�C����UV�Ǘ��p�̕ϐ�
    int g_coin_addition = 10;
    int g_coin_addition1 = 20;
    int g_coin_addition2 = 30;
    int g_coin_addition3= 40;
    int g_coin_addition4 = 50;


    // �^�b�v��UV�Ǘ��p�̕ϐ�
    int g_tap_addition = 0;

    //�^�b�v�̃G�t�F�N�g�̊p�x���Ǘ�����
    float g_tap_effect_angle;

    // �i�s�x�i���j
    double rate = 0.0;
    //�z�b�v�A�b�v�̃X�R�A�\���̂���
    int max_coin_cnt = 30;

    //�X�e�[�W���ƂɎ擾�����R�C���̐� �`���[�g���A��
    int get_tutorial_coin_cnt = 0;
    //�X�e�[�W���ƂɎ擾�����R�C���̐��@�P�|�P
    int get_1_1_coin_cnt = 0;
    //�X�e�[�W���ƂɎ擾�����R�C���̐��@�P�|�Q
    int get_1_2_coin_cnt = 0;
    //�X�e�[�W���ƂɎ擾�����R�C���̐��@�P�|�R
    int get_1_3_coin_cnt = 0;



    //�t�F�[�h�̎��Ɏg�� �i�s�x
    double fade_rate = 0.0f;


    double disply_Explanation = 0.0f;

    

    bool Stage_select_flag = false;
 
};

void DestroyWorld(b2World*& world);

#endif // !STAGE_SELECT_H