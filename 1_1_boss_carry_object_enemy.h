//-----------------------------------------------------------------------------------------------------
// #name boss_carry_object_enemy.h
// #description　ボスの上空をプカプカしてるエネミーオブジェクトを持ってて落とせる
// #make 2025/02/01　永野義也
// #update 2025/02/01
// #comment 追加・修正予定
//          
//----------------------------------------------------------------------------------------------------

#ifndef BOSS_CARRY_OBJECT_ENEMY_H
#define BOSS_CARRY_OBJECT_ENEMY_H

#include "include/box2d/box2d.h"
#include"1-1_boss_field_block.h"
#include<vector>

class boss_carry_object_enemy
{
public:
    boss_carry_object_enemy(b2Vec2 position, b2Vec2 enemy_size, bool left, float enemy_speed, b2Vec2 object_size,int object_type, int anchor_level);
    ~boss_carry_object_enemy();

    void Initialize();
    void Update();
    void Draw();
    void Finalize();
    void Destroy_Splitting();
    bool IsDestroyed() const { return destroyed; }

    void DestroySplittedBodies(std::vector<b2Body*>& bodyList);

    void AnchorHit();

    void Destroy_Body();

    // ID を取得する
    int GetID() const {
        return id;
    }

    void SetID(int ID)
    {
        id = ID;
    }


    //ボディを取得
    b2Body* GetEnemyBody(void)
    {
        return enemy_body;
    }

    //ボディをセット
    void SetEnemyBody(b2Body* body)
    {
        enemy_body = body;
    }



    //サイズを取得
    b2Vec2 GetEnemySize(void)
    {
        return m_enemy_size;
    }
    //サイズをセット
    void SetEnemySize(b2Vec2 size)
    {
        m_enemy_size = size;
    }



    //ボディを取得
    b2Body* GetObjectBody(void)
    {
        return object_body;
    }

    //ボディをセット
    void SetObjectBody(b2Body* body)
    {
        object_body = body;
    }



    //サイズを取得
    b2Vec2 GetObjectSize(void)
    {
        return m_object_size;
    }
    //サイズをセット
    void SetObjectSize(b2Vec2 size)
    {
        m_object_size = size;
    }


    void SetAnchorHItFlag(bool flag)
    {
        Anchor_Hit_flag = flag;
    }

    void SetSplittingDestroyFlag(bool flag)
    {
        Splitting_Destroy_Flag = flag;
    }

private:
    bool isUse = false;

    b2Body* enemy_body;
    b2Vec2 m_enemy_size;
   
    b2Body* object_body;
    b2Vec2 m_object_size;

    float enemy_speed;//横に移動するスピード

    bool left_flag;//左にすすむかどうか

    int object_type;//テクスチャのタイプ


    bool Anchor_Hit_flag=false;

    int id;
    bool destroyed = false;

    int needlevel;

    int lifetime = 0;

    int enemy_texture_cnt;

    int Destroy_Cnt=0;

    std::vector<b2Body*> boss_Object_body_Splitting;

    bool Splitting_Destroy_Flag;//分割するフラグ
    bool Splitting_end;//分割が終わった
    int Splitting_x;
    int Splitting_y;

};

#endif // BOSS_CARRY_OBJECT_ENEMY_H