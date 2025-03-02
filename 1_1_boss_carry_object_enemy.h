//-----------------------------------------------------------------------------------------------------
// #name boss_carry_object_enemy.h
// #description�@�{�X�̏����v�J�v�J���Ă�G�l�~�[�I�u�W�F�N�g�������Ăė��Ƃ���
// #make 2025/02/01�@�i��`��
// #update 2025/02/01
// #comment �ǉ��E�C���\��
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

    // ID ���擾����
    int GetID() const {
        return id;
    }

    void SetID(int ID)
    {
        id = ID;
    }


    //�{�f�B���擾
    b2Body* GetEnemyBody(void)
    {
        return enemy_body;
    }

    //�{�f�B���Z�b�g
    void SetEnemyBody(b2Body* body)
    {
        enemy_body = body;
    }



    //�T�C�Y���擾
    b2Vec2 GetEnemySize(void)
    {
        return m_enemy_size;
    }
    //�T�C�Y���Z�b�g
    void SetEnemySize(b2Vec2 size)
    {
        m_enemy_size = size;
    }



    //�{�f�B���擾
    b2Body* GetObjectBody(void)
    {
        return object_body;
    }

    //�{�f�B���Z�b�g
    void SetObjectBody(b2Body* body)
    {
        object_body = body;
    }



    //�T�C�Y���擾
    b2Vec2 GetObjectSize(void)
    {
        return m_object_size;
    }
    //�T�C�Y���Z�b�g
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

    float enemy_speed;//���Ɉړ�����X�s�[�h

    bool left_flag;//���ɂ����ނ��ǂ���

    int object_type;//�e�N�X�`���̃^�C�v


    bool Anchor_Hit_flag=false;

    int id;
    bool destroyed = false;

    int needlevel;

    int lifetime = 0;

    int enemy_texture_cnt;

    int Destroy_Cnt=0;

    std::vector<b2Body*> boss_Object_body_Splitting;

    bool Splitting_Destroy_Flag;//��������t���O
    bool Splitting_end;//�������I�����
    int Splitting_x;
    int Splitting_y;

};

#endif // BOSS_CARRY_OBJECT_ENEMY_H