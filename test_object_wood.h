#pragma once

#include"include/box2d/box2d.h"
#include"test_object_wood.h"
#include"include/box2d/box2d.h"

#define ONE_OBJECT_MAX_BODYS (10)

class test_wood
{
public:
	test_wood(b2Vec2 position);
	~test_wood();


	static void Initialize();
	static void Update();
	static void Draw();
	static void Finalize();

	//ï`âÊópÇ…ÉTÉCÉYÇéùÇΩÇπÇƒÇ®Ç≠
	b2Vec2 GetSize(int index) const
	{
		return m_size[index];
	}

	void SetSize(b2Vec2 size,int index) {
		m_size[index] = size;
	}


	//bodyÇÃèKìæ
	b2Body* GetObjectBody(int Index)
	{
		return bodys[Index];
	}

	void SetObjectBody(b2Body*body,int Index)
	{
		bodys[Index] = body;
	}

private:
	
	b2Body* bodys[ONE_OBJECT_MAX_BODYS];//ç≈ëÂÇPÇOÇ©Ç»

	b2Vec2 m_size[ONE_OBJECT_MAX_BODYS];
};

