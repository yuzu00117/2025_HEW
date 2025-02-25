//-----------------------------------------------------------------------------------------------------
// #name spawner_enemy.h
// #description エネミースポナーのヘッダーファイル
// #make 2025/02/21
// #update 2025/02/21
// #comment 追加・修正予定
//          ・
//           
//----------------------------------------------------------------------------------------------------

#ifndef SPAWNER_ENEMY_H
#define SPAWNER_ENEMY_H

#include"world_box2d.h"

class SpawnerEnemy
{
private:
	//オブジェクト固有ID
	int m_id = -999;

	//body
	b2Body* m_body = nullptr;
	//サイズ
	b2Vec2 m_size = b2Vec2(0.0f, 0.0f);

	bool m_in_screen = false;
	int m_spawn_counter = 0;
	const int m_spawn_space = 300;
public:
	SpawnerEnemy(b2Vec2 position, b2Vec2 size, float angle);
	~SpawnerEnemy() = default;

	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	b2Body* GetBody()
	{
		return m_body;
	}
	void SetBody(b2Body* body)
	{
		m_body = body;
	}

	b2Vec2 GetSize()
	{
		return m_size;
	}
	void SetSize(b2Vec2 size)
	{
		m_size = size;
	}

	int GetID()
	{
		return m_id;
	}
	void SetID(int id)
	{
		m_id = id;
	}

	bool GetInScreen()
	{
		return m_in_screen;
	}
	void SetInScreen(bool in_screen)
	{
		m_in_screen = in_screen;
		m_spawn_counter = 0;
	}
};

#endif //SPAWNER_ENEMY_H