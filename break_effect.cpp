#include"break_effect.h"

// シングルトンのインスタンス
PillarFragmentsManager* PillarFragmentsManager::instance = nullptr;

// Fragment クラスの実装
Fragment::Fragment(b2Body* b, ID3D11ShaderResourceView* tex, int idx)
    : body(b), texture(tex), index(idx), creationTime(std::chrono::steady_clock::now()) {}

bool Fragment::ShouldDestroy() {
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now() - creationTime)
        .count() >= 5;
}

// PillarFragmentsManager の実装
PillarFragmentsManager::PillarFragmentsManager()
    : world(nullptr), splittingX(3), splittingY(3) {}

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

            fragment->SetLinearVelocity(b2Vec2(velocity.x * 2, velocity.y * 2));
            fragment->SetAngularVelocity(angularVelocity);

            b2PolygonShape fragmentShape;
            fragmentShape.SetAsBox(size.x / (2.0f * splittingX), size.y / (2.0f * splittingY));

            b2FixtureDef fragmentFixture;
            fragmentFixture.shape = &fragmentShape;
            fragmentFixture.density = 1.0f;
            fragmentFixture.friction = 0.5f;
            fragmentFixture.restitution = 0.0f;

            fragment->CreateFixture(&fragmentFixture);

            fragments.emplace_back(fragment, texture, index++);
        }
    }
}

void PillarFragmentsManager::UpdateFragments() {
    fragments.erase(std::remove_if(fragments.begin(), fragments.end(),
        [&](Fragment& fragment) {
            if (fragment.ShouldDestroy()) {
                world->DestroyBody(fragment.body);
                return true;
            }
            return false;
        }), fragments.end());
}

void PillarFragmentsManager::RenderFragments(ID3D11DeviceContext* context) {
    b2Vec2 playerPos = PlayerPosition::GetPlayerPosition();

    float scale = SCREEN_SCALE;
    b2Vec2 screen_center = { SCREEN_CENTER_X, SCREEN_CENTER_Y };

    for (const auto& fragment : fragments) {
        if (!fragment.body || !fragment.texture) continue;

        b2Vec2 bodyPos = fragment.body->GetPosition();

        float body_draw_x = ((bodyPos.x - playerPos.x) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.x;
        float body_draw_y = ((bodyPos.y - playerPos.y) * BOX2D_SCALE_MANAGEMENT) * scale + screen_center.y;

        context->PSSetShaderResources(0, 1, &fragment.texture);

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
