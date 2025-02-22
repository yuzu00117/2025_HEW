//-----------------------------------------------------------------------------------------------------
// #name teleport_block.h
// #description 接触したら指定した場所に飛ぶブロック
// #make 2024/01/02
// #update 2024/01/02
// #comment 追加・修正予定
//         
//          
//----------------------------------------------------------------------------------------------------

#ifndef TELEPORT_BLOCK_H
#define TELEPORT_BLOCK_H

#include"include/box2d/box2d.h"

class teleport_block
{
public:
	teleport_block(b2Vec2 Position, b2Vec2 teleport_block_size, b2Vec2 to_position);
	~teleport_block();


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

	//描画用にサイズを持たせておく
	b2Vec2 GetTeleportBlockSize() const
	{
		return m_size;
	}

	void SetTeleportBlockSize(b2Vec2 size) {
		m_size = size;
	}


	//bodyの習得
	b2Body* GetTeleportBlockBody()
	{
		return m_body;
	}

	void SetTeleportBlockBody(b2Body* body)
	{
		m_body = body;
	}


	bool GetTeleportFlag(void)
	{
		return teleport_flag;
	}

	void SetTeleportFlag(bool flag)
	{
		teleport_flag = flag;
	}


	void SetTeleportPoint(b2Vec2 Position)
	{
		teleport_point = Position;
	}

	b2Vec2 GetTeleportPoint(void)
	{
		return teleport_point;
	}

private:

	b2Body* m_body;

	b2Vec2 m_size;

	b2Vec2 teleport_point;

	int id;

	bool teleport_flag=false;

};


#endif // !TELEPORT_BLOCK_H
