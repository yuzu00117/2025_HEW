
//-----------------------------------------------------------------------------------------------------
// #name square_collider.h
// #description �l�p�̂����蔻�����肽��
// #make 2024/11/03
// #update 2024/11/03
// #comment �ǉ��E�C���\��
//          �E�l�p�̂����蔻�����������
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

	// �R���X�g���N
	SquareCollider(
		XMFLOAT2 potion = ZERO2f,
		XMFLOAT2 size = XMFLOAT2(1.f, 1.f),
		float angle = 0,
		bool bFixed = false //���I���ÓI��

	);


	// �f�X�g���N�^
	virtual ~SquareCollider(void) {
		if (m_p_body && m_p_world) {
			m_p_world->DestroyBody(m_p_body); // b2World�o�R��b2Body���폜
			m_p_body = nullptr;
		}
	}

	
	

	

	//colider �̃^�C�v�̃Z�b�g�֐�
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
	
	//collider�̃^�C�v
	ColliderType m_p_collider_type;

	
};
#endif // SQUERA_COLIDER