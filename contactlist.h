#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include"include/box2d/box2d.h"
#include"collider_type.h"




class MyContactListener : public b2ContactListener {
public:
   


    // シングルトンのインスタンスを取得する
    static MyContactListener& GetInstance() {
        static MyContactListener instance;
        return instance;
    }




    // 衝突した瞬間
    void BeginContact(b2Contact* contact) override {


        // 衝突したフィクスチャを取得
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        // それぞれのボディからユーザーデータを取得
          // ボディのユーザーデータを取得
        auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);
        auto* objectB = reinterpret_cast<ObjectData*>(fixtureB->GetUserData().pointer);

        // プレーヤーと地面が衝突したかを判定
        if ((objectA->collider_type == collider_player && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_player)) {
            // 衝突処理（プレーヤーと地面が接触した時）
            
            int i = 0;
            // 必要な処理をここに記述
        }
        
     
    }

    // 衝突終了時
    void EndContact(b2Contact* contact) override {
        // 衝突したフィクスチャを取得
        // 衝突したフィクスチャを取得
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();

        // それぞれのボディからユーザーデータを取得
          // ボディのユーザーデータを取得
        auto* objectA = reinterpret_cast<ObjectData*>(fixtureA->GetUserData().pointer);
        auto* objectB = reinterpret_cast<ObjectData*>(fixtureB->GetUserData().pointer);

        // プレーヤーと地面が衝突したかを判定
        if ((objectA->collider_type == collider_player && objectB->collider_type == collider_ground) ||
            (objectA->collider_type == collider_ground && objectB->collider_type == collider_player)) {
            // 衝突処理（プレーヤーと地面が接触した時）

            int i = 0;

            // 必要な処理をここに記述
        }
    }

};


#endif // !CONTACTLIST_H