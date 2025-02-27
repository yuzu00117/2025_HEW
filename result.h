// ---------------------------------------------------------------------------------------------------- -
// #name result.h
// #description     リザルト画面用ヘッダー
// #make 2025/02/25 髙橋連
// #update 2024/02/26
// #comment 追加・修正予定
//----------------------------------------------------------------------------------------------------

#ifndef RESULT_H
#define RESULT_H

#include "scene.h"

//スコアランクの描画サイズ
#define RANK_SIZE (150)
#define TOTAL_RANK_SIZE (200)
#define NUMBER_SIZE (50)

enum RESULT_STATE
{
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
    int m_coin_count = 0;
    const XMFLOAT2 m_coin_pos_finish = XMFLOAT2(500.0f, 160.0f);
    XMFLOAT2 m_coin_pos = XMFLOAT2(500.0f, 160.0f);
    float m_coin_alpha = 0;

    int m_gokai_count = 0;
    const XMFLOAT2 m_gokai_pos_finish = XMFLOAT2(500.0f, 240.0f);
    XMFLOAT2 m_gokai_pos = XMFLOAT2(500.0f, 240.0f);
    float m_gokai_alpha = 0;

    int m_boss_clear_time = 0;
    const XMFLOAT2 m_boss_pos_finish = XMFLOAT2(500.0f, 330.0f);
    XMFLOAT2 m_boss_pos = XMFLOAT2(500.0f, 330.0f);
    float m_boss_alpha = 0;

    int m_total_score_points = 0;

    int m_state = STATE_RESULT_COIN;
    bool m_is_display_score = false;
};

#endif //RESULT_H