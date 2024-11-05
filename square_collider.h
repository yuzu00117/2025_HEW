
//-----------------------------------------------------------------------------------------------------
// #name square_collider.h
// #description 四角のあたり判定を作りたい
// #make 2024/11/03
// #update 2024/11/03
// #comment 追加・修正予定
//          ・四角のあたり判定を持たせる
//          
//----------------------------------------------------------------------------------------------------
#ifndef SQUERA_COLIDER
#define SQUERA_COLIDER

#include"include/box2d/box2d.h"
#include"collider_type.h"
#include"tool.h"
#include"sprite.h"



class SquareCollider
{
public:

	// コンストラク
	SquareCollider(
		XMFLOAT2 potion = ZERO2f,
		XMFLOAT2 size = XMFLOAT2(1.f, 1.f),
		float angle = 0,
		bool bFixed = false //動的か静的か

	);


	// デストラクタ
	virtual ~SquareCollider(void) {
		if (m_p_body && m_p_world) {
			m_p_world->DestroyBody(m_p_body); // b2World経由でb2Bodyを削除
			m_p_body = nullptr;
		}
	}

	
	

	

	//colider のタイプのセット関数
	void SetColliderType(ColliderType type)
	{
		m_p_collider_type = type;
	}


	ColliderType GetColliderType(void)
	{
		return m_p_collider_type;
	}

	b2Body *GetColliderBody(void)
	{
		return m_p_body;
	}
	XMFLOAT2 Get_m_size(void)
	{
		return m_p_size;
	}

private:
	
	b2Body* m_p_body;
	XMFLOAT2 m_p_size;
	b2World* m_p_world;
	
	//colliderのタイプ
	ColliderType m_p_collider_type;

	
};
#endif // SQUERA_COLIDER