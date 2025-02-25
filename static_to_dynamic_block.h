//-----------------------------------------------------------------------------------------------------
// #name static_to_dynamic_block.h
// #description ��̃I�u�W�F�N�g��CPP
// #make 2024/12/06�@�i��`��
// #update 2024/12/06
// #comment �ǉ��E�C���\��
//          �E�A���J�[�����������炻�̂Ԃ��������ÓI�ȃI�u�W�F�N�g���瓮�I�ȃI�u�W�F�N�g�ɂȂ�
//			�E���V�̎��Ƃ����΂ɍ�肽��
//----------------------------------------------------------------------------------------------------

#ifndef STATIC_TO_DYNAMIC_BLOCK_H
#define STATIC_TO_DYNAMIC_BLOCK_H

#include"include/box2d/box2d.h"
#include"sound.h"


enum collider_type_Box_or_Circle
{
	Box_collider,
	Circle_collider
};

class static_to_dynamic_block
{
public:

	static_to_dynamic_block(b2Vec2 Position, b2Vec2 size, collider_type_Box_or_Circle collider_type, int need_anchor_level,bool break_flag);
	//�X�|�i�[���琶����������u���b�N�p�̃R���X�g���N�^
	static_to_dynamic_block(b2Vec2 position, b2Vec2 body_size, int need_level, int id);
	~static_to_dynamic_block();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Change_dynamic();


	// ID ���擾����
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetSize() const
	{
		return object_size;
	}

	void SetSize(b2Vec2 size) {
		object_size = size;
	}


	//body�̏K��
	b2Body* GetObjectBody()
	{
		return object_body;
	}

	void SetObjectBody(b2Body* body)
	{
		object_body = body;
	}

	//�R���C�_�[�̌`���L�����Ă���
	collider_type_Box_or_Circle GetBox_or_Circle()
	{
		return box_or_circle;
	}

	void SetBox_or_Circle(collider_type_Box_or_Circle type)
	{
		box_or_circle = type;
	}


	bool Get_Change_Dynamic_flag()
	{
		return change_dynamic_flag;
	}

	void Set_Change_Dynamic_flag(bool flag)
	{
		change_dynamic_flag = flag;
	}


	void SetNowBreakBlock(bool flag)
	{
		Now_Break_Flag = flag;
	}

	bool	GetIfPulling() { return m_pulling; }
	void	SetIfPulling(bool flag) { m_pulling = flag; }


private:
	int id; // �e�C���X�^���X�ŗL�� ID

	b2Body* object_body;

	b2Vec2 object_size;

	collider_type_Box_or_Circle box_or_circle;

	bool change_dynamic_flag;

	bool	m_pulling = false;


	bool Break_Flag;

	bool Now_Break_Flag=false;

	b2Vec2 prevVelocity;

	bool m_is_border = true;
	int m_need_level = 0;
	float m_border_alpha = 0;
	const float m_border_alpha_max = 0.75f;
	const float m_border_alpha_min = 0.0f;


	bool camera_shake_was = false;
};


#endif // !STATIC_TO_DYNAMIC_BLOCK_H
