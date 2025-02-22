//-----------------------------------------------------------------------------------------------------
// #name stage_select.h
// #description ステージセレクトの管理をしてるヘッダー
// #make 2024/12/27　　永野義也
// #update 2024/12/27
// #comment 追加・修正予定
//                      ・ののさんの上がってから足すかも
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


    //コインの表示につかうランダム関数
    int g_coin_effect_random_number;
    int g_coin_effect_random_number1;
    int g_coin_effect_random_number2;
    int g_coin_effect_random_number3;
    int g_coin_effect_random_number4;


    // コインのUV管理用の変数
    int g_coin_addition = 10;
    int g_coin_addition1 = 20;
    int g_coin_addition2 = 30;
    int g_coin_addition3= 40;
    int g_coin_addition4 = 50;


    // タップのUV管理用の変数
    int g_tap_addition = 0;

    //タップのエフェクトの角度を管理する
    float g_tap_effect_angle;

    // 進行度（仮）
    double rate = 0.0;
    //ホップアップのスコア表示のため
    int max_coin_cnt = 30;

    //ステージごとに取得したコインの数 チュートリアル
    int get_tutorial_coin_cnt = 0;
    //ステージごとに取得したコインの数　１－１
    int get_1_1_coin_cnt = 0;
    //ステージごとに取得したコインの数　１－２
    int get_1_2_coin_cnt = 0;
    //ステージごとに取得したコインの数　１－３
    int get_1_3_coin_cnt = 0;



    //フェードの時に使う 進行度
    double fade_rate = 0.0f;


    double disply_Explanation = 0.0f;

    

    bool Stage_select_flag = false;
 
};

void DestroyWorld(b2World*& world);

#endif // !STAGE_SELECT_H