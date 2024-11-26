//-----------------------------------------------------------------------------------------------------
// #name world_.h
// #description worldのポインタをセットしたりしたいだけ
// #make 2024/11/02
// #update 2024/11/02
// #comment 追加・修正予定
//          ・ワールドのセッターゲッター関数
//          ・インスタンスのゲットもある
//          
//----------------------------------------------------------------------------------------------------
#ifndef BOX_2D_WORLD_H
#define BOX_2D_WORLD_H


#define SCREEN_SCALE (32.f)

#define SCREEN_CENTER_X (600.0f)
#define SCREEN_CENTER_Y (350.0f)


#define BOX2D_SCALE_MANAGEMENT (5.f)

#include"include/box2d/box2d.h"


class Box2dWorld
{
public:

	//デストラクタ
	~Box2dWorld() {
		if (m_p_world) {
			delete m_p_world;
			m_p_world = nullptr;
		}
	}

	static Box2dWorld& GetInstance() {
		static Box2dWorld instance;
		return instance;
	}

	b2World* GetBox2dWorldPointer()
	{
		return m_p_world;
	}



private:


	//コンストラクタ
	Box2dWorld()
	{
		//ワールドを生成
		m_p_world = new b2World(b2Vec2(0.0f, 10.f));

	}


	//ワールドのポインタ
	b2World* m_p_world = nullptr;
};


#endif //BOX_2D_WORLD_H
