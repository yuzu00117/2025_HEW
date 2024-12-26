#include "stage_point_factory.h"

StagePointFactory::StagePointFactory() {}

StagePointFactory::~StagePointFactory() {
    Finalize();
}

void StagePointFactory::CreateStagePoint(b2World* world, float x, float y, float size,int id) {
    StagePoint point;
    point.Initialize(world, x, y, size,id);
    m_stagePoints.push_back(std::move(point)); // ���X�g�ɒǉ�
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
            return true; // �Փ˂��Ă���ꍇ�� true ��Ԃ�
        }
    }
    return false;
}

//StagePoint *StagePointFactory::FindPointID(int id)
//{
//    for (const auto& point : m_stagePoints) { // unique_ptr �Ȃ̂ŎQ�ƂŃ��[�v
//        if (point.GetID() == id) { // �|�C���^�o�R�ŃA�N�Z�X
//            return point;    // ���|�C���^��Ԃ�
//        }
//    }
//    return nullptr; // ������Ȃ��ꍇ�� nullptr ��Ԃ�
//}