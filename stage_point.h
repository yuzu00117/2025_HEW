#ifndef STAGE_POINT_H
#define STAGE_POINT_H

#include "include/box2d/box2d.h"
#include"sprite.h"

class StagePoint {
public:
    StagePoint();
    ~StagePoint();

    // 初期化
    void Initialize(b2World* world, float x, float y, float size,int id);

    // 更新（必要なら）
    void Update();

    // 描画
    void Draw();

    // 衝突チェック用
    bool IsPlayerColliding(b2Body* playerBody);

    // 終了処理
    void Finalize();

    int GetID()const
    {
        return id;
    }

private:
    int id;
    b2Body* m_body; // ステージポイントのBox2Dボディ
    float m_size;   // ステージポイントのサイズ
    b2World* m_world; // 参照用
};

#endif // !STAGE_POINT_H
