//-----------------------------------------------------------------------------------------------------
// #name boss_field_block.h
// #description ボス戦部屋のフィールドを生成するH
// #make 2025/01/14　永野義也
// #update 2025/01/14
// #comment 追加・修正予定
//          
//----------------------------------------------------------------------------------------------------

#ifndef BOSS_FIELD_BLOCK_H
#define BOSS_FIELD_BLOCK_H
#include"include/box2d/box2d.h"


enum Boss_Room_Level
{
	null,
	boss_room_level_1,
	boss_room_level_2,
	boss_room_level_3,
};

class boss_field_block
{
public:
	boss_field_block(b2Vec2 position, b2Vec2 size,  int block_hp, Boss_Room_Level level);
	~boss_field_block();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();



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

	b2Body* m_body;

	b2Vec2 m_size;

	int Block_Hp;

	Boss_Room_Level BossRoomLevel;


};



#endif // BOSS_FIELD_BLOCK_H
