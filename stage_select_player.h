//-----------------------------------------------------------------------------------------------------
// #name stage_select_player.cpp
// #description ステージセレクトの動かすプレイヤーの管理をしている
// #make 2024/12/27　　永野義也
// #update 2024/12/27
// #comment 追加・修正予定
//                      ・ののさんの上がってから足すかも
//           
//----------------------------------------------------------------------------------------------------
#ifndef STAGE_SELECT_PLAYER_H
#define STAGE_SELECT_PLAYER_H
#include"include/box2d/box2d.h"

#include <memory>

class StageSelectPlayer {
public:
    StageSelectPlayer();  // コンストラクタ
    ~StageSelectPlayer(); // デストラクタ


    // シングルトンのインスタンス取得
    static StageSelectPlayer& GetInstance() {
        static StageSelectPlayer instance; // 静的インスタンス
        return instance;
    }

    // 初期化
    void Initialize(b2World* world, float x, float y);

    // 更新
    void Update();

    // 描画
    void Draw();

    // 終了処理
    void Finalize();


    b2Body* GetBody()
    {
        return m_body;
    }


    int GetTouchStageSelectNum(void)
    {
        return touch_stage_select_number;
    }


    void SetTouchStageSelectNum(int num)
    {
        touch_stage_select_number = num;
    }

    b2Body* GetTouchStagePointBody()
    {
        return m_stage_point_body;
    }


    void SetTouchStagePointBody(b2Body* body)
    {
        m_stage_point_body = body;
    }

private:
    b2Body* m_body;           // プレイヤーのBox2Dボディ
    float m_size;             // プレイヤーのサイズ
    b2World* m_world;         // Box2Dワールド（参照用）

    b2Body* m_stage_point_body;


    int touch_stage_select_number;
};


#endif // !STAGE_SELECT_PLAYER_H

