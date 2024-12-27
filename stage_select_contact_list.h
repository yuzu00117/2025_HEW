//-----------------------------------------------------------------------------------------------------
// #name stage_select_contact_list.h
// #description �X�e�[�W�Z���N�g�̂����蔻��ɂ�鏈�����Ǘ����Ă���
// #make 2024/12/27�@�@�i��`��
// #update 2024/12/27
// #comment �ǉ��E�C���\��
//                      �E�̂̂���̏オ���Ă��瑫������
//           
//----------------------------------------------------------------------------------------------------

#ifndef STAGE_SELECT_CONTACT_LIST_H
#define STAGE_SELECT_CONTACT_LIST_H
#include"include/box2d/box2d.h"
#include"stage_point.h"
#include"stage_point_factory.h"
#include"stage_select_player.h"
#include"collider_type.h"

class StageSelectContactListener :public b2ContactListener
{
public:

	static StageSelectContactListener& GetInstance()
	{
		static StageSelectContactListener instance;
		return instance;
	}


    void BeginContact(b2Contact* contact) {
        // �Փ˂���2�̃t�B�N�X�`�����擾
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        // �Փ˂����{�f�B���擾
        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();

        // ���ꂼ��̃{�f�B���烆�[�U�[�f�[�^���擾
          // �{�f�B�̃��[�U�[�f�[�^���擾
        auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);
        auto* objectB = reinterpret_cast<ObjectData*>(fixtureB->GetUserData().pointer);
       

        if ((objectA->collider_type == collider_stage_select_player && objectB->collider_type == collider_stage_select_point) ||
            (objectA->collider_type == collider_stage_select_point && objectB->collider_type == collider_stage_select_player))
        {

            StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();
            //�ǂ������X�e�[�W�|�C���g
            if (objectA->collider_type == collider_stage_select_point)
            {
           
                switch (objectA->id)
                {

                case 1:
                    m_player.SetTouchStageSelectNum(1);
                    break;
                case 2:
                    m_player.SetTouchStageSelectNum(2);
                    break;
                case 3:
                    m_player.SetTouchStageSelectNum(3);
                    break;
                default:
                    break;
                }
                m_player.SetTouchStagePointBody(bodyA);
            }
            else
            {
                switch (objectB->id)
                {
                case 1:
                    m_player.SetTouchStageSelectNum(1);
                    break;
                case 2:
                    m_player.SetTouchStageSelectNum(2);
                    break;
                case 3:
                    m_player.SetTouchStageSelectNum(3);
                    break;
                default:
                    break;
                }
                m_player.SetTouchStagePointBody(bodyB);
            }
        }
    }



    void EndContact(b2Contact* contact) {
        // �Փ˂���2�̃t�B�N�X�`�����擾
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        // �Փ˂����{�f�B���擾
        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();

        // ���ꂼ��̃{�f�B���烆�[�U�[�f�[�^���擾
          // �{�f�B�̃��[�U�[�f�[�^���擾
        auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);
        auto* objectB = reinterpret_cast<ObjectData*>(fixtureB->GetUserData().pointer);


        if ((objectA->collider_type == collider_stage_select_player && objectB->collider_type == collider_stage_select_point) ||
            (objectA->collider_type == collider_stage_select_point && objectB->collider_type == collider_stage_select_player))
        {

            StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();
            //�ǂ������X�e�[�W�|�C���g
            if (objectA->collider_type == collider_stage_select_point)
            {
                m_player.SetTouchStageSelectNum(0);
            }
            else
            {
                m_player.SetTouchStageSelectNum(0);
            }

        }

    }
};

#endif // !STAGE_SELECT_CONTACT_LIST_H
