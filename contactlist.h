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
#include"enemy_static.h"
#include"enemy_dynamic.h"



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


        //プレイヤーとアンカーが触れた
        if ((objectA->collider_type == collider_player && objectB->collider_type == collider_anchor) ||
            (objectA->collider_type == collider_anchor && objectB->collider_type == collider_player))
        {
            if (Anchor::GetAnchorState() == Pulling_state)
            {
                Anchor::SetAnchorState(Deleting_state);
            }


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

            //状態が投げてる時にのみ以降する
            if (Anchor::GetAnchorState() == Throwing_state)
            {
                Anchor::SetAnchorState(Connected_state);//プレイヤーアップデートの中のスイッチ文の移行よう 接続状態に移行
            }
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
                    wood* wood_instance = object_manager.FindWoodByID(objectA->id);//woodで同じIDのを探してインスタンスをもらう
                    wood_instance->Pulling_wood(objectA->add_force);//木を引っ張る処理を呼び出す
                }
                else
                {
                    wood* wood_instance = object_manager.FindWoodByID(objectB->id);
                    wood_instance->Pulling_wood(objectB->add_force);
                }

            }

            //岩のオブジェクトの引っ張る処理
            if (objectA->object_name == Object_Rock || objectB->object_name == Object_Rock)
            {
                //どちらが岩のオブジェクトか特定
                if (objectA->object_name == Object_Rock)//Aが岩のオブジェクト
                {
                    rock* rock_instance = object_manager.FindRockByID(objectA->id);//woodで同じIDのを探してインスタンスをもらう
                    rock_instance->Pulling_rock(objectA->add_force);//木を引っ張る処理を呼び出す
                }
                else
                {
                    rock* rock_instance = object_manager.FindRockByID(objectB->id);//woodで同じIDのを探してインスタンスをもらう
                    rock_instance->Pulling_rock(objectB->add_force);//木を引っ張る処理を呼び出す
                }

            }


            //静的動的のオブジェクトの
            if (objectA->object_name == Object_Static_to_Dynamic || objectB->object_name == Object_Static_to_Dynamic)
            {
                //どちらが岩のオブジェクトか特定
                if (objectA->object_name == Object_Static_to_Dynamic)//Aが静的動的のオブジェクト
                {
                    static_to_dynamic_block* static_to_dynamic_block_instance = object_manager.FindStatic_to_Dynamic_BlcokID(objectA->id);//woodで同じIDのを探してインスタンスをもらう
                    static_to_dynamic_block_instance->Change_dynamic();//静的を動的にする
                }
                else
                {
                    static_to_dynamic_block* static_to_dynamic_block_instance = object_manager.FindStatic_to_Dynamic_BlcokID(objectB->id);//woodで同じIDのを探してインスタンスをもらう
                    static_to_dynamic_block_instance->Change_dynamic();//静的を動的にする
                }
            }
       
             
        }

        //静的プレイヤーとエネミーの衝突
        if ((objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_player) ||
            (objectA->collider_type == collider_player && objectB->collider_type == collider_enemy_static))
        {
            if (objectA->collider_type == collider_enemy_static)
            {
                EnemyStatic::CollisionPlayer(fixtureA->GetBody());
            }
            else if (objectB->collider_type == collider_enemy_static)
            {
                EnemyStatic::CollisionPlayer(fixtureB->GetBody());
            }
        }

        //動的プレイヤーとエネミーの衝突
        if ((objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_player) ||
            (objectA->collider_type == collider_player && objectB->collider_type == collider_enemy_dynamic))
        {
            if (objectA->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic::CollisionPlayer(fixtureA->GetBody());
            }
            else if (objectB->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic::CollisionPlayer(fixtureB->GetBody());
            }
        }

        //引っ張られている状態のオブジェクトとエネミーの衝突
        if (((objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_wall) ||
            (objectA->collider_type == collider_wall && objectB->collider_type == collider_enemy_static)) &&
            (Anchor::GetAnchorState() == Pulling_state))
        {
            if (objectA->collider_type == collider_enemy_static)
            {
                EnemyStatic::CollisionPulledObject(fixtureA->GetBody());
            }
            else if (objectB->collider_type == collider_enemy_static)
            {
                EnemyStatic::CollisionPulledObject(fixtureB->GetBody());
            }
        }

        //引っ張られている状態のオブジェクトと動的エネミーの衝突
        if (((objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_wall) ||
            (objectA->collider_type == collider_wall && objectB->collider_type == collider_enemy_dynamic)) &&
            (Anchor::GetAnchorState() == Pulling_state))
        {
            if (objectA->collider_type == collider_enemy_dynamic && Anchor::GetAnchorState)
            {
                EnemyDynamic::CollisionPulledObject(fixtureA->GetBody());
            }
            else if (objectB->collider_type == collider_enemy_dynamic && Anchor::GetAnchorState)
            {
                EnemyDynamic::CollisionPulledObject(fixtureB->GetBody());
            }
        }

        //引っ張られている状態のオブジェクトとエネミーの衝突
        if (((objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_anchor_point) ||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_enemy_static)) &&
            (Anchor::GetAnchorState() == Pulling_state))
        {
            if (objectA->collider_type == collider_enemy_static)
            {
                EnemyStatic::CollisionPulledObject(fixtureA->GetBody());
            }
            else if (objectB->collider_type == collider_enemy_static)
            {
                EnemyStatic::CollisionPulledObject(fixtureB->GetBody());
            }
        }

        //引っ張られている状態のオブジェクトと動的エネミーの衝突
        if (((objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_anchor_point) ||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_enemy_dynamic)) &&
            (Anchor::GetAnchorState() == Pulling_state))
        {
            if (objectA->collider_type == collider_enemy_dynamic && Anchor::GetAnchorState)
            {
                EnemyDynamic::CollisionPulledObject(fixtureA->GetBody());
            }
            else if (objectB->collider_type == collider_enemy_dynamic && Anchor::GetAnchorState)
            {
                EnemyDynamic::CollisionPulledObject(fixtureB->GetBody());
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