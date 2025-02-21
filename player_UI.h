#ifndef PLAYER_UI_H
#define PLAYER_UI_H

#include <DirectXMath.h> // 必要なヘッダーをインクルード
#include "Item_Jewel.h"

class player_UI {
public:
    player_UI();  // コンストラクタ
    ~player_UI(); // デストラクタ

    // プレイヤーの初期化
    static void Initialize();
    static void Update();
    static void Draw();
    static void Finalize();

    static DirectX::XMFLOAT2 GetRingPosition() { return m_ring_position; }

    static  void    SetJewelCollected(Jewel_Type type, bool flag) {
        switch (type)
        {
        case BLUE:
            m_blue_jewel_collected = flag;
            m_blue_jewel_collected_effect_cnt = 1;
            break;
        case RED:
            m_red_jewel_collected = flag;
            m_red_jewel_collected_effect_cnt = 1;
            break;
        case YELLOW:
            m_yellow_jewel_collected = flag;
            m_yellow_jewel_collected_effect_cnt = 1;
            break;
        }
    }


private:
    // 静的メンバー変数の型を正しく指定
    static DirectX::XMFLOAT2 player_ui_position;
    static DirectX::XMFLOAT2 player_ui_size;

    static DirectX::XMFLOAT2 gauge_only_position;
    static DirectX::XMFLOAT2 gauge_only_size;
    static float player_ui_alpha;

    static bool    m_blue_jewel_collected;
    static bool    m_red_jewel_collected;
    static bool    m_yellow_jewel_collected;

    static float    m_blue_jewel_collected_effect_cnt;
    static float    m_red_jewel_collected_effect_cnt;
    static float    m_yellow_jewel_collected_effect_cnt;

    static DirectX::XMFLOAT2 m_ring_position;
};

#endif // PLAYER_UI_H