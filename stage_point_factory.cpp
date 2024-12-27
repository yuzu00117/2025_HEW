//-----------------------------------------------------------------------------------------------------
// #name stage_point_factory.cpp
// #description ステージポイントのファクトリー
// #make 2024/12/27　　永野義也
// #update 2024/12/27
// #comment 追加・修正予定
//                      ・特になし
//           
//----------------------------------------------------------------------------------------------------

#include "stage_point_factory.h"

StagePointFactory::StagePointFactory() {}

StagePointFactory::~StagePointFactory() {
    Finalize();
}

void StagePointFactory::CreateStagePoint(b2World* world, float x, float y, float size,int id) {
    StagePoint point;
    point.Initialize(world, x, y, size,id);
    m_stagePoints.push_back(std::move(point)); // リストに追加
}

void StagePointFactory::Update() {
    for (auto& point : m_stagePoints) {
        point.Update();
    }
}

void StagePointFactory::Draw() {
    for (auto& point : m_stagePoints) {
        point.Draw();
    }
}

void StagePointFactory::Finalize() {
    for (auto& point : m_stagePoints) {
        point.Finalize();
    }
    m_stagePoints.clear();
}

bool StagePointFactory::CheckCollisions(b2Body* playerBody) {
    for (auto& point : m_stagePoints) {
        if (point.IsPlayerColliding(playerBody)) {
            return true; // 衝突している場合は true を返す
        }
    }
    return false;
}

//StagePoint *StagePointFactory::FindPointID(int id)
//{
//    for (const auto& point : m_stagePoints) { // unique_ptr なので参照でループ
//        if (point.GetID() == id) { // ポインタ経由でアクセス
//            return point;    // 生ポインタを返す
//        }
//    }
//    return nullptr; // 見つからない場合は nullptr を返す
//}