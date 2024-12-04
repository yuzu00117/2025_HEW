#ifndef WOOD_H
#define WOOD_H

#include"include/box2d/box2d.h"




class wood
{
public:
	wood(b2Vec2 position,b2Vec2 Woodsize,b2Vec2 AnchorPoint_size);
	~wood();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	//描画用にサイズを持たせておく
	b2Vec2 GetWoodSize() const
	{
		return m_Wood_size;
	}

	void SetWoodSize(b2Vec2 size) {
		m_Wood_size = size;
	}


	//bodyの習得
	b2Body* GetObjectWoodBody()
	{
		return Wood_body;
	}

	void SetObjectWoodBody(b2Body* body)
	{
		Wood_body = body;
	}




	///-----------------------------------------------------------------------------
	//アンカーポイント


	//描画用にサイズを持たせておく
	b2Vec2 GetAnchorPointSize() const
	{
		return m_AnchorPoint_size;
	}

	void SetAnchorPointSize(b2Vec2 size) {
		m_AnchorPoint_size = size;
	}

	//bodyの習得
	b2Body* GetObjectAnchorPointBody()
	{
		return AnchorPoint_body;
	}

	void SetObjectAnchorPointBody(b2Body* body)
	{
		AnchorPoint_body = body;
	}

private:

	b2Body* Wood_body;

	b2Vec2 m_Wood_size;


	b2Body* AnchorPoint_body;

	b2Vec2 m_AnchorPoint_size;
};
#endif // !WOOD_H
