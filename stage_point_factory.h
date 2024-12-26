#ifndef STAGE_POINT_FACTORY_H
#define STAGE_POINT_FACTORY_H
#include <vector>
#include"include/box2d/box2d.h"
#include"stage_point.h"

class StagePointFactory {
public:
    StagePointFactory();
    ~StagePointFactory();

    // �X�e�[�W�|�C���g�̐���
    void CreateStagePoint(b2World* world, float x, float y, float size,int id);

    // �X�V����
    void Update();

    // �`�揈��
    void Draw();

    // �I������
    void Finalize();

    //id�̌���
    StagePoint* FindPointID(int id);

    // �Փ˃`�F�b�N�i�v���C���[�ƃX�e�[�W�|�C���g�j
    bool CheckCollisions(b2Body* playerBody);

private:
    std::vector<StagePoint> m_stagePoints; // �X�e�[�W�|�C���g�̃��X�g
};

#endif // !STAGE_POINT_FACTORY_H
