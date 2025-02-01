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

	



	// ID を取得する
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}


	//ボディを取得
	b2Body* GetBody(void)
	{
		return m_body;
	}

	//ボディをセット
	void SetBody(b2Body* body)
	{
		m_body = body;
	}



	//サイズを取得
	b2Vec2 GetSize(void)
	{
		return m_size;
	}
	//サイズをセット
	void SetSize(b2Vec2 size)
	{
		m_size = size;
	}




private:

	int id;

	//スポナーのボディ
	b2Body* m_body;
	b2Vec2 m_size;


	int spawnTimer = 0;  // 生成用タイマー（フレーム単位）
	int spawnIntervalFrames = 600;  // 10秒ごとにエネミー生成（60FPS × 10秒）

	std::vector<EnemyObjectPair> enemyObjectPairs; // 敵とオブジェクトのセット
	int maxLifetimeFrames = 4000; // 削除されるまでのフレーム数 (例: 300フレーム = 5秒 @ 60FPS)


	b2Vec2 Enemy_Speed;

	b2Vec2 Max_object_size;


	Boss_Room_Level BossRoomLevel;



	int body_delete_cnt = 0;

	bool break_flag = false;

	int Object_need_level;


};


#endif // !BOSS_CARRY_OBJECT_ENEMY_H
