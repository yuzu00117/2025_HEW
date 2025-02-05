#ifndef CONTACT_BLOCK_H
#define CONTACT_BLOCK_H

#include"include/box2d/box2d.h"



enum Contact_Block_Type
{
	NULL_TYPE,		//なにもないやつ
	DEAD_BLOCK_TYPE,//触れたら死ぬ

};



class contact_block
{
public:
	contact_block(b2Vec2 Position, b2Vec2 block_size, Contact_Block_Type type, b2Vec2 num);
	~contact_block();


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
	b2Vec2 GetSize() const
	{
		return m_size;
	}

	void SetSize(b2Vec2 size) {
		m_size = size;
	}


	//bodyの習得
	b2Body* GetBody()
	{
		return m_body;
	}

	void SetBody(b2Body* body)
	{
		m_body = body;
	}



	bool GetFlag(void)
	{
		return m_flag;
	}


	void SetFlag(bool flag)
	{
		m_flag = flag;
	}

private:

	b2Body* m_body;

	b2Vec2 m_size;

	

	int id;


	bool m_flag = false;

	

};


#endif // !CONTACT_BLOCK_H
