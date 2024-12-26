#ifndef STAGE_SELECT_PLAYER_H
#define STAGE_SELECT_PLAYER_H
#include"include/box2d/box2d.h"

#include <memory>

class StageSelectPlayer {
public:
    StageSelectPlayer();  // �R���X�g���N�^
    ~StageSelectPlayer(); // �f�X�g���N�^


    // �V���O���g���̃C���X�^���X�擾
    static StageSelectPlayer& GetInstance() {
        static StageSelectPlayer instance; // �ÓI�C���X�^���X
        return instance;
    }

    // ������
    void Initialize(b2World* world, float x, float y);

    // �X�V
    void Update();

    // �`��
    void Draw();

    // �I������
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
    b2Body* m_body;           // �v���C���[��Box2D�{�f�B
    float m_size;             // �v���C���[�̃T�C�Y
    b2World* m_world;         // Box2D���[���h�i�Q�Ɨp�j

    b2Body* m_stage_point_body;


    int touch_stage_select_number;
};


#endif // !STAGE_SELECT_PLAYER_H

