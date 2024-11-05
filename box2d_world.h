#ifndef BOX_2D_WORLD_H

#define BOX_2D_WORLD_H


#include"include/box2d/box2d.h"
//-----------------------------------------------------------------------------------------------------
// #name box2d_world.h
// #description worldのポインタをセットしたりしたいだけ
// #make 2024/11/02
// #update 2024/11/02
// #comment 追加・修正予定
//          ・ワールドのセッターゲッター関数
//          ・インスタンスのゲットもある
//          
//----------------------------------------------------------------------------------------------------


class Box2dWorld
{
public:
	//コンストラクタ
	Box2dWorld()
	{

	}

	//デストラクタ
	~Box2dWorld() {

	}

	static Box2dWorld& GetInstance() {
		static Box2dWorld instance;
		return instance;
	}

	b2World* GetBox2dWorldPointer()
	{
		return m_p_world;
	}

	void SetBox2dWorldPointer(b2World* b2world_pointer)
	{
		this->m_p_world = b2world_pointer;
	}

private:

	//ワールドのポインタ
	b2World* m_p_world=nullptr;
};


#endif //BOX_2D_WORLD_H