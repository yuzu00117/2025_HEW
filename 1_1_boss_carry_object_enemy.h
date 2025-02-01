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

#include"include/box2d/box2d.h"
#include"1-1_boss_field_block.h"
#include<vector>


struct EnemyObjectPair {
	b2Body* enemyBody;
	b2Body* objectBody;
	b2Vec2 enemySize;
	b2Vec2 objectSize;
	float lifetime;
};

class boss_carry_object_enemy
{
public:
	boss_carry_object_enemy(b2Vec2 position, b2Vec2 size, Boss_Room_Level level, b2Vec2 enemy_size, b2Vec2 enemy_speed, b2Vec2 max_obejct_size, int object_need_levl);
	~boss_carry_object_enemy();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void EnemyUpdate();

	void CreateEnemyBodyandObjectBody(b2Vec2 object_size, b2Vec2 enemy_size);

	



	// ID ���擾����
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}


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

	//�X�|�i�[�̃{�f�B
	b2Body* m_body;
	b2Vec2 m_size;


	int spawnTimer = 0;  // �����p�^�C�}�[�i�t���[���P�ʁj
	int spawnIntervalFrames = 600;  // 10�b���ƂɃG�l�~�[�����i60FPS �~ 10�b�j

	std::vector<EnemyObjectPair> enemyObjectPairs; // �G�ƃI�u�W�F�N�g�̃Z�b�g
	int maxLifetimeFrames = 4000; // �폜�����܂ł̃t���[���� (��: 300�t���[�� = 5�b @ 60FPS)


	b2Vec2 Enemy_Speed;

	b2Vec2 Max_object_size;


	Boss_Room_Level BossRoomLevel;



	int body_delete_cnt = 0;

	bool break_flag = false;

	int Object_need_level;


};


#endif // !BOSS_CARRY_OBJECT_ENEMY_H
