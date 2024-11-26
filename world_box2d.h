//-----------------------------------------------------------------------------------------------------
// #name world_.h
// #description world�̃|�C���^���Z�b�g�����肵��������
// #make 2024/11/02
// #update 2024/11/02
// #comment �ǉ��E�C���\��
//          �E���[���h�̃Z�b�^�[�Q�b�^�[�֐�
//          �E�C���X�^���X�̃Q�b�g������
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

	//�f�X�g���N�^
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


	//�R���X�g���N�^
	Box2dWorld()
	{
		//���[���h�𐶐�
		m_p_world = new b2World(b2Vec2(0.0f, 10.f));

	}


	//���[���h�̃|�C���^
	b2World* m_p_world = nullptr;
};


#endif //BOX_2D_WORLD_H
