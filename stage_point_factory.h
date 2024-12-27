//-----------------------------------------------------------------------------------------------------
// #name stage_point_factory.h
// #description ステージポイントのファクトリークラス
// #make 2024/12/27　　永野義也
// #update 2024/12/27
// #comment 追加・修正予定
//                      ・特になし
//           
//----------------------------------------------------------------------------------------------------

#ifndef STAGE_POINT_FACTORY_H
#define STAGE_POINT_FACTORY_H
#include <vector>
#include"include/box2d/box2d.h"
#include"stage_point.h"

class StagePointFactory {
public:
    StagePointFactory();
    ~StagePointFactory();

    // ステージポイントの生成
    void CreateStagePoint(b2World* world, float x, float y, float size,int id);

    // 更新処理
    void Update();

    // 描画処理
    void Draw();

    // 終了処理
    void Finalize();

    //idの検査
    StagePoint* FindPointID(int id);

    // 衝突チェック（プレイヤーとステージポイント）
    bool CheckCollisions(b2Body* playerBody);

private:
    std::vector<StagePoint> m_stagePoints; // ステージポイントのリスト
};

#endif // !STAGE_POINT_FACTORY_H
