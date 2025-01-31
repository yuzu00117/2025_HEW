#ifndef PLAYER_UI_H
#define PLAYER_UI_H

#include <DirectXMath.h> // 必要なヘッダーをインクルード

class player_UI {
public:
    player_UI();  // コンストラクタ
    ~player_UI(); // デストラクタ

    // プレイヤーの初期化
    static void Initialize();
    static void Update();
    static void Draw();
    static void Finalize();

private:
    // 静的メンバー変数の型を正しく指定
    static DirectX::XMFLOAT2 player_ui_position;
    static DirectX::XMFLOAT2 player_ui_size;

    static DirectX::XMFLOAT2 gauge_only_position;
    static DirectX::XMFLOAT2 gauge_only_size;
    static float player_ui_alpha;
};

#endif // PLAYER_UI_H