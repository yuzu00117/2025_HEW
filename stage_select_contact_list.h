//-----------------------------------------------------------------------------------------------------
// #name stage_select_contact_list.h
// #description ステージセレクトのあたり判定による処理を管理している
// #make 2024/12/27　　永野義也
// #update 2024/12/27
// #comment 追加・修正予定
//                      ・ののさんの上がってから足すかも
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
        // 衝突した2つのフィクスチャを取得
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        // 衝突したボディを取得
        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();

        // それぞれのボディからユーザーデータを取得
          // ボディのユーザーデータを取得
        auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);
        auto* objectB = reinterpret_cast<ObjectData*>(fixtureB->GetUserData().pointer);
       

        if ((objectA->collider_type == collider_stage_select_player && objectB->collider_type == collider_stage_select_point) ||
            (objectA->collider_type == collider_stage_select_point && objectB->collider_type == collider_stage_select_player))
        {

            StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();
            //どっちがステージポイント
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
        // 衝突した2つのフィクスチャを取得
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        // 衝突したボディを取得
        b2Body* bodyA = fixtureA->GetBody();
        b2Body* bodyB = fixtureB->GetBody();

        // それぞれのボディからユーザーデータを取得
          // ボディのユーザーデータを取得
        auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);
        auto* objectB = reinterpret_cast<ObjectData*>(fixtureB->GetUserData().pointer);


        if ((objectA->collider_type == collider_stage_select_player && objectB->collider_type == collider_stage_select_point) ||
            (objectA->collider_type == collider_stage_select_point && objectB->collider_type == collider_stage_select_player))
        {

            StageSelectPlayer& m_player = StageSelectPlayer::GetInstance();
            //どっちがステージポイント
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
