//-----------------------------------------------------------------------------------------------------
// #name world_.h
// #description world�̃|�C���^���Z�b�g�����肵��������
// #make 2024/11/02
// #update 2024/11/22
// #comment �ǉ��E�C���\��
//          �E���[���h�̃Z�b�^�[�Q�b�^�[�֐�
//          �E�C���X�^���X�̃Q�b�g������
//          
//----------------------------------------------------------------------------------------------------
#ifndef BOX_2D_WORLD_H
#define BOX_2D_WORLD_H


#define SCREEN_SCALE (32.f) //���[���h�̕`��ɗp����萔�@�`�掩�̂ɔ{����������@���l�������قǊ��ɂȂ�

#define SCREEN_CENTER_X (600.0f)//�v���C���[��^�񒆂��������E�ɕ\�����邽�߂ɗ��p���Ă���
#define SCREEN_CENTER_Y (350.0f)

//Box2d�Ȃ��̒P�ʒ��������p���Ă���
//����Box2d�̂P�̒P�ʂ͂P���Ȃ̂łP�����P���̃u���b�N�𐧍삷��Ƃł������āA����𓮂������߂ɕK�v�ȗ͂��c��ȕ��ɂȂ�
//���̂��߁A�S�̂ɔ{���������ĕ��̂����������āA�Ǘ����₷���悤�ɂ��Ă�B
//���̂��߂��̒萔�͕��̂��̃T�C�Y�𒲐�����Ƃ���A�`�掞�ɂ����邱�ƂŒ������Ă���
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
