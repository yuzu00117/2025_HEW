//-----------------------------------------------------------------------------------------------------
//#name boss_carry_object_enemy_spawner.h
// #description　ボスの上空をプカプカしてるエネミーオブジェクトを持ってて落とせるのスポナー
// #make 2025/02/01　永野義也
// #update 2025/02/01
// #comment 追加・修正予定
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

    void SpawnEnemy(); // エネミーを生成する関数


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


	// ID を取得する
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
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


    b2Body* m_body;
    b2Vec2 m_size;
    Boss_Room_Level BossRoomLevel;

	bool left_flag=false;

    int spawnTimer = 0;
    int spawnIntervalFrames = 600; // 10秒ごとにエネミー生成

    std::vector<boss_carry_object_enemy*> enemyList; // 生成されたエネミーを管理
};

#endif // BOSS_CARRY_OBJECT_SPAWNER_H

