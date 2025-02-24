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
#include"impact_effect.h"
#include<vector>
#include"gokai.h"
#include"blown_away_effect.h"
#include"Change_Enemy_Filter_and_Body.h"
#include"bound_block.h"
#include"UI_Block.h"
#include"break_block.h"
#include"boss_wall_object.h"

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

  
    float impactThreshold = 1.0f; // エフェクトを発生させる衝撃の下限？　まあーこれ以上ならエフェクト発生させるよ

    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override {
        float maxImpulse = 0.0f;

        // 衝撃の最大値を取得
        for (int i = 0; i < impulse->count; i++) {
            if (impulse->normalImpulses[i] > maxImpulse) {
                maxImpulse = impulse->normalImpulses[i];
            }
        }

        // 衝撃が閾値を超えたらエフェクトを記録
        if (maxImpulse > impactThreshold) {

            // ワールド座標の衝突位置を取得
            b2WorldManifold worldManifold;
            contact->GetWorldManifold(&worldManifold);
            b2Vec2 worldPoint = worldManifold.points[0]; // **ワールド座標の衝突位置**

            // エフェクトの角度を法線ベクトルから計算（90度補正）
            b2Vec2 normal = worldManifold.normal;
            float effectAngle = atan2(normal.y, normal.x) + (b2_pi / 2.0f); // **90度補正**

            // **衝撃の大きさに基づいてサイズを決定**
            float minScale = 0.5f;  // 最小サイズ
            float maxScale = 2.0f;  // 最大サイズ
            float effectScale = minScale + (maxImpulse / 10.0f); // 衝撃が大きいほどサイズを拡大

            // サイズの上限を設定
            if (effectScale > maxScale) effectScale = maxScale;

            //エフェクトの種類を設定できる
            int effectType;


            if (effectScale < 0.5f)
            {
                effectType = 1;
            }
            else
            {
                effectType = 2;
            }

            // エフェクトリストに追加
            impactEffects.emplace_back(ImpactEffect(worldPoint, effectAngle, effectScale, effectType));
        }
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
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_break_block) ||
            (objectA->collider_type == collider_break_block && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_object)||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_boss_field && objectB->collider_type == collider_player_leg)||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_boss_field)||
            (objectA->collider_type == collider_bound_block && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_bound_block)){
            // 衝突処理（プレーヤーと地面が接触した時）
            
            player.SetIsJumping(false);


        }


        // プレーヤーとコンタクトブロックが衝突したかを判定
        if ((objectA->collider_type == collider_player_leg && objectB->collider_type == collider_contact_block) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_contact_block) ||
            (objectA->collider_type == collider_contact_block && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_contact_block && objectB->collider_type == collider_player_leg))
        {
            // 衝突処理

            if (objectA->collider_type == collider_contact_block)//Aがコンタクトブロックのオブジェクト
            {
                contact_block* contact_block_instance = object_manager.FindContactBlock(objectA->id);
                contact_block_instance->SetFlag(true);
            }
            if (objectB->collider_type == collider_contact_block)
            {
                contact_block* contact_block_instance = object_manager.FindContactBlock(objectB->id);
                contact_block_instance->SetFlag(true);
            }

        }

        // プレーヤーとテレポートブロックが衝突したかを判定
        if ((objectA->collider_type == collider_player_leg && objectB->collider_type == collider_teleport_block) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_teleport_block) ||
            (objectA->collider_type == collider_teleport_block && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_teleport_block && objectB->collider_type == collider_player_leg))
        {
            // 衝突処理（プレーヤーと地面が接触した時）

                     //どちらが木のオブジェクトか特定
            if (objectA->collider_type== collider_teleport_block)//Aがテレポートブロックのオブジェクト
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
                geyser_instance->SetFlag(true);
            }
            else
            {
                geyser* geyser_instance = object_manager.FindGeyserID(objectB->id);//woodで同じIDのを探してインスタンスをもらう
                geyser_instance->SetFlag(true);
            }
        }



        //プレイヤーとバウンドブロックが触れた場合
        if ((objectA->collider_type == collider_player_leg && objectB->collider_type == collider_bound_block) ||
            (objectA->collider_type == collider_bound_block && objectB->collider_type == collider_player_leg))
        {


            if (objectA->collider_type == collider_bound_block)
            {
                boss_bound_block* bound_block_instance = object_manager.FindBossBoundBlock(objectA->id);
                bound_block_instance->SetJumpFlag(true);
            
            }
            else
            {
                boss_bound_block* bound_block_instance = object_manager.FindBossBoundBlock(objectB->id);
                bound_block_instance->SetJumpFlag(true);
            
            }
        }


        //プレイヤーと傾斜のついたブロックが衝突したかを判定
        if ((objectA->collider_type == collider_player_leg && objectB->object_name == Object_sloping_block) ||
            (objectA->object_name == Object_sloping_block && objectB->collider_type == collider_player_leg))
        {
            if (objectA->object_name == Object_sloping_block)
            {
                sloping_block* sloping_block_instance = object_manager.FindSloping_BlockByID(objectA->id);
                sloping_block_instance->SetPlayerCollided(true);
            }
            else if (objectB->object_name == Object_sloping_block)
            {
                sloping_block* sloping_block_instance = object_manager.FindSloping_BlockByID(objectB->id);
                sloping_block_instance->SetPlayerCollided(true);
            }

        }

        // プレーヤーとオブジェクトが衝突したかを判定
        if ((objectA->collider_type == collider_player_leg && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_player_leg))
        {

            float object_velocity=0;
            const b2Body* object_body = nullptr;
            
            if (objectA->collider_type == collider_object)
            {
                object_body = fixtureA->GetBody();
                object_velocity=ReturnAbsoluteValue(object_body->GetLinearVelocity().x) + ReturnAbsoluteValue(object_body->GetLinearVelocity().y);
            }
            else
            {
                object_body = fixtureB->GetBody();
                object_velocity=ReturnAbsoluteValue(object_body->GetLinearVelocity().x) + ReturnAbsoluteValue(object_body->GetLinearVelocity().y);
            }

            if (0.7f < object_velocity)//ここに入ったらオブジェクトが移動中であり、被弾判定してよい
            {
                    player.Player_Damaged(0, 120, object_body);//被弾処理
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
                if (10 < player.GetAnchorFrameManagement())
                {
                    Anchor::SetAnchorState(Deleting_state);
                }
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
                if (ground_instance->GetIfPulled()) { return; }
                ground_instance->Pulling_ground();
                ground_instance->SetIfPulling(true);
                CameraShake::StartCameraShake(20, 80, 50);
            }
            else
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectB->id);//movable_groundで同じIDのを探してインスタンスをもらう
                if (ground_instance->GetIfPulled()) { return; }
                ground_instance->Pulling_ground();
                ground_instance->SetIfPulling(true);
                CameraShake::StartCameraShake(20, 80, 50);
            }

        }

        //オブジェクトと壊れるブロック
        if ((objectA->collider_type == collider_break_block && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_break_block))
        {

            if (objectA->object_name == Boss_pillar || objectB->object_name == Boss_pillar) { return; }


           

           /* HitStop::StartHitStop(5);*/

            if (objectA->collider_type == collider_break_block)
            {
                Break_Block* breakblock_instance = object_manager.FindBreakBlock(objectA->id);
                breakblock_instance->SetFlag(true);
            }
            else if (objectB->collider_type == collider_break_block)
            {
                Break_Block* breakblock_instance = object_manager.FindBreakBlock(objectB->id);
                breakblock_instance->SetFlag(true);
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
                        wood_instance->Pulling_wood();//木を引っ張る処理を呼び出す
                    }
                    else
                    {
                        wood* wood_instance = object_manager.FindWoodByID(objectB->id);
                        wood_instance->Pulling_wood();
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
                        ground_instance->Pulling_ground();
                        ground_instance->SetIfPulling(true);
                    }
                    else
                    {
                        movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectB->id);//movable_groundで同じIDのを探してインスタンスをもらう
                        ground_instance->Pulling_ground();
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
                        rock_instance->Pulling_rock();//木を引っ張る処理を呼び出す
                    }
                    else
                    {
                        rock* rock_instance = object_manager.FindRockByID(objectB->id);//woodで同じIDのを探してインスタンスをもらう
                        rock_instance->Pulling_rock();//木を引っ張る処理を呼び出す
                    }

                }

                //水が噴き出す
                if (objectA->object_name == Object_Geyser || objectB->object_name == Object_Geyser)
                {
                    //どちらが岩のオブジェクトか特定
                    if (objectA->object_name == Object_Geyser)//Aが岩のオブジェクト
                    {
                        geyser* geyser_instance = object_manager.FindGeyserID(objectA->id);
                        geyser_instance->SetOpenGyserFlag(true);//水が噴き出す
                    }
                    else
                    {
                        geyser* geyser_instance = object_manager.FindGeyserID(objectB->id);
                        geyser_instance->SetOpenGyserFlag(true);//水が噴き出す
                    }

                    //噴き出す音の検知
                    app_atomex_start(Object_Geyser_Sound);

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
                    CameraShake::DelayStartCameraShake(10, 60, 20, 30);
                    HitStop::DelayStartHitStop(30, 10);
                   
                    //どちらが岩のオブジェクトか特定
                    if (objectA->object_name == Boss_core)//Aが静的動的のオブジェクト
                    {
                        boss.BossDamaged();
                        boss.SetCoreDeleteFlag(true);
                        boss.SetCorePullingFlag(true);

                    }
                    else
                    {
                        boss.BossDamaged();
                        boss.SetCoreDeleteFlag(true);
                        boss.SetCorePullingFlag(true);

                    }
                }

                //ボスの部屋の柱
                if (objectA->object_name == Boss_pillar || objectB->object_name == Boss_pillar)
                {
                    //どちらがボスの部屋の柱
                    if (objectA->object_name == Boss_pillar)//Aが木のオブジェクト
                    {
                        boss_pillar* pillar_instance = object_manager.FindBossPillar(objectA->id);//woodで同じIDのを探してインスタンスをもらう
                        pillar_instance->SetPullingFlag(true);
                    }
                    else
                    {
                        boss_pillar* pillar_instance = object_manager.FindBossPillar(objectB->id);
                        pillar_instance->SetPullingFlag(true);
                    }
                }


                //ボスのオブジェクト
                if (objectA->object_name == Boss_Carry_Object_Enemy || objectB->object_name == Boss_Carry_Object_Enemy)
                {
                    //どちらがボスの部屋の柱
                    if (objectA->object_name == Boss_Carry_Object_Enemy)//Aが木のオブジェクト
                    {
                        boss_carry_object_enemy* enemy_instance = object_manager.FindBossCarryObjectEnemy(objectA->id);
                        enemy_instance->SetAnchorHItFlag(true);

                    }
                    else
                    {
                        boss_carry_object_enemy* enemy_instance = object_manager.FindBossCarryObjectEnemy(objectB->id);
                        enemy_instance->SetAnchorHItFlag(true);
                    }
                }





                //ボスの部屋の壁
                if (objectA->object_name == Boss_Wall || objectB->object_name == Boss_Wall)
                {
                    //どちらがボスの部屋の柱
                    if (objectA->object_name == Boss_Wall)//ボス戦の壁
                    {
                        Boss_Wall_Objcet* wall_instance = object_manager.FindBossWallObjcet(objectA->id);
                        wall_instance->SetPullingFlag(true);
                    }
                    if (objectB->object_name == Boss_Wall)//ボス戦の壁
                    {
                        Boss_Wall_Objcet* wall_instance = object_manager.FindBossWallObjcet(objectB->id);
                        wall_instance->SetPullingFlag(true);
                    }
                }
            }//end_if( Anchor::GetAnchorState() == Connected_state)

        
           

       
             
        }


        //ボスの壁と柱が触れた
        if (objectA->collider_type == collider_object && objectB->collider_type == collider_object)
        {
            //どちらがボスの部屋の柱
            if (objectA->object_name == Boss_pillar&&objectB->object_name==Boss_Wall)
            {
                boss_pillar* pillar_instance = object_manager.FindBossPillar(objectA->id);
                pillar_instance->SetSplitting_Destroy_Flag(true);
            }

            //どちらがボスの部屋の柱
            if (objectB->object_name == Boss_pillar && objectA->object_name == Boss_Wall)
            {
                boss_pillar* pillar_instance = object_manager.FindBossPillar(objectB->id);
                pillar_instance->SetSplitting_Destroy_Flag(true);
            }


        
        }

     
        //転がる岩と静的動的ブロックの衝突判定じ
        if (objectA->object_name == Object_Static_to_Dynamic && objectB->object_name == Object_Rock)
        {
            static_to_dynamic_block* instance = object_manager.FindStatic_to_Dynamic_BlcokID(objectA->id);
            instance->SetNowBreakBlock(true);
        }
        if (objectA->object_name == Object_Rock && objectB-> object_name == Object_Static_to_Dynamic)
        {
            static_to_dynamic_block* instance = object_manager.FindStatic_to_Dynamic_BlcokID(objectB->id);
            instance->SetNowBreakBlock(true);
        }

      
      


        //プレイヤーと静的エネミーの衝突
        if ((objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_enemy_static)||
            (objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_enemy_static))
        {
            
        }

        //プレイヤーと動的エネミーの衝突
        if ((objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_enemy_dynamic)||
            (objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_enemy_dynamic))
        {
            
        }

        //プレイヤーの通常攻撃攻撃と動的エネミーの衝突
        if ((objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_normal_attack_anchor) ||
            (objectA->collider_type == collider_normal_attack_anchor && objectB->collider_type == collider_enemy_dynamic))
        {

            //カメラシェイクとヒットストップを追加しました
            CameraShake::StartCameraShake(20, 20, 10);
            HitStop::StartHitStop(5);
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

        //プレイヤーの通常攻撃攻撃と静的エネミーの衝突
        if ((objectA->collider_type == collider_enemy_static && objectB->collider_type == collider_normal_attack_anchor) ||
            (objectA->collider_type == collider_normal_attack_anchor && objectB->collider_type == collider_enemy_static))
        {

            //カメラシェイクとヒットストップを追加しました
            CameraShake::StartCameraShake(20, 20, 10);
            HitStop::StartHitStop(5);
            if (objectA->collider_type == collider_enemy_static)
            {
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectA->id);
                enemy_instance->CollisionPulledObject();
            }
            else if (objectB->collider_type == collider_enemy_static)
            {
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectB->id);
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
            //カメラシェイクとヒットストップを追加しました
            CameraShake::StartCameraShake(20, 20, 10);
            CameraShake::StartCameraShake(5, 3, 15);
            HitStop::StartHitStop(15);
          
            if (objectA->collider_type == collider_enemy_floating)
            {
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectA->id);
                if (enemy_instance->GetAttactCoolingTime() <= 0)
                {
                    enemy_instance->CollisionPlayer();
                    enemy_instance->SetAttactCoolingTime(60);
                }                
            }
            else if (objectB->collider_type == collider_enemy_floating)
            {
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectB->id);
                if (enemy_instance->GetAttactCoolingTime() <= 0)
                {
                    enemy_instance->CollisionPlayer();
                    enemy_instance->SetAttactCoolingTime(60);
                }
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
            if (objectA->collider_type == collider_enemy_dynamic)
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectB->id);//movable_groundで同じIDのを探してインスタンスをもらう
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                ground_instance->AddContactedEnemyList(enemy_instance);
            }
            else if (objectB->collider_type == collider_enemy_dynamic)
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectA->id);//movable_groundで同じIDのを探してインスタンスをもらう
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                ground_instance->AddContactedEnemyList(enemy_instance);
            }

        }
        
        //引っ張れれている状態の床と浮遊エネミーの衝突
        if ((objectA->collider_type == collider_enemy_floating && objectB->collider_type == collider_object_destroyer_of_enemy) ||
            (objectA->collider_type == collider_object_destroyer_of_enemy && objectB->collider_type == collider_enemy_floating))
        {
            if (objectA->collider_type == collider_enemy_floating)
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectB->id);//movable_groundで同じIDのを探してインスタンスをもらう
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectA->id);
                ground_instance->AddContactedEnemyList(enemy_instance);
            }
            else if (objectB->collider_type == collider_enemy_floating)
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectA->id);//movable_groundで同じIDのを探してインスタンスをもらう
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
            EnemyStatic* enemy_instance;
            b2Vec2 GetObjectVelocity;

            if (objectA->collider_type == collider_enemy_static)

            {
                enemy_instance = object_manager.FindEnemyStaticByID(objectA->id);

                GetObjectVelocity = fixtureB->GetBody()->GetLinearVelocity();
            }
            else
            {
                enemy_instance = object_manager.FindEnemyStaticByID(objectB->id);

                GetObjectVelocity = fixtureA->GetBody()->GetLinearVelocity();
            }

            if (0.5 < (ReturnAbsoluteValue(GetObjectVelocity.x) + ReturnAbsoluteValue(GetObjectVelocity.y)))
            {


                //豪快ゲージの加算処理-------------------------------------------------------------------------------------------
                int needlevel = 0;
                if (objectA->collider_type == collider_enemy_static)
                {
                    needlevel = objectB->need_anchor_level;
                }
                else
                {
                    needlevel = objectA->need_anchor_level;
                }
                switch (needlevel)
                {
                case 1:
                    Gokai_UI::AddGokaiCount(100);
                    break;
                case 2:
                    Gokai_UI::AddGokaiCount(500);
                    break;
                case 3:
                    Gokai_UI::AddGokaiCount(1000);
                    break;
                default:
                    break;
                }
                //--------------------------------------------------------------------------------------------
                enemy_instance->CollisionPulledObject();

                app_atomex_start(Player_Dead_Sound);
                HitStop::StartHitStop(15);
                CameraShake::StartCameraShake(5, 3, 15);
            }

        }

        //引っ張られている状態のオブジェクトと動的エネミーの衝突
        if ((objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_enemy_dynamic) ||
            (objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_anchor_point) ||
            (objectA->collider_type == collider_anchor_point && objectB->collider_type == collider_enemy_dynamic))
        {


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

            if (0.001<(ReturnAbsoluteValue(GetObjectVelocity.x) + ReturnAbsoluteValue(GetObjectVelocity.y)))
            {


                //豪快ゲージの加算処理-------------------------------------------------------------------------------------------
                int needlevel=0;
                if (objectA->collider_type == collider_enemy_dynamic)
                {
                    needlevel = objectB->need_anchor_level;
                }
                else
                {
                    needlevel = objectA->need_anchor_level;
                }
                switch (needlevel)
                {
                case 1:
                    Gokai_UI::AddGokaiCount(100);
                    break;
                case 2:
                    Gokai_UI::AddGokaiCount(500);
                    break;
                case 3:
                    Gokai_UI::AddGokaiCount(1000);
                    break;
                default:
                    break;
                }
                //--------------------------------------------------------------------------------------------
                enemy_instance->CollisionPulledObject();

                app_atomex_start(Player_Dead_Sound);
                HitStop::StartHitStop(15);
                CameraShake::StartCameraShake(5, 3, 15);
            }
            //静止状態のオブジェクトに当たると向きを反転させる
            else
            {
                enemy_instance->SetDirection(!enemy_instance->GetDirection());
            }
       
        }

        //引っ張られている状態のオブジェクトと浮遊エネミーの衝突
        if ((objectA->collider_type == collider_enemy_floating && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_enemy_floating))
        {

            EnemyFloating* enemy_instance;
            b2Vec2 GetObjectVelocity;

            if (objectA->collider_type == collider_enemy_floating)

            {
                enemy_instance = object_manager.FindEnemyFloatingByID(objectA->id);

                GetObjectVelocity = fixtureB->GetBody()->GetLinearVelocity();
            }
            else
            {
                enemy_instance = object_manager.FindEnemyFloatingByID(objectB->id);

                GetObjectVelocity = fixtureA->GetBody()->GetLinearVelocity();
            }

            if (0.5 < (ReturnAbsoluteValue(GetObjectVelocity.x) + ReturnAbsoluteValue(GetObjectVelocity.y)))
            {


                //豪快ゲージの加算処理-------------------------------------------------------------------------------------------
                int needlevel = 0;
                if (objectA->collider_type == collider_enemy_floating)
                {
                    needlevel = objectB->need_anchor_level;
                }
                else
                {
                    needlevel = objectA->need_anchor_level;
                }
                switch (needlevel)
                {
                case 1:
                    Gokai_UI::AddGokaiCount(100);
                    break;
                case 2:
                    Gokai_UI::AddGokaiCount(500);
                    break;
                case 3:
                    Gokai_UI::AddGokaiCount(1000);
                    break;
                default:
                    break;
                }
                //--------------------------------------------------------------------------------------------
                enemy_instance->CollisionPulledObject();

                app_atomex_start(Player_Dead_Sound);
                HitStop::StartHitStop(15);
                CameraShake::StartCameraShake(5, 3, 15);
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

        //プレイヤーと静的エネミーに付属しているセンサーが触れた場合
        if ((objectA->collider_type == collider_enemy_static_sensor && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_enemy_static_sensor) ||
            (objectA->collider_type == collider_enemy_static_sensor && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_enemy_static_sensor))
        {
            if (objectA->collider_type == collider_enemy_static_sensor)
            {
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectA->id);
                enemy_instance->CollisionSensorPlayer();
            }
            else if (objectB->collider_type == collider_enemy_static_sensor)
            {
                EnemyStatic* enemy_instance = object_manager.FindEnemyStaticByID(objectB->id);
                enemy_instance->CollisionSensorPlayer();
            }
        }

        //プレイヤーが敵の攻撃に触れた場合
        if ((objectA->collider_type == collider_enemy_attack && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_enemy_attack) ||
            (objectA->collider_type == collider_enemy_attack && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_enemy_attack))
        {
            int damage;
            const b2Body* attack_body = nullptr;
            if (objectA->collider_type == collider_enemy_attack)
            {
                EnemyAttack* attack_instance = object_manager.FindEnemyAttackByID(objectA->id);
                damage = attack_instance->GetDamage();

                //Player_Damaged関数の引数のために攻撃してるエネミー元のBodyを検索してる(なぜenemy_attackのbodyを使わないかというと、enemy_attackとplayerの位置順番がたまに実際とは逆になることあるから)
                EnemyDynamic* enemy_dynamic_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                if (enemy_dynamic_instance != nullptr)
                {
                    attack_body = enemy_dynamic_instance->GetBody();
                }
                else
                {
                    EnemyStatic* enemy_static_instance = object_manager.FindEnemyStaticByID(objectA->id);
                    attack_body = enemy_static_instance->GetBody();
                }

            }
            else if (objectB->collider_type == collider_enemy_attack)
            {
                EnemyAttack* attack_instance = object_manager.FindEnemyAttackByID(objectB->id);
                damage = attack_instance->GetDamage();

                //Player_Damaged関数の引数のために攻撃してるエネミー元のBodyを検索してる(なぜenemy_attackのbodyを使わないかというと、enemy_attackとplayerの位置順番がたまに実際とは逆になることあるから)
                EnemyDynamic* enemy_dynamic_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                if (enemy_dynamic_instance != nullptr)
                {
                    attack_body = enemy_dynamic_instance->GetBody();
                }
                else
                {
                    EnemyStatic* enemy_static_instance = object_manager.FindEnemyStaticByID(objectB->id);
                    attack_body = enemy_static_instance->GetBody();
                }
            }

            app_atomex_start(Player_Dead_Sound);
            HitStop::StartHitStop(15);
            CameraShake::StartCameraShake(5, 3, 15);

            player.Player_Damaged(-damage, 120, attack_body);
        }

        //動的エネミーに付属しているセンサーと地面が触れた場合
        if ((objectA->collider_type == collider_enemy_sensor && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_enemy_sensor) ||
            (objectA->collider_type == collider_enemy_sensor && objectB->collider_type == collider_break_block) ||
            (objectA->collider_type == collider_break_block && objectB->collider_type == collider_enemy_sensor))
        {
            if (objectA->collider_type == collider_enemy_sensor)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                enemy_instance->SetIsGround(true);
            }
            else if (objectB->collider_type == collider_enemy_sensor)
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
        //動的エネミーが静的エネミーに触れた時
        if ((objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_enemy_static_sensor) ||
            (objectA->collider_type == collider_enemy_static_sensor && objectB->collider_type == collider_enemy_dynamic))
        {
            if (objectA->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic* enemy_dynamic_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                EnemyStatic* enemy_static_instance = object_manager.FindEnemyStaticByID(objectB->id);
                if (enemy_dynamic_instance->GetBody()->GetPosition().x < enemy_static_instance->GetBody()->GetPosition().x)
                {
                    enemy_dynamic_instance->SetDirection(true);
                }
                else
                {
                    enemy_dynamic_instance->SetDirection(false);
                }
            }
            else if (objectB->collider_type == collider_enemy_dynamic)
            {
                EnemyDynamic* enemy_dynamic_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                EnemyStatic* enemy_static_instance = object_manager.FindEnemyStaticByID(objectA->id);
                if (enemy_dynamic_instance->GetBody()->GetPosition().x < enemy_static_instance->GetBody()->GetPosition().x)
                {
                    enemy_dynamic_instance->SetDirection(true);
                }
                else
                {
                    enemy_dynamic_instance->SetDirection(false);
                }
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
            case ITEM_JEWEL:
            {
                ItemJewel* jewel_instance = item_manager.FindItem_Jewel_ByID(item->id);//ItemSpeedUpで同じIDのを探してインスタンスをもらう
                if (jewel_instance != nullptr) {
                    jewel_instance->SetIfCollecting(true);//削除を呼び出す
                }
            }
            break;
            case ITEM_SAVEPOINT:
            {
                ItemSavePoint* savepoint_instance = item_manager.FindItem_SavePoint(item->id);//ItemSpeedUpで同じIDのを探してインスタンスをもらう
                if (savepoint_instance != nullptr) {
                    const ItemSavePoint* player_registered_SavePoint = player.GetRegisteredSavePoint();
                    if (player_registered_SavePoint == nullptr)
                    {
                        savepoint_instance->SetPlayerPassed();
                    }
                    else if (savepoint_instance->GetBody()->GetPosition().x > player_registered_SavePoint->GetBody()->GetPosition().x)
                    {
                        savepoint_instance->SetPlayerPassed();
                    }
                }
            }
            break;
            }
      
        }



        //プレイヤーとUIセンサーが触れた場合
        if ((objectA->collider_type == collider_player_body && objectB->collider_type == collider_UI_block) ||
            (objectA->collider_type == collider_UI_block && objectB->collider_type == collider_player_body))
        {
            if (objectA->collider_type == collider_UI_block)
            {
                UI_block* ui_instance = object_manager.FindUiBlock(objectA->id);
                if (ui_instance->GetIfVideo())
                {
                    ui_instance->SetVideoState(Video_Resume);
                }
                else
                {
                    ui_instance->SetFlag(true);
                }               
            }
            else if (objectB->collider_type == collider_UI_block)
            {

                UI_block* ui_instance = object_manager.FindUiBlock(objectB->id);
                if (ui_instance->GetIfVideo())
                {
                    ui_instance->SetVideoState(Video_Resume);
                }
                else
                {
                    ui_instance->SetFlag(true);
                }
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

            const b2Body* boss_body = nullptr;
            if (objectA->collider_type == collider_boss)
            {
                boss_body = fixtureA->GetBody();
            }
            else
            {
                boss_body = fixtureB->GetBody();
            }

            player.Player_Damaged(0, 120, boss_body);
            

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

            const b2Body* wave_body = nullptr;
            if (objectA->collider_type == collider_shock_wave)
            {
                wave_body = fixtureA->GetBody();
            }
            else
            {
                wave_body = fixtureB->GetBody();
            }

            player.Player_Damaged(-50, 120, wave_body);
            
            //ショックウェーブを破壊する
            boss.SetShockWaveFrame(300);

        }

        //壁とショックウェーブ
        if ((objectA->collider_type == collider_shock_wave && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_shock_wave))
        {

            if (objectA->object_name == Boss_field_block)return; 
            if (objectB->object_name == Boss_field_block)return;

            boss.SetShockWaveFrame(300);
        }

        //柱とショックウェーブ
        if ((objectA->collider_type == collider_shock_wave && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_shock_wave))
        {

            boss.SetShockWaveFrame(300);
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

            const b2Body* attack_body = nullptr;
            if (objectA->collider_type == collider_chage_attack)
            {
                attack_body = fixtureA->GetBody();
            }
            else
            {
                attack_body = fixtureB->GetBody();
            }

            player.Player_Damaged(-50, 120, attack_body);

          
            

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

            b2Body* golem_body = nullptr;
            if (objectA->collider_type == collider_mini_golem)
            {
                golem_body = fixtureA->GetBody();
            }
            else
            {
                golem_body = fixtureB->GetBody();
            }

            player.Player_Damaged(-50, 120, golem_body);

            boss.SetDestroyMiniGolemBody(true, golem_body);

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


        //ミニゴーレムとオブジェクト
        if ((objectA->collider_type == collider_mini_golem && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_mini_golem))
        {
          
           
            if (objectA->collider_type == collider_mini_golem)
            {
                boss.SetDestroyMiniGolemBody(true, fixtureA->GetBody());
                HitStop::StartHitStop(15);
                CameraShake::StartCameraShake(5, 3, 15);
            }
            else
            {
                boss.SetDestroyMiniGolemBody(true, fixtureB->GetBody());
                HitStop::StartHitStop(15);
                CameraShake::StartCameraShake(5, 3, 15);
            }
        }

        //ミニゴーレムとボスの壁
        if ((objectA->collider_type == collider_mini_golem && objectB->collider_type == collider_object) ||
            (objectA->collider_type == collider_object && objectB->collider_type == collider_mini_golem))
        {
            if (objectA->object_name == Boss_Wall || objectB->object_name == Boss_Wall)
            {
                if (objectA->collider_type == collider_mini_golem)
                {
                    boss.SetDestroyMiniGolemBody(true, fixtureA->GetBody());
                }
                else
                {
                    boss.SetDestroyMiniGolemBody(true, fixtureB->GetBody());
                }
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
                
                boss.SetBossSheetCnt(0);
                if (objectA->object_name == Boss_pillar)
                {
                    if (boss.GetNowBossState() != panic_state)
                    {
                        boss.SetNowBossState(damage_state);
                    }
                    boss_pillar* pillar_instance = object_manager.FindBossPillar(objectA->id);//woodで同じIDのを探してインスタンスをもらう
                    pillar_instance->SetSplitting_Destroy_Flag(true);
                  
                }
                if (objectB->object_name == Boss_pillar)
                {
                    if (boss.GetNowBossState() != panic_state)
                    {
                        boss.SetNowBossState(damage_state);
                    }
                    boss_pillar* pillar_instance = object_manager.FindBossPillar(objectB->id);//woodで同じIDのを探してインスタンスをもらう
                    pillar_instance->SetSplitting_Destroy_Flag(true);
                }


                if (objectA->object_name == Boss_Wall)
                {
                    boss.SetNowBossState(panic_state);
                    Boss_Wall_Objcet* wall_instance = object_manager.FindBossWallObjcet(objectA->id);//woodで同じIDのを探してインスタンスをもらう
                    wall_instance->SetSplitting_Destroy_Flag(true);
                }
                if (objectB->object_name == Boss_Wall)
                {
                    boss.SetNowBossState(panic_state);
                    Boss_Wall_Objcet* wall_instance = object_manager.FindBossWallObjcet(objectB->id);//woodで同じIDのを探してインスタンスをもらう
                    wall_instance->SetSplitting_Destroy_Flag(true);
                }
      

            }
            else
            {

                //ボスがぶつかったら壊れるように
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
                        if (boss.GetNowBossState() != panic_state)
                        {
                            boss.SetBossSheetCnt(0);
                            boss.SetNowBossState(damage_state);
                        }
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
                        if (boss.GetNowBossState() != panic_state)
                        {
                            boss.SetBossSheetCnt(0);
                            boss.SetNowBossState(damage_state);
                        }
                    }
                }

              
            }
     
        }

        //動いている壁とぶつかった時　止まっているボスの壁がぶつかった時
        if (objectA->object_name == Boss_Wall && objectB->object_name == Boss_Wall)
        {
            //動いている方
            if (fixtureA->GetType() == b2_dynamicBody)
            {
                Boss_Wall_Objcet* wall_instance = object_manager.FindBossWallObjcet(objectA->id);
                wall_instance->SetSplitting_Destroy_Flag(true);
            }


            if (fixtureB->GetType() == b2_dynamicBody)
            {
                Boss_Wall_Objcet* wall_instance = object_manager.FindBossWallObjcet(objectB->id);
                wall_instance->SetSplitting_Destroy_Flag(true);
            }
        }




        //進入禁止エリアとミニゴーレム
        if ((objectA->collider_type == collider_mini_golem && objectB->collider_type == collider_no_entry_block) ||
            (objectA->collider_type == collider_no_entry_block && objectB->collider_type == collider_mini_golem))
        {
            if (objectA->collider_type == collider_mini_golem)
            {
                boss.SetDestroyMiniGolemBody(true, fixtureA->GetBody());
            }
            else
            {
                boss.SetDestroyMiniGolemBody(true, fixtureB->GetBody());
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

        //引っ張れる床と敵が離れた場合
        if ((objectA->collider_type == collider_enemy_dynamic && objectB->collider_type == collider_object_destroyer_of_enemy) ||
            (objectA->collider_type == collider_object_destroyer_of_enemy && objectB->collider_type == collider_enemy_dynamic))
        {

            if (objectA->collider_type == collider_enemy_dynamic)
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectB->id);//movable_groundで同じIDのを探してインスタンスをもらう
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                if (enemy_instance != nullptr && ground_instance != nullptr) {
                    ground_instance->DeleteContactedEnemyList(enemy_instance);
                }
            }
            else if (objectB->collider_type == collider_enemy_dynamic)
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectA->id);//movable_groundで同じIDのを探してインスタンスをもらう
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                if (enemy_instance != nullptr && ground_instance != nullptr) {
                    ground_instance->DeleteContactedEnemyList(enemy_instance);
                }
            }
        }

        //引っ張れる床と浮遊敵が離れた場合
        if ((objectA->collider_type == collider_enemy_floating && objectB->collider_type == collider_object_destroyer_of_enemy) ||
            (objectA->collider_type == collider_object_destroyer_of_enemy && objectB->collider_type == collider_enemy_floating))
        {

            if (objectA->collider_type == collider_enemy_floating)
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectB->id);//movable_groundで同じIDのを探してインスタンスをもらう
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectA->id);
                if (enemy_instance != nullptr && ground_instance != nullptr) {
                    ground_instance->DeleteContactedEnemyList(enemy_instance);
                }
            }
            else if (objectB->collider_type == collider_enemy_floating)
            {
                movable_ground* ground_instance = object_manager.FindMovable_GroundID(objectA->id);//movable_groundで同じIDのを探してインスタンスをもらう
                EnemyFloating* enemy_instance = object_manager.FindEnemyFloatingByID(objectB->id);
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

        // プレーヤーとコンタクトブロックが衝突したかを判定
        if ((objectA->collider_type == collider_player_leg && objectB->collider_type == collider_contact_block) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_contact_block) ||
            (objectA->collider_type == collider_contact_block && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_contact_block && objectB->collider_type == collider_player_leg))
        {
            // 衝突処理

            if (objectA->collider_type == collider_contact_block)//Aがコンタクトブロックのオブジェクト
            {
                contact_block* contact_block_instance = object_manager.FindContactBlock(objectA->id);
                contact_block_instance->SetFlag(false);
            }
            if (objectB->collider_type == collider_contact_block)
            {
                contact_block* contact_block_instance = object_manager.FindContactBlock(objectB->id);
                contact_block_instance->SetFlag(false);
            }

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



         //動的エネミーに付属しているセンサーと地面が離れた時
        if ((objectA->collider_type == collider_enemy_sensor && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_enemy_sensor) ||
            (objectA->collider_type == collider_enemy_sensor && objectB->collider_type == collider_break_block) ||
            (objectA->collider_type == collider_break_block && objectB->collider_type == collider_enemy_sensor))
        {
            if (objectA->collider_type == collider_enemy_sensor)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                enemy_instance->SetIsGround(false);
            }
            else if (objectB->collider_type == collider_enemy_sensor)
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


        //プレイヤーと動的エネミーに付属しているセンサーが離れた場合
        if ((objectA->collider_type == collider_enemy_sensor && objectB->collider_type == collider_player_body) ||
            (objectA->collider_type == collider_player_body && objectB->collider_type == collider_enemy_sensor) ||
            (objectA->collider_type == collider_enemy_sensor && objectB->collider_type == collider_player_leg) ||
            (objectA->collider_type == collider_player_leg && objectB->collider_type == collider_enemy_sensor))
        {
            if (objectA->collider_type == collider_enemy_sensor)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectA->id);
                enemy_instance->QuitSensorPlayer();
            }
            else if (objectB->collider_type == collider_enemy_sensor)
            {
                EnemyDynamic* enemy_instance = object_manager.FindEnemyDynamicByID(objectB->id);
                enemy_instance->QuitSensorPlayer();
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



        //------------------------------------------------------------------------------------------------------------------------
        //エネミーを吹っ飛ばした時の演出
        if ((objectA->collider_type == collider_blown_away_enemy && objectB->collider_type == collider_effect_sensor) ||
            (objectA->collider_type == collider_effect_sensor && objectB->collider_type == collider_blown_away_enemy)||
            (objectA->collider_type == collider_blown_away_enemy && objectB->collider_type == collider_player_sensor) ||
            (objectA->collider_type == collider_player_sensor && objectB->collider_type == collider_blown_away_enemy))
        {
             //撃墜した時の音
            app_atomex_start(Enemy_Shot_Down_Sound);


            change_enemy_filter_and_body* body_instance;
            // 速度ベクトルを取得
            b2Vec2 velocity;
            b2Vec2 effect_pos;
            if (objectA->collider_type == collider_blown_away_enemy)
            {
                body_instance = object_manager.FindChangeEnemyFilterAndBody(objectA->id);
                effect_pos=fixtureA->GetBody()->GetPosition();
                velocity = fixtureA->GetBody()->GetLinearVelocity();
            }
            else
            {
                body_instance = object_manager.FindChangeEnemyFilterAndBody(objectB->id);
                effect_pos = fixtureB->GetBody()->GetPosition();
                velocity = fixtureB->GetBody()->GetLinearVelocity();
            }

            body_instance->SetDestoryFlag(true);

            // 速度ベクトルから角度（ラジアン）を求める
            float angle_rad = atan2(velocity.y, velocity.x);
            // 90度補正（Y軸基準にする場合）
            angle_rad -= M_PI / 2;


            int  rand = GetRandomInt(1, 3);

            // エフェクトリストに追加
            blown_away_Effects.emplace_back(Blown_Away_Effect(effect_pos, angle_rad, 2, rand));
        }

        //-------------------------------------------------------------------------------------------
        //プレイヤーとUIセンサーが離れた場合
        if ((objectA->collider_type == collider_player_body && objectB->collider_type == collider_UI_block) ||
            (objectA->collider_type == collider_UI_block && objectB->collider_type == collider_player_body))
        {
            if (objectA->collider_type == collider_UI_block)
            {
                UI_block* ui_instance = object_manager.FindUiBlock(objectA->id);
                if (ui_instance->GetIfVideo())
                {
                    ui_instance->SetVideoState(Video_Pause);
                }
                else
                {
                    ui_instance->SetFlag(false);
                }
            }
            else if (objectB->collider_type == collider_UI_block)
            {

                UI_block* ui_instance = object_manager.FindUiBlock(objectB->id);
                if (ui_instance->GetIfVideo())
                {
                    ui_instance->SetVideoState(Video_Pause);
                }
                else
                {
                    ui_instance->SetFlag(false);
                }
            }
        }


        //プレイヤーと傾斜のついたブロックが衝突したかを判定
        if ((objectA->collider_type == collider_player_leg && objectB->object_name == Object_sloping_block) ||
            (objectA->object_name == Object_sloping_block && objectB->collider_type == collider_player_leg))
        {
            if (objectA->object_name == Object_sloping_block)
            {
                sloping_block* sloping_block_instance = object_manager.FindSloping_BlockByID(objectA->id);
                sloping_block_instance->SetPlayerCollided(false);
            }
            else if (objectB->object_name == Object_sloping_block)
            {
                sloping_block* sloping_block_instance = object_manager.FindSloping_BlockByID(objectB->id);
                sloping_block_instance->SetPlayerCollided(false);
            }

        }

        //プレイヤーとバウンドブロックが触れた場合
        if ((objectA->collider_type == collider_player_leg && objectB->collider_type == collider_bound_block) ||
            (objectA->collider_type == collider_bound_block && objectB->collider_type == collider_player_leg))
        {


            if (objectA->collider_type == collider_bound_block)
            {
                boss_bound_block* bound_block_instance = object_manager.FindBossBoundBlock(objectA->id);
                bound_block_instance->SetJumpFlag(false);
              
            }
            else
            {
                boss_bound_block* bound_block_instance = object_manager.FindBossBoundBlock(objectB->id);
                bound_block_instance->SetJumpFlag(false);
             
            }
        }

    }



};


#endif // !CONTACTLIST_H
