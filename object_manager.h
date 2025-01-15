//-----------------------------------------------------------------------------------------------------
// #name object_manager
// #description �I�u�W�F�N�g���Ǘ����邽�߂̃t�@�N�g���[�̃C���[�W�ɋ߂�
// #make 2024/12/04�@�i��`��
// #update 2024/12/13
// #comment �ǉ��E�C���\��
//          �E�I�u�W�F�N�g����邲�Ƃɐ������銴��
//----------------------------------------------------------------------------------------------------


#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <vector>
#include <memory>
#include "wood.h"
#include"one-way_platform.h"
#include"sloping_block.h"
#include"rock.h"
#include"static_to_dynamic_block.h"
#include"movable_ground.h"
#include"enemy_static.h"
#include"enemy_dynamic.h"
#include"1-1_boss_field_block.h"

// �I�u�W�F�N�g�̎�ނ��`
enum ObjectType {
    NULL_object,
    Object_Wood, // ��
    Object_Rock, // ��
    Object_one_way_platform,//����@�������炵�����Ȃ�
    Object_Static_to_Dynamic,//�ÓI���瓮�I�ɕύX����I�u�W�F�N�g
    Object_Movable_Ground,  //��������鏰 

    
    Object_Enemy_Static,//�ÓI�G�l�~�[
    Object_Enemy_Dynamic,//���I�G�l�~�[
};

class Object{};

// �I�u�W�F�N�g���Ǘ�����N���X
class ObjectManager {
public:
    // �V���O���g���̃C���X�^���X�擾
    static ObjectManager& GetInstance();

    // �؂�ǉ�
    void AddWood(const b2Vec2& position, const b2Vec2& woodSize, const b2Vec2& anchorPointSize,const int&need_level);

    //���ǉ�
    void AddRock(const b2Vec2& position, const float& radius, const int& need_anchor_level);
    //�����ǉ�
    void AddOne_way_platformList(const b2Vec2& position, const b2Vec2& local_position, const b2Vec2 &size);
    //�X�΃u���b�N�̒ǉ�
    void AddSloping_block(const b2Vec2& position, const b2Vec2& size, const SlopingBlockAspect& aspect);
    //�ÓI�����I�̃u���b�N�̒ǉ�
    void AddStatic_to_Dynamic_block(const b2Vec2& position, const b2Vec2& size, const collider_type_Box_or_Circle& collider_type, const int& need_level);
    // ��������鏰��ǉ�
    void AddMovable_Ground(const b2Vec2& position, const b2Vec2& groundSize, const b2Vec2& anchorPointSize, const int& need_level);
    //�ÓI�G�l�~�[����
    void AddEnemyStatic(b2Vec2 position, b2Vec2 body_size, float angle);
    //���I�G�l�~�[����
    void AddEnemyDynamic(b2Vec2 position, b2Vec2 body_size, float angle);

    void AddBossFieldBlock(b2Vec2 position, b2Vec2 body_size, int block_hp, Boss_Room_Level level);

    // ID ���g���Ė؂�����
    wood* FindWoodByID(int id);
    //ID���g���ā@�������
    rock* FindRockByID(int id);
    //ID���g���đ���u���b�N������
    one_way_platform* Findone_way_platformByID(int id);
    //ID���g���ĎΖʃu���b�N������
    sloping_block* FindSloping_BlockByID(int id);
    //ID���g���ĐÓI�����I�u���b�N��ǉ�
    static_to_dynamic_block* FindStatic_to_Dynamic_BlcokID(int id);
    //ID���g���Ĉ�������鏰������
    movable_ground* FindMovable_GroundID(int id);
    //ID���g���ĐÓI�G�l�~�[������
    EnemyStatic* FindEnemyStaticByID(int id);
    //ID���g���ē��I�G�l�~�[������
    EnemyDynamic* FindEnemyDynamicByID(int id);

    boss_field_block* FindBossFieldBlock(int id);
    
    //ID�ƃI�u�W�F�N�g�^�C�v�ŃI�u�W�F�N�g������
    Object* FindObjectByID_ObjectType(int id, ObjectType type);

    //�w��̐ÓI�G�l�~�[���폜
    void DestroyEnemyStatic(int id);
    //�w��̓��I�G�l�~�[���폜
    void DestroyEnemyDynamic(int id);


    // �S�ẴI�u�W�F�N�g��������
    void InitializeAll();

    // �S�ẴI�u�W�F�N�g���X�V
    void UpdateAll();

    // �S�ẴI�u�W�F�N�g��`��
    void DrawAll();

    // �S�ẴI�u�W�F�N�g��j��
    void FinalizeAll();

private:
    std::vector<std::unique_ptr<wood>> woodList;//�؂̃��X�g
    std::vector < std::unique_ptr<rock>>rockList;//��̃��X�g
    std::vector<std::unique_ptr<one_way_platform>> one_way_platformList;// ����̃��X�g
    std::vector<std::unique_ptr<sloping_block>> sloping_blockList;//�Ζʂ̃��X�g
    std::vector<std::unique_ptr<static_to_dynamic_block>> static_to_dynamic_blockList;//�ÓI�����I�u���b�N����
    std::vector<std::unique_ptr<movable_ground>> movable_groundList;//�؂̃��X�g
    std::vector<std::unique_ptr<EnemyStatic>> enemy_staticList;//�ÓI�G�l�~�[�̃��X�g
    std::vector<std::unique_ptr<EnemyDynamic>> enemy_dynamicList;//�ÓI�G�l�~�[�̃��X�g

    std::vector<std::unique_ptr<boss_field_block>> boss_filed_block_List;//�ÓI�G�l�~�[�̃��X�g

    //�����ɃI�u�W�F�N�g���ƂɃ��X�g��ǉ����Ă�������


    ObjectManager() = default;
    ObjectManager(const ObjectManager&) = delete;
    ObjectManager& operator=(const ObjectManager&) = delete;
};

#endif // OBJECT_MANAGER_H
