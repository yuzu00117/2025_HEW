//-----------------------------------------------------------------------------------------------------
//#name boss_carry_object_enemy_spawner.h
// #description�@�{�X�̏����v�J�v�J���Ă�G�l�~�[�I�u�W�F�N�g�������Ăė��Ƃ���̃X�|�i�[
// #make 2025/02/01�@�i��`��
// #update 2025/02/01
// #comment �ǉ��E�C���\��
//          
//----------------------------------------------------------------------------------------------------
#ifndef BOSS_CARRY_OBJECT_SPAWNER_H
#define BOSS_CARRY_OBJECT_SPAWNER_H

#include "include/box2d/box2d.h"
#include"1_1_boss_carry_object_enemy.h"
#include <vector>

class boss_carry_object_spawner
{
public:
    boss_carry_object_spawner(b2Vec2 position, b2Vec2 Size, Boss_Room_Level level,bool left);
    ~boss_carry_object_spawner();

    void Initialize();
    void Update();
    void Draw();
    void Finalize();

    void SpawnEnemy(); // �G�l�~�[�𐶐�����֐�


	//�{�f�B���擾
	b2Body* GetBody(void)
	{
		return m_body;
	}

	//�{�f�B���Z�b�g
	void SetBody(b2Body* body)
	{
		m_body = body;
	}


	// ID ���擾����
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}



	//�T�C�Y���擾
	b2Vec2 GetSize(void)
	{
		return m_size;
	}
	//�T�C�Y���Z�b�g
	void SetSize(b2Vec2 size)
	{
		m_size = size;
	}

private:

	int id;


    b2Body* m_body;
    b2Vec2 m_size;
    Boss_Room_Level BossRoomLevel;

	bool left_flag=false;

    int spawnTimer = 0;
    int spawnIntervalFrames = 600; // 10�b���ƂɃG�l�~�[����

    std::vector<boss_carry_object_enemy*> enemyList; // �������ꂽ�G�l�~�[���Ǘ�
};

#endif // BOSS_CARRY_OBJECT_SPAWNER_H

