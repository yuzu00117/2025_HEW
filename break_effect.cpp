// #name　break_effect.cpp
// #description テクスチャとボディサイズを渡すとバラバラにしてくれる　豪快だねー
// #make 2025/02/06
// #update 2025/02/06
// #comment 追加・修正予定  豪快な感じがする
//         
//          
//----------------------------------------------------------------------------------------------------

#include"break_effect.h"

// シングルトンのインスタンス
PillarFragmentsManager* PillarFragmentsManager::instance = nullptr;

// Fragment 
Fragment::Fragment(b2Body* b, ID3D11ShaderResourceView* tex, int idx)
    : body(b), texture(tex), index(idx), creationTime(std::chrono::steady_clock::now()) {}

//破壊するかどうか
bool Fragment::ShouldDestroy() {
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now() - creationTime)
        .count() >= 5;
}


PillarFragmentsManager::PillarFragmentsManager()
    : world(nullptr), splittingX(3), splittingY(3) {}

//みんな大好きシングルトン
PillarFragmentsManager& PillarFragmentsManager::GetInstance() {
    if (!instance) {
        instance = new PillarFragmentsManager();
    }
    return *instance;
}

void PillarFragmentsManager::Init(b2World* w, int sx, int sy) {
    world = w;
    splittingX = sx;
    splittingY = sy;
}

//ここにボディをとテクスチャとサイズをいれると
//ボディを削除したのち、複数のボディを作成してヴェロシティを加えてぼーん　豪快だね～
void PillarFragmentsManager::Destroy_Splitting(b2Body* targetBody, ID3D11ShaderResourceView* texture, b2Vec2 size) {
    if (!targetBody || !world) return;

    fragmentSize = size;

    b2Vec2 destroyPosition = targetBody->GetPosition();
    float angle = targetBody->GetAngle();
    b2Vec2 velocity = targetBody->GetLinearVelocity();
    float angularVelocity = targetBody->GetAngularVelocity();

    world->DestroyBody(targetBody);

    size.x /= BOX2D_SCALE_MANAGEMENT;
    size.y /= BOX2D_SCALE_MANAGEMENT;

    // 乱数生成のためのセットアップ
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> velocityDist(-5.0f, 5.0f); // -5.0f ～ 5.0f の範囲でランダム

    int index = 0;
    for (int y = 0; y < splittingY; y++) {
        for (int x = 0; x < splittingX; x++) {
            float localX = ((x - (splittingX - 1) / 2.0f) * size.x / splittingX);
            float localY = ((y - (splittingY - 1) / 2.0f) * size.y / splittingY);

            float rotatedX = localX * cos(angle) - localY * sin(angle);
            float rotatedY = localX * sin(angle) + localY * cos(angle);

            b2Vec2 fragmentPosition(destroyPosition.x + rotatedX, destroyPosition.y + rotatedY);

            b2BodyDef fragmentDef;
            fragmentDef.type = b2_dynamicBody;
            fragmentDef.position = fragmentPosition;
            fragmentDef.angle = angle;

            b2Body* fragment = world->CreateBody(&fragmentDef);

            // ランダムな方向の速度を追加
            float randomX = velocityDist(gen); // -5.0f ～ 5.0f のランダム値
            float randomY = velocityDist(gen);
            fragment->SetLinearVelocity(b2Vec2(velocity.x * 4 + randomX, velocity.y * 4 + randomY));

            // ランダムな回転速度を追加
            float randomAngularVelocity = velocityDist(gen);
            fragment->SetAngularVelocity(angularVelocity + randomAngularVelocity);

            b2PolygonShape fragmentShape;
            fragmentShape.SetAsBox(size.x / (2.0f * splittingX), size.y / (2.0f * splittingY));

            b2FixtureDef fragmentFixture;
            fragmentFixture.shape = &fragmentShape;
            fragmentFixture.density = 1.0f;
            fragmentFixture.friction = 0.5f;
            fragmentFixture.restitution = 0.0f;
            fragmentFixture.filter = createFilterExclude("texture_body_filter",
                { "texture_body_filter", "object_filter", "ground_filter", "enemy_filter", "Player_filter" });

            fragment->CreateFixture(&fragmentFixture);

            fragments.emplace_back(fragment, texture, index++);
        }
    }
}

//ボディを削除するまでの時間の管理
void PillarFragmentsManager::UpdateFragments() {
    if (!world) return;

    // まず削除対象のボディを全て削除する
    for (auto& fragment : fragments) {
        if (fragment.ShouldDestroy() && fragment.body) {
            world->DestroyBody(fragment.body);
            fragment.body = nullptr;
        }
    }

    // nullptr になったものを削除
    fragments.erase(std::remove_if(fragments.begin(), fragments.end(),
        [](const Fragment& fragment) {
            return fragment.body == nullptr;
        }), fragments.end());
}

//描画ー
void PillarFragmentsManager::DrawFragments() {
    b2Vec2 playerPos = PlayerPosition::GetPlayerPosition();

    float scale = SCREEN_SCALE;
    b2Vec2 screen_center = { SCREEN_CENTER_X, SCREEN_CENTER_Y };

    for (const auto& fragment : fragments) {
        if (!fragment.body || !fragment.texture) continue;

        b2Vec2 bodyPos = fragment.body->GetPosition();

        float body_draw_x = ((bodyPos.x - playerPos.x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
        float body_draw_y = ((bodyPos.y - playerPos.y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

        GetDeviceContext()->PSSetShaderResources(0, 1, &fragment.texture);

       
        DrawSplittingSprite(
            { body_draw_x, body_draw_y },
            fragment.body->GetAngle(),
            { fragmentSize.x / splittingX * scale, fragmentSize.y / splittingY * scale },
            splittingX,
            splittingY,
            fragment.index,
            1.0f
        );
    }
}


void PillarFragmentsManager::Finalize() {
    if (!world) return;  // world が nullptr なら何もしない

    // 全フラグメントのボディを削除
    for (auto& fragment : fragments) {
        if (fragment.body) {
            world->DestroyBody(fragment.body);
            fragment.body = nullptr;
        }
    }

    // フラグメントリストをクリア
    fragments.clear();
}
