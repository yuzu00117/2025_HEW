//-----------------------------------------------------------------------------------------------------
// #name spawner_block_damage.h
// #description 壊れるブロックのスポナーのヘッダーファイル
// #make 2025/02/24
// #update 2025/02/24
// #comment 追加・修正予定
//          ・
//           
//----------------------------------------------------------------------------------------------------

#ifndef SPAWNER_BLOCK_DAMAGE_H
#define SPAWNER_BLOCK_DAMAGE_H

#include"world_box2d.h"

class SpawnerBlockDamage
{
private:
	//オブジェクト固有ID
	int m_id = -999;

	//body
	b2Body* m_body = nullptr;
	//サイズ
	b2Vec2 m_size = b2Vec2(0.0f, 0.0f);

	int m_need_level = 0;
public:
	SpawnerBlockDamage(b2Vec2 position, b2Vec2 size, float angle, int need_level);
	~SpawnerBlockDamage() = default;

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
};

#endif //SPAWNER_BLOCK_DAMAGE_H