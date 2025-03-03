// ---------------------------------------------------------------------------------------------------- -
// #name result.h
// #description     リザルト画面用ヘッダー
// #make 2025/02/25 髙橋連
// #update 2024/02/27
// #comment 追加・修正予定
//----------------------------------------------------------------------------------------------------

#ifndef RESULT_H
#define RESULT_H

#include "scene.h"

//スコアランクの描画サイズ
#define RANK_SIZE (150)
#define TOTAL_RANK_SIZE (190)
#define NUMBER_SIZE (50)

enum RESULT_STATE
{
    STATE_RESULT_START,
    STATE_RESULT_COIN,
    STATE_RESULT_DYNAMIC,
    STATE_RESULT_BOSS,
    STATE_RESULT_TOTAL,
    STATE_RESULT_FINISH
};

class ResultScene :public Scene {

public:
    void Initialize() override;

    void Update() override;

    void Draw()override;

    void Finalize() override;

private:
    float m_back_alpha = 0.0f;

    int m_coin_count = 0;
    const XMFLOAT2 m_coin_pos_finish = XMFLOAT2(500.0f, 215.0f);
    XMFLOAT2 m_coin_pos = XMFLOAT2(600.0f, 215.0f);
    float m_coin_alpha = 0;
    float m_coin_score_alpha = 0;

    int m_gokai_count = 0;
    const XMFLOAT2 m_gokai_pos_finish = XMFLOAT2(500.0f, 295.0f);
    XMFLOAT2 m_gokai_pos = XMFLOAT2(600.0f, 295.0f);
    float m_gokai_alpha = 0;
    float m_gokai_score_alpha = 0;

    int m_boss_clear_time = 0;
    const XMFLOAT2 m_boss_pos_finish = XMFLOAT2(500.0f, 385.0f);
    XMFLOAT2 m_boss_pos = XMFLOAT2(600.0f, 385.0f);
    float m_boss_alpha = 0;
    float m_boss_score_alpha = 0;

    int m_total_score_points = 0;
    float m_total_gage_filling = 0;
    float m_total_score_alpha = 0;

    int m_state = STATE_RESULT_START;
    int m_ui_move_speed = 2;
    int m_gage_move_speed = 4;
    int m_score_alpha_move_speed = 5;
};

#endif //RESULT_H