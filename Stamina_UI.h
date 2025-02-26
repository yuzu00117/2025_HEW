//-----------------------------------------------------------------------------------------------------
// #name Stamina_UI.h
// #description 体力のUI
// #make 2025/2/24　王泳心
// #update 2025/2/24
// #comment 追加・修正予定
//      
//
// 
//----------------------------------------------------------------------------------------------------
#ifndef STAMINA_H
#define STAMINA_H

#include <DirectXMath.h> // 必要なヘッダーをインクルード

class Stamina_UI
{
public:
    Stamina_UI();  // コンストラクタ
    ~Stamina_UI(); // デストラクタ

    // プレイヤーの初期化
    static void Initialize();
    static void Update();
    static void Draw();
    static void Finalize();


private:
    // 静的メンバー変数の型を正しく指定
    static DirectX::XMFLOAT2 player_icon_position;
    static DirectX::XMFLOAT2 player_icon_size;

    static DirectX::XMFLOAT2 heart_position[3];
    static DirectX::XMFLOAT2 heart_size[3];

};


#endif // !STAMINA_H

