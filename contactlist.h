//-----------------------------------------------------------------------------------------------------
// #name contactlist.h
// #description 衝突時の処理を管理する
// #make 2024/11/22　永野義也
// #update 2025/1/10
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
#include"Item_Manager.h"
#include"FixtureSizeCalculate.h"



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
        ItemManager& item_manager = ItemManager::GetInstance();

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
        if ((objectA->collider_type == collider_player_leg && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_player_leg)||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_object)||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_player_leg)) {
            // 衝突処理（プレーヤーと地面が接触した時）
            
            player.SetIsJumping(false);


        }



        //プレイヤーとアンカーが触れた
        if ((objectA->collider_type == collider_player_body && objectB->collider_type == collider_anchor) ||
            (objectA->collider_type == collider_anchor && objectB->collider_type == collider_player_body))
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
                if (fixtureA->GetBody() == AnchorPoint::GetTargetAnchorPointBody() || fixtureB->GetBody() == AnchorPoint::GetTargetAnchorPointBody())//ぶつかった物体のどちらかが　ターゲットとしたアンカーポイントである
                {
                    Anchor::SetAnchorState(Connected_state);//プレイヤーアップデートの中のスイッチ文の移行よう 接続状態に移行
                }
                else
                {
                    return;//ぶつかったアンカーポイントがターゲットとしたアンカーポイントではない場合
                }
          
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

            //引っ張れる床のオブジェクトの引っ張る処理
            if ((objectA->object_name == Object_Movable_Ground && objectB->collider_type == collider_anchor) ||
                (objectA->collider_type == collider_anchor && objectB->object_name == Object_Movable_Ground))
            {
                //どちらが床のオブジェクトか特定
                if (objectA->object_name == Object_Movable_Ground)//Aが岩のオブジェクト
                {
                    movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectA->id);//movable_groundで同じIDのを探してインスタンスをもらう
                    ground_instance->Pulling_ground(objectA->add_force);
                    ground_instance->SetIfPulling(true);
                }
                else
                {
                    movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectB->id);//movable_groundで同じIDのを探してインスタンスをもらう
                    ground_instance->Pulling_ground(objectB->add_force);
                    ground_instance->SetIfPulling(true);
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

        //プレイヤーと静的エネミーの衝突
        if ((objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_enemy_static)||
            (objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_enemy_static))
        {
            if (objectA->collider_type == collider_enemy_static)
            {
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectA->id);
                enemy_instance->CollisionPlayer();
            }
            else if (objectB->collider_type == collider_enemy_static)
            {
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectB->id);
                enemy_instance->CollisionPlayer();
            }
        }

        //プレイヤーと動的エネミーの衝突
        if ((objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_enemy_dynamic)||
            (objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_enemy_dynamic))
        {
            if (objectA->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                enemy_instance->CollisionPlayer();
            }
            else if (objectB->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                enemy_instance->CollisionPlayer();
            }
        }

        //プレイヤーと浮遊エネミーの衝突
        if ((objectA->collider_type == collider_enemy_floating && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_enemy_floating) ||
            (objectA->collider_type == collider_enemy_floating && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_enemy_floating))
        {
            if (objectA->collider_type == collider_enemy_floating)
            {
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectA->id);
                enemy_instance->CollisionPlayer();
            }
            else if (objectB->collider_type == collider_enemy_floating)
            {
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectB->id);
                enemy_instance->CollisionPlayer();
            }
        }

        //引っ張れれている状態の床と静的エネミーの衝突
        if ((objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_object_destroyer_of_enemy) ||
            (objectA->collider_type == collider_object_destroyer_of_enemy && objectB->collider_type == collider_enemy_static))
        {

            if (objectA->collider_type == collider_enemy_static)
            {
                auto ground_instance = object_manager.FindMovable_GroundID(objectB->id);//movable_groundで同じIDのを探してインスタンスをもらう
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectA->id);
                ground_instance->AddContactedEnemyList(enemy_instance);
            }
            else if (objectB->collider_type == collider_enemy_static)
            {
                auto ground_instance = object_manager.FindMovable_GroundID(objectA->id);//movable_groundで同じIDのを探してインスタンスをもらう
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectB->id);
                ground_instance->AddContactedEnemyList(enemy_instance);
            }
        }

        //引っ張れれている状態の床と動的エネミーの衝突
        if ((objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_object_destroyer_of_enemy) ||
            (objectA->collider_type == collider_object_destroyer_of_enemy && objectB->collider_type == collider_enemy_dynamic))
        {
            if ((objectA->collider_type == collider_enemy_dynamic) &&
                (fixtureB->GetBody()->GetLinearVelocity() != b2Vec2(0.0, 0.0)))
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectB->id);//movable_groundで同じIDのを探してインスタンスをもらう
                //床が静止状態の場合
                if (ground_instance->GetObjectGroundBody()->GetLinearVelocity().x == 0.0f || ground_instance->GetIfPulling() == false) {
                    return;
                }
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                ground_instance->AddContactedEnemyList(enemy_instance);
            }
            else if ((objectB->collider_type == collider_enemy_dynamic) &&
                (fixtureA->GetBody()->GetLinearVelocity() != b2Vec2(0.0, 0.0)))
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectA->id);//movable_groundで同じIDのを探してインスタンスをもらう
                //床が静止状態の場合
                if (ground_instance->GetObjectGroundBody()->GetLinearVelocity().x == 0.0f || ground_instance->GetIfPulling() == false) {
                    return;
                }
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                ground_instance->AddContactedEnemyList(enemy_instance);
            }

        }
        
        //引っ張れれている状態の床と浮遊エネミーの衝突
        if ((objectA->collider_type == collider_enemy_floating && objectB->collider_type == collider_object_destroyer_of_enemy) ||
            (objectA->collider_type == collider_object_destroyer_of_enemy && objectB->collider_type == collider_enemy_floating))
        {
            if ((objectA->collider_type == collider_enemy_floating) &&
                (fixtureB->GetBody()->GetLinearVelocity() != b2Vec2(0.0, 0.0)))
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectB->id);//movable_groundで同じIDのを探してインスタンスをもらう
                //床が静止状態の場合
                if (ground_instance->GetObjectGroundBody()->GetLinearVelocity().x == 0.0f || ground_instance->GetIfPulling() == false) {
                    return;
                }
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectA->id);
                ground_instance->AddContactedEnemyList(enemy_instance);
            }
            else if ((objectB->collider_type == collider_enemy_floating) &&
                (fixtureA->GetBody()->GetLinearVelocity() != b2Vec2(0.0, 0.0)))
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectA->id);//movable_groundで同じIDのを探してインスタンスをもらう
                //床が静止状態の場合
                if (ground_instance->GetObjectGroundBody()->GetLinearVelocity().x == 0.0f || ground_instance->GetIfPulling() == false) {
                    return;
                }
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectB->id);
                ground_instance->AddContactedEnemyList(enemy_instance);
            }

        }


        //引っ張られている状態のオブジェクトとエネミーの衝突
        if ((objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_enemy_static) ||
            (objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_anchor_point)||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_enemy_static))
        {
            if ((objectA->collider_type == collider_enemy_static) &&
                (fixtureB->GetBody()->GetLinearVelocity() != b2Vec2(0.0, 0.0)))
            {
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectA->id);
                enemy_instance->CollisionPulledObject();
            }
            else if ((objectB->collider_type == collider_enemy_static) &&
                (fixtureA->GetBody()->GetLinearVelocity() != b2Vec2(0.0, 0.0)))
            {
                    EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectB->id);
                    enemy_instance->CollisionPulledObject();
            }
        }

        //引っ張られている状態のオブジェクトと動的エネミーの衝突
        if ((objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_enemy_dynamic) ||
            (objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_anchor_point) ||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_enemy_dynamic))
        {

            if ((objectA->collider_type == collider_enemy_dynamic) &&
                (fixtureB->GetBody()->GetLinearVelocity() != b2Vec2(0.0, 0.0)))
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                enemy_instance->CollisionPulledObject();
            }
            else if ((objectB->collider_type == collider_enemy_dynamic) &&
                (fixtureA->GetBody()->GetLinearVelocity() != b2Vec2(0.0, 0.0)))
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                enemy_instance->CollisionPulledObject();
            }
        }

        //引っ張られている状態のオブジェクトと浮遊エネミーの衝突
        if ((objectA->collider_type == collider_enemy_floating && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_enemy_floating))
        {

            if ((objectA->collider_type == collider_enemy_floating) &&
                (fixtureB->GetBody()->GetLinearVelocity() != b2Vec2(0.0, 0.0)))
            {
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectA->id);
                enemy_instance->CollisionPulledObject();
            }
            else if ((objectB->collider_type == collider_enemy_floating) &&
                (fixtureA->GetBody()->GetLinearVelocity() != b2Vec2(0.0, 0.0)))
            {
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectB->id);
                enemy_instance->CollisionPulledObject();
            }
        }


     
        //プレイヤーに付属しているセンサーと静的エネミーが触れた場合
        if ((objectA->collider_type == collider_player_sensor && objectB->collider_type == collider_enemy_static) ||
            (objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_player_sensor))
        {
            if (objectA->collider_type == collider_enemy_static)
            {
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectA->id);
                enemy_instance->InPlayerSensor();

            }
            else if (objectB->collider_type == collider_enemy_static)
            {
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectB->id);
                enemy_instance->InPlayerSensor();
            }
        }

        //プレイヤーに付属しているセンサーと動的エネミーが触れた場合
        if ((objectA->collider_type == collider_player_sensor && objectB->collider_type == collider_enemy_dynamic) ||
            (objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_player_sensor))
        {
            if (objectA->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                enemy_instance->InPlayerSensor();
            }
            else if (objectB->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                enemy_instance->InPlayerSensor();
            }
        }

        //プレイヤーに付属しているセンサーと浮遊エネミーが触れた場合(今のところ浮遊エネミーでこのセンサー判定使う予定まだないけど、一応)
        if ((objectA->collider_type == collider_player_sensor && objectB->collider_type == collider_enemy_floating) ||
            (objectA->collider_type == collider_enemy_floating && objectB->collider_type == collider_player_sensor))
        {
            if (objectA->collider_type == collider_enemy_floating)
            {
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectA->id);
                enemy_instance->InPlayerSensor();
            }
            else if (objectB->collider_type == collider_enemy_floating)
            {
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectB->id);
                enemy_instance->InPlayerSensor();
            }
        }
        //プレイヤーに付属しているセンサーと浮遊エネミーが触れた場合
        if ((objectA->collider_type == collider_player_body && objectB->collider_type == collider_enemy_floating_sensor) ||
            (objectA->collider_type == collider_enemy_floating_sensor && objectB->collider_type == collider_player_body))
        {
            if (objectA->collider_type == collider_enemy_floating_sensor)
            {
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectA->id);
                enemy_instance.SetIf
            }
            else if (objectB->collider_type == collider_enemy_floating_sensor)
            {
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectB->id);
                enemy_instance->InPlayerSensor();
            }
        }




        //ソウルアイテムがオブジェクトとぶつかったとき
        if ((objectA->Item_name == ITEM_SPIRIT && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->Item_name == ITEM_SPIRIT)||
            (objectA->Item_name == ITEM_SPIRIT && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->Item_name == ITEM_SPIRIT))
        {
            //ソウルとオブジェのボディやfixtureやオブジェクトデータの変数を準備
            //-------------------------------------------------------------------------
            ObjectData* object_data = objectA;
            b2Body* object = fixtureA->GetBody();
            b2Fixture* object_fixture = fixtureA;

            ObjectData* spirit_data = objectB;
            b2Fixture* spirit_fixture = fixtureB;

            if (objectA->Item_name == ITEM_SPIRIT) {
                spirit_data = objectA;
                spirit_fixture = fixtureA;

                object_data = objectB;
                object = fixtureB->GetBody();
                object_fixture = fixtureB;
            }
            ItemSpirit* spirit_instance = item_manager.FindItem_Spirit_ByID(spirit_data->id);//ItemSpeedUpで同じIDのを探してインスタンスをもらう
           
            // もし収集中の場合は衝突処理を無視
            //-------------------------------------------------------------------------
            if (spirit_instance->GetState() == Spirit_Collecting)
            {
                return;
            }

            // オブジェからソウルまでのベクトルを計算
            //-------------------------------------------------------------------------
            b2Vec2 object_position = object->GetPosition();
            b2Vec2 spirit_position = spirit_instance->GetBody()->GetPosition();
            b2Vec2 vec;
            vec.x = spirit_position.x - object_position.x;
            vec.y = spirit_position.y - object_position.y;
            
            //オブジェのfixtureの半径を取得
            b2Shape* const object_shape = object_fixture->GetShape();
            b2Vec2 object_half_size = GetFixtureHalfSize(object_shape);


          //条件別で違う処理をする（上昇するかどうか、地面に着いているかどうか）
         //-------------------------------------------------------------------------
            
            //ベクトルが縦幅より小さい時かつ、ソウルは上昇中ではない場合（つまり上昇していないソウルがオブジェの上に乗っている場合）
            //上昇中じゃないのを条件にしたのは、連続した複数のオブジェの中で上昇している時オブジェ間を入る離れる瞬間と本当に一番上のオブジェに乗る瞬間を誤認させないため
            if ((vec.y <= -object_half_size.y) && spirit_instance->GetState() != Spirit_Rising)
            {
                spirit_instance->SetState(Spirit_OnGround); //ソウルの状態が地面にいる（ソウルのグラビティが0になる）(ソウルが落ちなくなる)
            }
            //今衝突しているオブジェとさっきまで衝突していたオブジェが同じの場合、或いは地面に着いていない空中にいるソウルは、そのまま停止
            else if (spirit_instance->GetRecentCollidedObject() == object || spirit_instance->GetState() == Spirit_Falling)
            {
                spirit_instance->SetState(Spirit_OnGround); //ソウルの状態が地面にいる（ソウルのグラビティが0になる）(ソウルが落ちなくなる)
            }
            else
            {
                spirit_instance->SetRecentCollidedObject(object);   //今当たっているオブジェを更新
                spirit_instance->SetState(Spirit_Rising);   //ソウルの状態が上昇中
            }

        }

        // プレーヤーとアイテムが衝突したかを判定
        if ((objectA->collider_type == collider_player_body && objectB->collider_type == collider_item) ||
            (objectA->collider_type == collider_item && objectB->collider_type == collider_player_body)||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_item)||
            (objectA->collider_type == collider_item && objectB->collider_type == collider_player_leg)) {

            //最初に　objectB　が item　だと仮定する
            auto item = objectB;
            //どちらがアイテムか特定
            if (objectA->collider_type == collider_item)//Aがアイテム
            {
                item = objectA;
            }

            //アイテム種類別に処理する
            switch (item->Item_name)
            {
            case ITEM_SPEED_UP:
            {
                ItemSpeedUp* item_instance = item_manager.FindItem_SpeedUp_ByID(item->id);//ItemSpeedUpで同じIDのを探してインスタンスをもらう
                item_instance->Function();
                item_instance->SetDestory(true);//削除を呼び出す
            }
            break;
            case ITEM_SPIRIT:
            {
                ItemSpirit* spirit_instance = item_manager.FindItem_Spirit_ByID(item->id);//ItemSpeedUpで同じIDのを探してインスタンスをもらう
                //spirit_instance->SetIfCollidedPlayer(true);
                spirit_instance->Function();
                spirit_instance->SetState(Spirit_Destory);//削除を呼び出す

            }
            break;
            case ITEM_COIN:
            {
                ItemCoin* coin_instance = item_manager.FindItem_Coin_ByID(item->id);//ItemSpeedUpで同じIDのを探してインスタンスをもらう
                coin_instance->Function();
                coin_instance->SetDestory(true);//削除を呼び出す
            }
            break;

            }
            
        }
    }
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------// 
//               衝突終了時
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
    void EndContact(b2Contact* contact) override {
        ObjectManager& object_manager = ObjectManager::GetInstance();
        ItemManager& item_manager = ItemManager::GetInstance();

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
        if ((objectA->collider_type == collider_player_leg && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_player_leg)) {

            // 衝突処理（プレーヤーと地面が接触した時）


            // 必要な処理をここに記述
        }

        //引っ張れる床のオブジェクトとアンカーが離れた場合
        if ((objectA->object_name == Object_Movable_Ground && objectB->collider_type == collider_anchor) ||
            (objectA->collider_type == collider_anchor && objectB->object_name == Object_Movable_Ground))
        {
            //どちらが床のオブジェクトか特定
            if (objectA->object_name == Object_Movable_Ground)//Aが岩のオブジェクト
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectA->id);//movable_groundで同じIDのを探してインスタンスをもらう
                ground_instance->SetIfPulling(false);
            }
            else
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectB->id);//movable_groundで同じIDのを探してインスタンスをもらう
                ground_instance->SetIfPulling(false);
            }

        }        
        //引っ張れる床と敵が離れた場合
        if ((objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_object_destroyer_of_enemy) ||
            (objectA->collider_type == collider_object_destroyer_of_enemy && objectB->collider_type == collider_enemy_static))
        {

            if (objectA->collider_type == collider_enemy_static)
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectB->id);//movable_groundで同じIDのを探してインスタンスをもらう
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectA->id);
                ground_instance->DeleteContactedEnemyList(enemy_instance);
            }
            else if (objectB->collider_type == collider_enemy_static)
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectA->id);//movable_groundで同じIDのを探してインスタンスをもらう
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectB->id);
                ground_instance->DeleteContactedEnemyList(enemy_instance);
            }
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





        //プレイヤーに付属しているセンサーと静的エネミーが離れた場合
        if ((objectA->collider_type == collider_player_sensor && objectB->collider_type == collider_enemy_static) ||
            (objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_player_sensor))
        {
            if (objectA->collider_type == collider_enemy_static)
            {
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectA->id);
                if (enemy_instance)
                {
                    enemy_instance->OutPlayerSensor();
                }
            }
            else if (objectB->collider_type == collider_enemy_static)
            {
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectB->id);
                if (enemy_instance)
                {
                    enemy_instance->OutPlayerSensor();
                }
            }
        }

        //プレイヤーに付属しているセンサーと動的エネミーが離れた場合
        if ((objectA->collider_type == collider_player_sensor && objectB->collider_type == collider_enemy_dynamic) ||
            (objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_player_sensor))
        {
            if (objectA->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                enemy_instance->OutPlayerSensor();
            }
            else if (objectB->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                enemy_instance->OutPlayerSensor();
            }
        }

        //  ソウルアイテムがオブジェクトや床から離れた時
        if ((objectA->Item_name == ITEM_SPIRIT && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->Item_name == ITEM_SPIRIT) ||
            (objectA->Item_name == ITEM_SPIRIT && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->Item_name == ITEM_SPIRIT))
        {
            auto spirit = objectA->Item_name == ITEM_SPIRIT ? objectA : objectB;
            auto object = objectA->Item_name == ITEM_SPIRIT ? fixtureB : fixtureA;

            ItemSpirit* spirit_instance = item_manager.FindItem_Spirit_ByID(spirit->id);//ItemSpeedUpで同じIDのを探してインスタンスをもらう
          
            //ソウルが上昇中でかつ、ソウルの直近まで当たっているオブジェクトが今のオブジェクトの場合はまだ上昇終わった
            if (spirit_instance->GetState() == Spirit_Rising && spirit_instance->GetRecentCollidedObject() == object->GetBody())
            {
                spirit_instance->SetState(Spirit_Falling);//ソウルの状態が落下中
            }
            else if (spirit_instance->GetState() != Spirit_Rising && spirit_instance->GetState() != Spirit_Collecting)
            {
                spirit_instance->SetState(Spirit_OnGround);//ソウルの状態が床の上
            }

        }


    }


};


#endif // !CONTACTLIST_H
