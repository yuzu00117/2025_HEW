//-----------------------------------------------------------------------------------------------------
// #name world_.h
// #description worldのポインタをセットしたりしたいだけ
// #make 2024/11/02
// #update 2024/11/22
// #comment 追加・修正予定
//          ・ワールドのセッターゲッター関数
//          ・インスタンスのゲットもある
//          
//----------------------------------------------------------------------------------------------------
#ifndef BOX_2D_WORLD_H
#define BOX_2D_WORLD_H


#define SCREEN_SCALE (32.0f) //ワールドの描画に用いる定数　描画自体に倍率をかける　数値が高いほど寄りになる

#define SCREEN_CENTER_X (0)//プレイヤーを真ん中よりも少し右に表示するために利用している
#define SCREEN_CENTER_Y (0)

//Box2dないの単位調整え利用している
//このBox2dの１の単位は１ｍなので１ｍ＊１ｍのブロックを制作するとでかすぎて、それを動かすために必要な力も膨大な物になる
//そのため、全体に倍率をかけて物体を小さくして、管理しやすいようにしてる。
//そのためこの定数は物体をのサイズを調整するときや、描画時にかけることで調整している
#define BOX2D_SCALE_MANAGEMENT (5.0f)

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


    // **ワールドのリセット**
    void ResetWorld() {
        if (!m_p_world) return;

        // すべてのジョイントを削除
        b2Joint* joint = m_p_world->GetJointList();
        while (joint) {
            b2Joint* nextJoint = joint->GetNext();
            m_p_world->DestroyJoint(joint);
            joint = nextJoint;
        }

        // すべてのボディを削除
        b2Body* body = m_p_world->GetBodyList();
        while (body) {
            b2Body* nextBody = body->GetNext();
            m_p_world->DestroyBody(body);
            body = nextBody;
        }

        world_call_step = false;
        

 
    }

    // **ワールドを完全に作り直す**
    void RecreateWorld() {
        if (m_p_world) {
            delete m_p_world;
        }
        m_p_world = new b2World(b2Vec2(0.0f, 10.f)); // 重力もリセット
      
        world_call_step = true;
    }

    bool GetWorldCallStep(void)
    {
        return world_call_step;
    }


    void SetWorldCallStep(bool flag)
    {
        world_call_step = flag;
    }
 
private:


	//コンストラクタ
	Box2dWorld()
	{
		//ワールドを生成
		m_p_world = new b2World(b2Vec2(0.0f, 10.f));
    
        world_call_step = true;
	}


	//ワールドのポインタ
	b2World* m_p_world = nullptr;
    bool world_call_step = false;
};


#endif //BOX_2D_WORLD_H
