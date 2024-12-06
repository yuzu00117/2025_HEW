//-----------------------------------------------------------------------------------------------------
// #name sloping_block.h
// #description 傾斜のついたブロックのヘッダー
// #make 2024/12/04　永野義也
// #update 2024/12/04
// #comment 追加・修正予定
//          ・ないとおもうー
//
//----------------------------------------------------------------------------------------------------

#ifndef SLOPING_BLOCK_H
#define SLOPING_BLOCK_H


#include"include/box2d/box2d.h"


enum SlopingBlockAspect //斜面の向きを管理する
{
	right_down,
	right_upper,
	left_down,
	left_upper,
};


class sloping_block
{
public:
	sloping_block(b2Vec2 position, b2Vec2 size, SlopingBlockAspect aspect);
	~sloping_block();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Pulling_wood(b2Vec2 pullingpower);


	// ID を取得する
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}

	//描画用にサイズを持たせておく
	b2Vec2 GetSlopingBlockSize() const
	{
		return m_SlopingBlock_size;
	}

	void SetSlopingBlockSize(b2Vec2 size) {
		m_SlopingBlock_size = size;
	}


	//bodyの習得
	b2Body* GetObjectSlopingBlockBody()
	{
		return SlopingBlock_body;
	}

	void SetObjectSlopingBlockBody(b2Body* body)
	{
		SlopingBlock_body = body;
	}





	void SetBlockAspect(SlopingBlockAspect aspect) 
	{
		m_aspect = aspect;
	}

	SlopingBlockAspect GetBlockAspect()
	{
		return m_aspect;
	}



private:
	int id; // 各インスタンス固有の ID

	b2Body* SlopingBlock_body;

	b2Vec2 m_SlopingBlock_size;

	SlopingBlockAspect m_aspect;
	
};



#endif // !SLOPING_BLOCK_H
