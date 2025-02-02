//-----------------------------------------------------------------------------------------------------
// #name contactlist.h
// #description 衝突時の処理を管理する
// #make 2024/11/22　永野義也
// #update 2025/1/17
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
#include"tool.h"
#include"hit_stop.h"
#include"camera_shake.h"
#include"sound.h"
#include"1-1_boss.h"
#include"1_1_boss_pillar.h"

class MyContactListener : public b2ContactListener {
private:
    Player &player = Player::GetInstance();
    Boss_1_1& boss = Boss_1_1::GetInstance();


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
            (objectA->collider_type == collider_object && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_boss_field && objectB->collider_type == collider_player_leg)||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_boss_field)){
            // 衝突処理（プレーヤーと地面が接触した時）
            
            player.SetIsJumping(false);


        }

        // プレーヤーとテレポートブロックが衝突したかを判定
        if ((objectA->collider_type == collider_player_leg && objectB->collider_type == collider_teleport_block) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_teleport_block) ||
            (objectA->collider_type == collider_teleport_block && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_teleport_block && objectB->collider_type == collider_player_leg))
        {
            // 衝突処理（プレーヤーと地面が接触した時）

                     //どちらが木のオブジェクトか特定
            if (objectA->collider_type== collider_teleport_block)//Aが木のオブジェクト
            {
                teleport_block* teleport_block_instance = object_manager.FindTeleportBlock(objectA->id);
                teleport_block_instance->SetTeleportFlag(true);
            }
            else
            {
                teleport_block* teleport_block_instance = object_manager.FindTeleportBlock(objectB->id);
                teleport_block_instance->SetTeleportFlag(true);
            }

        }


        //プレイヤーと間欠泉の水が触れた場合
        if ((objectA->collider_type == collider_player_leg && objectB->collider_type == collider_geyser_water) ||
            (objectA->collider_type == collider_geyser_water && objectB->collider_type == collider_player_leg))
        {


            if (objectA->collider_type == collider_geyser_water)
            {
                geyser* geyser_instance = object_manager.FindGeyserID(objectA->id);//woodで同じIDのを探してインスタンスをもらう
                geyser_instance->SetFlag(true);//木を引っ張る処理を呼び出す
            }
            else
            {
                geyser* geyser_instance = object_manager.FindGeyserID(objectB->id);//woodで同じIDのを探してインスタンスをもらう
                geyser_instance->SetFlag(true);//木を引っ張る処理を呼び出す
            }
        }

        // プレーヤーとテレポートブロックが衝突したかを判定
        if ((objectA->collider_type == collider_player_leg && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_player_leg))
        {

            float object_velocity=0;
            if (objectA->collider_type == collider_object)
            {
                object_velocity=ReturnAbsoluteValue(fixtureA->GetBody()->GetLinearVelocity().x) + ReturnAbsoluteValue(fixtureA->GetBody()->GetLinearVelocity().y);
            }
            else
            {
                object_velocity=ReturnAbsoluteValue(fixtureB->GetBody()->GetLinearVelocity().x) + ReturnAbsoluteValue(fixtureB->GetBody()->GetLinearVelocity().y);
            }

            if (1.0f < object_velocity)//ここに入ったらオブジェクトが移動中であり、被弾判定してよい
            {
               
                player.Player_Damaged(-50, 120);//被弾処理

                if (objectA->collider_type == collider_object)
                {
                    player.Player_knockback(2, fixtureA->GetBody());
                }
                if (objectB->collider_type == collider_object)
                {
                    player.Player_knockback(2, fixtureB->GetBody());
                }
            }

        }



        //プレイヤーとアンカーが触れた
        if ((objectA->collider_type == collider_player_body && objectB->collider_type == collider_anchor) ||
            (objectA->collider_type == collider_anchor && objectB->collider_type == collider_player_body))
        {
            if (Anchor::GetAnchorState() == Pulling_state)
            {
                Anchor::SetAnchorState(Deleting_state);
            }

            if (Anchor::GetAnchorState() == Throwing_state)
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


            //状態が投げてる時にのみ以降する
            if (Anchor::GetAnchorState() == Connected_state)
            {
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

                //ボスのコア
                if (objectA->object_name == Boss_core || objectB->object_name == Boss_core)
                {
                    //どちらが岩のオブジェクトか特定
                    if (objectA->object_name == Boss_core)//Aが静的動的のオブジェクト
                    {
                        boss.BossDamaged();
                        boss.SetCoreDeleteFlag(true);

                    }
                    else
                    {
                        boss.BossDamaged();
                        boss.SetCoreDeleteFlag(true);

                    }
                }

                //ボスの部屋の柱
                if (objectA->object_name == Boss_pillar || objectB->object_name == Boss_pillar)
                {
                    //どちらがボスの部屋の柱
                    if (objectA->object_name == Boss_pillar)//Aが木のオブジェクト
                    {
                        boss_pillar* pillar_instance = object_manager.FindBossPillar(objectA->id);//woodで同じIDのを探してインスタンスをもらう
                        pillar_instance->Pulling_pillar(objectA->add_force);//木を引っ張る処理を呼び出す
                    }
                    else
                    {
                        boss_pillar* pillar_instance = object_manager.FindBossPillar(objectB->id);
                        pillar_instance->Pulling_pillar(objectB->add_force);
                    }
                }


                //ボスのオブジェクト
                if (objectA->object_name == Boss_Carry_Object_Enemy || objectB->object_name == Boss_Carry_Object_Enemy)
                {
                    //どちらがボスの部屋の柱
                    if (objectA->object_name == Boss_Carry_Object_Enemy)//Aが木のオブジェクト
                    {
                        boss_carry_object_enemy*enemy_instance= object_manager.FindBossCarryObjectEnemy(objectA->id);
                        enemy_instance->SetAnchorHItFlag(true);

                    }
                    else
                    {
                        boss_carry_object_enemy* enemy_instance = object_manager.FindBossCarryObjectEnemy(objectB->id);
                        enemy_instance->SetAnchorHItFlag(true);
                    }
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
            app_atomex_start(Player_Dead_Sound);
            HitStop::StartHitStop(15);
            CameraShake::StartCameraShake(5, 3, 15);
            player.Player_Damaged(-50, 120);

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

        //プレイヤーの通常攻撃攻撃と動的エネミーの衝突
        if ((objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_normal_attack_anchor) ||
            (objectA->collider_type == collider_normal_attack_anchor && objectB->collider_type == collider_enemy_dynamic))
        {
            if (objectA->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                enemy_instance->CollisionPulledObject();
            }
            else if (objectB->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                enemy_instance->CollisionPulledObject();
            }
        }

        //プレイヤーと浮遊エネミーの衝突
        if ((objectA->collider_type == collider_enemy_floating && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_enemy_floating) ||
            (objectA->collider_type == collider_enemy_floating && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_enemy_floating))
        {
            app_atomex_start(Player_Dead_Sound);
            HitStop::StartHitStop(15);
            CameraShake::StartCameraShake(5, 3, 15);
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
            (objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_anchor_point) ||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_enemy_static))
        {
            ObjectData* enemy_data = objectB;
            ObjectData* object_data = objectA;
            b2Fixture* object_fixture = fixtureA;
            if (objectA->collider_type == collider_enemy_static)
            {
                enemy_data = objectA;
                object_data = objectB;
                object_fixture = fixtureB;
            }
            EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(enemy_data->id);
            b2Vec2  enemy_position = enemy_instance->GetBody()->GetPosition();
            b2Vec2  object_position = object_fixture->GetBody()->GetPosition();
            b2Vec2 vec;
            vec.x = enemy_position.x - object_position.x;
            vec.y = enemy_position.y - object_position.y;

            //オブジェのfixtureの半径を取得
            b2Shape* const object_shape = object_fixture->GetShape();
            b2Vec2 object_half_size = GetFixtureHalfSize(object_shape);

            //ベクトルが縦幅より小さい時 (つまりエネミーがオブジェの上に乗っている場合）
            //+0.01fはちょっと調整
            if (vec.y >= object_half_size.y + 0.01f && object_fixture->GetBody()->GetLinearVelocity() != b2Vec2(0.0, 0.0))
            {
                enemy_instance->CollisionPulledObject();
            }

        }

        //引っ張られている状態のオブジェクトと動的エネミーの衝突
        if ((objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_enemy_dynamic) ||
            (objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_anchor_point) ||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_enemy_dynamic))
        {



            app_atomex_start(Player_Dead_Sound);
            HitStop::StartHitStop(15);
            CameraShake::StartCameraShake(5, 3, 15);
 

            EnemyDynamic* enemy_instance;
            b2Vec2 GetObjectVelocity;

            if (objectA->collider_type == collider_enemy_dynamic)

            {
                enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);

                GetObjectVelocity = fixtureB->GetBody()->GetLinearVelocity();
            }
            else
            {
                enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);

                GetObjectVelocity = fixtureA->GetBody()->GetLinearVelocity();
            }

            if (1.0<(ReturnAbsoluteValue(GetObjectVelocity.x) + ReturnAbsoluteValue(GetObjectVelocity.y)))
            {
                enemy_instance->CollisionPulledObject();
            }
           
       
        }

        //引っ張られている状態のオブジェクトと浮遊エネミーの衝突
        if ((objectA->collider_type == collider_enemy_floating && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_enemy_floating))
        {

            ObjectData* enemy_data = objectB;
            ObjectData* object_data = objectA;
            b2Fixture* object_fixture = fixtureA;
            if (objectA->collider_type == collider_enemy_floating)
            {
                enemy_data = objectA;
                object_data = objectB;
                object_fixture = fixtureB;
            }
            EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(enemy_data->id);
            b2Vec2  enemy_position = enemy_instance->GetBody()->GetPosition();
            b2Vec2  object_position = object_fixture->GetBody()->GetPosition();
            b2Vec2 vec;
            vec.x = enemy_position.x - object_position.x;
            vec.y = enemy_position.y - object_position.y;

            //オブジェのfixtureの半径を取得
            b2Shape* const object_shape = object_fixture->GetShape();
            b2Vec2 object_half_size = GetFixtureHalfSize(object_shape);

            //ベクトルが縦幅より小さい時 (つまりエネミーがオブジェの上に乗っている場合）
            //+0.01fはちょっと調整
            if (vec.y >= object_half_size.y + 0.01f && object_fixture->GetBody()->GetLinearVelocity() != b2Vec2(0.0, 0.0))
            {
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
                if (enemy_instance != nullptr) {
                    enemy_instance->InPlayerSensor();
                }
            }
            else if (objectB->collider_type == collider_enemy_static)
            {
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectB->id);
                if (enemy_instance != nullptr) {
                    enemy_instance->InPlayerSensor();
                }
            }
        }

        //プレイヤーに付属しているセンサーと動的エネミーが触れた場合
        if ((objectA->collider_type == collider_player_sensor && objectB->collider_type == collider_enemy_dynamic) ||
            (objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_player_sensor))
        {
            if (objectA->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                if (enemy_instance != nullptr)
                    enemy_instance->InPlayerSensor();
            }
            else if (objectB->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                if (enemy_instance != nullptr)
                    enemy_instance->InPlayerSensor();
            }
        }


        //プレイヤーと動的エネミーに付属しているセンサーが触れた場合
        if ((objectA->collider_type == collider_enemy_sensor && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_enemy_sensor) ||
            (objectA->collider_type == collider_enemy_sensor && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_enemy_sensor))
        {
            if (objectA->collider_type == collider_enemy_sensor)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                enemy_instance->CollisionSensorPlayer();
            }
            else if (objectB->collider_type == collider_enemy_sensor)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                enemy_instance->CollisionSensorPlayer();
            }
        }

        //プレイヤーが敵の攻撃に触れた場合
        if ((objectA->collider_type == collider_enemy_attack && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_enemy_attack) ||
            (objectA->collider_type == collider_enemy_attack && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_enemy_attack))
        {
            if (objectA->collider_type == collider_enemy_attack)
            {
                EnemyAttack* attack_instance = object_manager.FindEnemyAttackByID(objectA->id);
                attack_instance->CollisionPlayer();
            }
            else if (objectB->collider_type == collider_enemy_attack)
            {
                EnemyAttack* attack_instance = object_manager.FindEnemyAttackByID(objectB->id);
                attack_instance->CollisionPlayer();
            }
        }

        //動的エネミーに付属しているセンサーと地面が触れた場合
        if ((objectA->collider_type == collider_enemy_sensor_move && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_enemy_sensor_move))
        {
            if (objectA->collider_type == collider_enemy_sensor_move)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                enemy_instance->SetIsGround(true);
            }
            else if (objectB->collider_type == collider_enemy_sensor_move)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                enemy_instance->SetIsGround(true);
            }
        }

        //動的エネミー同士が触れた場合
        if ((objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_enemy_dynamic))
        {
            EnemyDynamic* enemy_instanceA= object_manager.FindEnemyDynamicByID(objectA->id);
            EnemyDynamic* enemy_instanceB = object_manager.FindEnemyDynamicByID(objectB->id);
            if (enemy_instanceA->GetBody()->GetPosition().x < enemy_instanceB->GetBody()->GetPosition().x)
            {
                enemy_instanceA->SetDirection(true);
                enemy_instanceB->SetDirection(false);
            }
            else if (enemy_instanceA->GetBody()->GetPosition().x > enemy_instanceB->GetBody()->GetPosition().x)
            {
                enemy_instanceA->SetDirection(false);
                enemy_instanceB->SetDirection(true);
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
       //浮遊エネミーのセンサーがプレイヤーに触れた場合
        if ((objectA->collider_type == collider_enemy_floating_sensor && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_enemy_floating_sensor))
        {
            if (objectA->collider_type == collider_enemy_floating_sensor)
            {
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectA->id);
                enemy_instance->SetIfSensedPlayer(true);
            }
            else if (objectB->collider_type == collider_enemy_floating_sensor)
            {
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectB->id);
                enemy_instance->SetIfSensedPlayer(true);

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
            if (spirit_instance == nullptr) {
                return;
            }
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


          //条件別で違う処理をする（上昇するかどうか、地面に着いているかどうか）d
         //-------------------------------------------------------------------------
            
            //ベクトルが縦幅より小さい時かつ、ソウルは上昇中ではない場合（つまり上昇していないソウルがオブジェの上に乗っている場合）
            //上昇中じゃないのを条件にしたのは、連続した複数のオブジェの中で上昇している時オブジェ間を入る離れる瞬間と本当に一番上のオブジェに乗る瞬間を誤認させないため
            if ((vec.y <= -object_half_size.y) && spirit_instance->GetState() != Spirit_Rising)
            {
                spirit_instance->SetState(Spirit_Idle); //ソウルの状態が地面にいる（ソウルのグラビティが0になる）(ソウルが落ちなくなる)
            }
            else
            {
                spirit_instance->SetState(Spirit_Rising);   //ソウルの状態が上昇中
            }

            spirit_instance->AddCollidedObject(object); //今当たっているオブジェをlistに追加
        }

        // プレーヤーとアイテムが衝突したかを判定
        if ((objectA->collider_type == collider_player_body && objectB->collider_type == collider_item) ||
            (objectA->collider_type == collider_item && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_item) ||
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
                if (item_instance != nullptr) {
                    item_instance->Function();
                    item_instance->SetDestory(true);//削除を呼び出す
                }
            }
            break;
            case ITEM_SPIRIT:
            {
                ItemSpirit* spirit_instance = item_manager.FindItem_Spirit_ByID(item->id);//ItemSpeedUpで同じIDのを探してインスタンスをもらう
                if (spirit_instance != nullptr) {
                    spirit_instance->Function();
                    spirit_instance->SetState(Spirit_Destory);//削除を呼び出す
                }

            }
            break;
            case ITEM_COIN:
            {
                ItemCoin* coin_instance = item_manager.FindItem_Coin_ByID(item->id);//ItemSpeedUpで同じIDのを探してインスタンスをもらう
                if (coin_instance != nullptr) {
                    coin_instance->Function();
                    coin_instance->SetDestory(true);//削除を呼び出す
                }
            }
            break;
            }
      
        }
        //-------------------------------------------------------------------------------------------
          // 
          // ここからボス戦のあたり判定を作る
          // 
          //プレイヤーがボスに触れた時
        if ((objectA->collider_type == collider_boss && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_boss) ||
            (objectA->collider_type == collider_boss && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_boss))
        {

            app_atomex_start(Player_Dead_Sound);
            HitStop::StartHitStop(15);
            CameraShake::StartCameraShake(5, 3, 15);
            player.Player_Damaged(-50, 120);

        }

  

        //プレイヤーとショックウェーブ
        if ((objectA->collider_type == collider_shock_wave && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_shock_wave) ||
            (objectA->collider_type == collider_shock_wave && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_shock_wave))
        {

            app_atomex_start(Player_Dead_Sound);
            HitStop::StartHitStop(15);
            CameraShake::StartCameraShake(5, 3, 15);
            /* player.Player_Damaged(-50, 120);*/

        }


        //プレイヤーとチャージ攻撃
        if ((objectA->collider_type == collider_chage_attack && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_chage_attack) ||
            (objectA->collider_type == collider_chage_attack && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_chage_attack))
        {

            app_atomex_start(Player_Dead_Sound);
            HitStop::StartHitStop(15);
            CameraShake::StartCameraShake(5, 3, 15);
            /* player.Player_Damaged(-50, 120);*/
          
            
            player.Player_knockback(1, boss.GetOutSideBody());

        }

        //プレイヤーとミニゴーレム
        if ((objectA->collider_type == collider_mini_golem && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_mini_golem) ||
            (objectA->collider_type == collider_mini_golem && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_mini_golem))
        {
            app_atomex_start(Player_Dead_Sound);
            HitStop::StartHitStop(15);
            CameraShake::StartCameraShake(5, 3, 15);
            player.Player_Damaged(-50, 120);

            if (objectA->collider_type == collider_mini_golem)
            {
                boss.SetDestroyMiniGolemBody(true, fixtureA->GetBody());
            }
            else
            {
                boss.SetDestroyMiniGolemBody(true, fixtureB->GetBody());
            }
        }

        //プレイヤーの通常攻撃ととミニゴーレム
        if ((objectA->collider_type == collider_mini_golem && objectB->collider_type == collider_normal_attack_anchor) ||
            (objectA->collider_type == collider_normal_attack_anchor && objectB->collider_type == collider_mini_golem) )
        {
            app_atomex_start(Player_Dead_Sound);
            HitStop::StartHitStop(15);
            CameraShake::StartCameraShake(5, 3, 15);
           

            if (objectA->collider_type == collider_mini_golem)
            {
                boss.SetDestroyMiniGolemBody(true, fixtureA->GetBody());
            }
            else
            {
                boss.SetDestroyMiniGolemBody(true, fixtureB->GetBody());
            }
        }


        //ボスのセンサーとプレイヤー
        if ((objectA->collider_type == collider_boss_senosr && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_boss_senosr))
        {
            boss.SetPlayerisNearbyFlag(true);
        }



        //ボスと柱の衝突
        if ((objectA->collider_type == collider_boss && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_boss) ||
            (objectA->collider_type == collider_boss && objectB->collider_type == collider_anchor_point) ||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_boss))
        {

            b2Vec2 GetObjectVelocity;

            if (objectA->collider_type == collider_boss)

            {
            

                GetObjectVelocity = fixtureB->GetBody()->GetLinearVelocity();
            }
            else
            {
               

                GetObjectVelocity = fixtureA->GetBody()->GetLinearVelocity();
            }

            if (1.0 < (ReturnAbsoluteValue(GetObjectVelocity.x) + ReturnAbsoluteValue(GetObjectVelocity.y)))
            {
                boss.SetNowBossState(panic_state);

                if (objectA->object_name == Boss_pillar)
                {
                    boss_pillar* pillar_instance = object_manager.FindBossPillar(objectA->id);//woodで同じIDのを探してインスタンスをもらう
                    pillar_instance->SetSplitting_Destroy_Flag(true);
                  
                }
                if (objectB->object_name == Boss_pillar)
                {
                    boss_pillar* pillar_instance = object_manager.FindBossPillar(objectB->id);//woodで同じIDのを探してインスタンスをもらう
                    pillar_instance->SetSplitting_Destroy_Flag(true);
                   
                }


                if (objectA->object_name == Boss_Carry_Object_Enemy)
                {
                 
                }
                if (objectB->object_name == Boss_Carry_Object_Enemy)
                {

                }

            }

           


        }

        //ボスのオブジェクト
        if (objectA->object_name == Boss_Carry_Object_Enemy || objectB->object_name == Boss_Carry_Object_Enemy)
        {
            //どちらがボスの部屋の柱
            if (objectA->object_name == Boss_Carry_Object_Enemy)//Aが木のオブジェクト
            {

                if (fixtureA->IsSensor() == false && fixtureB->IsSensor() == false)
                {
                    if (objectB->collider_type == collider_anchor) { return; } 
                    if (objectB->object_name == Boss_Carry_Object_Enemy) { return; }
 
                    boss_carry_object_enemy* enemy_instance = object_manager.FindBossCarryObjectEnemy(objectA->id);
                    enemy_instance->SetSplittingDestroyFlag(true);

                    if (objectB->collider_type == collider_boss)
                    {
                        boss.SetNowBossState(panic_state);
                    }
                }

              
              
            }
            if (objectB->object_name == Boss_Carry_Object_Enemy)
            {
                if (fixtureB->IsSensor() == false&&fixtureA->IsSensor()==false)
                {
                    if (objectA->collider_type == collider_anchor) { return; }
                    if (objectA->object_name == Boss_Carry_Object_Enemy) { return; }

                    boss_carry_object_enemy* enemy_instance = object_manager.FindBossCarryObjectEnemy(objectB->id);
                    enemy_instance->SetSplittingDestroyFlag(true);
                    if (objectA->collider_type == collider_boss)
                    {
                        boss.SetNowBossState(panic_state);
                    }
                }

              
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
                if (ground_instance != nullptr) {
                    ground_instance->SetIfPulling(false);
                }
            }
            else
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectB->id);//movable_groundで同じIDのを探してインスタンスをもらう
                if (ground_instance != nullptr) {
                    ground_instance->SetIfPulling(false);

                }
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
                if (enemy_instance != nullptr && ground_instance != nullptr) {
                    ground_instance->DeleteContactedEnemyList(enemy_instance);
                }
            }
            else if (objectB->collider_type == collider_enemy_static)
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectA->id);//movable_groundで同じIDのを探してインスタンスをもらう
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectB->id);
                if (enemy_instance != nullptr && ground_instance != nullptr) {
                    ground_instance->DeleteContactedEnemyList(enemy_instance);
                }
            }
        }



        //プレイヤーと間欠泉の水が触れた場合
        if ((objectA->collider_type == collider_player_leg && objectB->collider_type == collider_geyser_water) ||
            (objectA->collider_type == collider_geyser_water && objectB->collider_type == collider_player_leg))
        {


            if (objectA->collider_type == collider_geyser_water)
            {
                geyser* geyser_instance = object_manager.FindGeyserID(objectA->id);//woodで同じIDのを探してインスタンスをもらう
                geyser_instance->SetFlag(false);//木を引っ張る処理を呼び出す
            }
            else
            {
                geyser* geyser_instance = object_manager.FindGeyserID(objectB->id);//woodで同じIDのを探してインスタンスをもらう
                geyser_instance->SetFlag(false);//木を引っ張る処理を呼び出す
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
                if (enemy_instance != nullptr) {
                    enemy_instance->OutPlayerSensor();
                }
            }
            else if (objectB->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                if (enemy_instance != nullptr) {
                    enemy_instance->OutPlayerSensor();
                }
            }
        }

        //浮遊エネミーのセンサーがプレイヤーに触れた場合
        if ((objectA->collider_type == collider_enemy_floating_sensor && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_enemy_floating_sensor))
        {
            if (objectA->collider_type == collider_enemy_floating_sensor)
            {
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectA->id);
                enemy_instance->SetIfSensedPlayer(false);
            }
            else if (objectB->collider_type == collider_enemy_floating_sensor)
            {
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectB->id);
                enemy_instance->SetIfSensedPlayer(false);
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
            if (spirit_instance == nullptr) {
                return;
            }
            if (spirit_instance->GetState() == Spirit_Collecting)
            {
                return;
            }
            spirit_instance->DeleteCollidedObject(object->GetBody());
        }


         //動的エネミーに付属しているセンサーと地面が離れた時
        if ((objectA->collider_type == collider_enemy_sensor_move && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_enemy_sensor_move))
        {
            if (objectA->collider_type == collider_enemy_sensor_move)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                enemy_instance->SetIsGround(false);
            }
            else if (objectB->collider_type == collider_enemy_sensor_move)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                enemy_instance->SetIsGround(false);
            }
        }

        //ボスのセンサーとプレイヤー
        if ((objectA->collider_type == collider_boss_senosr && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_boss_senosr))
        {
            if (boss.GetPlayerNearbylocked() == 0)
            {
                boss.SetPlayerisNearbyFlag(false);
            }
        }






        //プレイヤーに付属しているセンサーとアンカーポイントが離れた場合
        if ((objectA->collider_type == collider_anchor && objectB->collider_type == collider_anchor_point) ||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_anchor))
        {
            ObjectData* object_data = objectB;
            if (objectA->collider_type == collider_anchor_point) {
                object_data = objectA;
            }

            //木のオブジェクトの引っ張る処理
            if (object_data->object_name == Object_Wood)
            {
                wood* wood_instance = object_manager.FindWoodByID(objectB->id);
                if (wood_instance != nullptr && wood_instance->GetIfPulling() == true) {
                    wood_instance->SetIfPulling(false);
                }
            }

            //岩のオブジェクトの引っ張る処理
            if (object_data->object_name == Object_Rock )
            {
                rock* rock_instance = object_manager.FindRockByID(objectB->id);//woodで同じIDのを探してインスタンスをもらう
                if (rock_instance != nullptr && rock_instance->GetIfPulling() == true) {
                    rock_instance->SetIfPulling(false);
                }
            }


            //静的動的のオブジェクトの
            if (object_data->object_name == Object_Static_to_Dynamic)
            {
                static_to_dynamic_block* static_to_dynamic_block_instance = object_manager.FindStatic_to_Dynamic_BlcokID(objectB->id);//woodで同じIDのを探してインスタンスをもらう
                if (static_to_dynamic_block_instance != nullptr && static_to_dynamic_block_instance->GetIfPulling() == true) {
                    static_to_dynamic_block_instance->SetIfPulling(false);
                }
            }



        }

        //-------------------------------------------------------------------------------------------






        //ボスのセンサーとプレイヤー
        if ((objectA->collider_type == collider_boss_senosr && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_boss_senosr))
        {
            if (boss.GetPlayerNearbylocked() == 0)
            {
                boss.SetPlayerisNearbyFlag(false);
            }
        }
    }



};


#endif // !CONTACTLIST_H
