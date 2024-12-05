//-----------------------------------------------------------------------------------------------------
// #name contactlist.h
// #description 衝突時の処理を管理する
// #make 2024/11/22　永野義也
// #update 2024/11/22
// #comment 追加・修正予定
//          ・衝突時や衝突終了じなどの処理を書き込む
// 
// 
// 　　　＊注意　！
// 　　　　ここで、物体を直接けしたりジョイントしたりしないで！
//         理由としてはBox2dないのStepでここが呼ばれる部分で、物理演算中に物体が消えたり出現したりしたら正確な処理ができないから　エラーはく
// 
// 
//----------------------------------------------------------------------------------------------------

#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include"include/box2d/box2d.h"
#include"collider_type.h"
#include"anchor_point.h"
#include"anchor.h"
#include"player.h"
#include"object_manager.h"



class MyContactListener : public b2ContactListener {
private:
    Player player = Player::GetInstance();

public:
    b2Vec2 contactPoint;//衝突した地点
   

    // シングルトンのインスタンスを取得する
    static MyContactListener& GetInstance() {
        static MyContactListener instance;
        return instance;
    }

  

  
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------// 
//               衝突開始時
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

    // 衝突した瞬間
    void BeginContact(b2Contact* contact) override {

        ObjectManager& object_manager = ObjectManager::GetInstance();

        // 衝突したフィクスチャを取得
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();
        if (!fixtureA || !fixtureB) return; // NULLチェック
       


       
        // それぞれのボディからユーザーデータを取得
          // ボディのユーザーデータを取得
        auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);
        auto* objectB = reinterpret_cast<ObjectData*>(fixtureB->GetUserData().pointer);
        if (!objectA || !objectB)return;//NULLチェック

    

        // プレーヤーと地面が衝突したかを判定
        if ((objectA->collider_type == collider_player && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_player)) {
            // 衝突処理（プレーヤーと地面が接触した時）
            
            player.SetIsJumping(false);


        }


        //プレイヤーに付属しているセンサーとアンカーポイントが触れた場合
        if ((objectA->collider_type == collider_player_sensor && objectB->collider_type == collider_anchor_point) ||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_player_sensor))
        {
            //ぶつかったオブジェクトがどっちがアンカーポイントかわからないので比較する

            b2Body* anchor_point_body;//一時的な入れ物 この中にアンカーポイントのボディをいれる

            if (objectA->collider_type == collider_anchor_point)
            {
                //objectAがアンカーポイントだった
                //のでfixtureAがフィクスチャだよね
                anchor_point_body=fixtureA->GetBody();
            }
            else
            {
                //objectBがアンカーポイントだった
                //のでfixtureBがフィクスチャだよね
                anchor_point_body = fixtureB->GetBody();
            }
            //どちらがアンカーポイントか分かったのでアンカーポイントを保持しておく配列にいれる
            AnchorPoint::InsideSensor(anchor_point_body);
           
        }
        //プレイヤーに付属しているセンサーとアンカーポイントが触れた場合
        if ((objectA->collider_type == collider_anchor && objectB->collider_type == collider_anchor_point) ||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_anchor))
        {


            Anchor::SetAnchorState(Connected_state);//プレイヤーアップデートの中のスイッチ文の移行よう 接続状態に移行

            // 接触位置を取得
            b2WorldManifold worldManifold;
            contact->GetWorldManifold(&worldManifold);
            contactPoint = worldManifold.points[0];

            //木のオブジェクトの引っ張る処理
            if (objectA->object_name == Object_Wood || objectB->object_name == Object_Wood)
            {
                //どちらが木のオブジェクトか特定
                if (objectA->object_name == Object_Wood)//Aが木のオブジェクト
                {
                    wood*wood_instance=object_manager.FindWoodByID(objectA->id);//woodで同じIDのを探してインスタンスをもらう
                    wood_instance->Pulling_wood(objectA->add_force);//木を引っ張る処理を呼び出す
                }
                else
                {
                    wood* wood_instance = object_manager.FindWoodByID(objectB->id);
                    wood_instance->Pulling_wood(objectB->add_force);
                }

            }
        }

     
    }
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------// 
//               衝突終了時
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
    void EndContact(b2Contact* contact) override {
        // 衝突したフィクスチャを取得
        // 衝突したフィクスチャを取得
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();
        if (!fixtureA || !fixtureB) return; // NULLチェック


        // それぞれのボディからユーザーデータを取得
          // ボディのユーザーデータを取得
        auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);
        auto* objectB = reinterpret_cast<ObjectData*>(fixtureB->GetUserData().pointer);
        if (!objectA || !objectB)return;//NULLチェック

        // プレーヤーと地面が衝突したかを判定
        if ((objectA->collider_type == collider_player && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_player)) {
            // 衝突処理（プレーヤーと地面が接触した時）


            // 必要な処理をここに記述
        }



        //プレイヤーに付属しているセンサーとアンカーポイントが触れ終わった　（センサーの範囲外にでた）
        if ((objectA->collider_type == collider_player_sensor && objectB->collider_type == collider_anchor_point) ||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_player_sensor))
        {
            //ぶつかったオブジェクトがどっちがアンカーポイントかわからないので比較する

            b2Body* anchor_point_body;//一時的な入れ物 この中にアンカーポイントのボディをいれる

            if (objectA->collider_type == collider_anchor_point)
            {
                //objectAがアンカーポイントだった
                //のでfixtureAがフィクスチャだよね
                anchor_point_body = fixtureA->GetBody();
            }
            else
            {
                //objectBがアンカーポイントだった
                //のでfixtureBがフィクスチャだよね
                anchor_point_body = fixtureB->GetBody();
            }
            //どちらがアンカーポイントか分かったのでアンカーポイントを保持しておく配列にいれる
            AnchorPoint::OutsideSensor(anchor_point_body);

        }
    }

};


#endif // !CONTACTLIST_H