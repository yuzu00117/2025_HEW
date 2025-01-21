//-----------------------------------------------------------------------------------------------------
// #name wood.h
// #description �؂̃I�u�W�F�N�g�̃w�b�_�[
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�Ȃ��Ƃ������[
//			�E����Ȃ�E�����ɓ|�ꂽ���Ƃ����������邩���ˁ[
//----------------------------------------------------------------------------------------------------


#ifndef WOOD_H
#define WOOD_H

#include"include/box2d/box2d.h"
#include"sound.h"
#include"main.h"
#include<list>


class wood
{
public:
	wood(b2Vec2 position,b2Vec2 Woodsize,b2Vec2 AnchorPoint_size,int need_level);
	~wood();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Pulling_wood(b2Vec2 pullingpower);


	// ID ���擾����
	int GetID() const {
		return id;
	}
	
	void SetID(int ID)
	{
		id = ID;
	}

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetWoodSize() const
	{
		return m_Wood_size;
	}

	void SetWoodSize(b2Vec2 size) {
		m_Wood_size = size;
	}


	//body�̏K��
	b2Body* GetObjectWoodBody()
	{
		return Wood_body;
	}

	void SetObjectWoodBody(b2Body* body)
	{
		Wood_body = body;
	}


	bool	GetIfPulling() { return m_pulling; }
	void	SetIfPulling(bool flag) { m_pulling = flag; }

	//�|��鎞�Ԃ������I�u�W�F�N�g�̃��X�g�ɃI�u�W�F�N�g�ǉ�
	void	Add_CollidedObjectWhenFalling_List(b2Vec2 position, ObjectType type);

	///-----------------------------------------------------------------------------
	//�A���J�[�|�C���g


	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetAnchorPointSize() const
	{
		return m_AnchorPoint_size;
	}

	void SetAnchorPointSize(b2Vec2 size) {
		m_AnchorPoint_size = size;
	}

	//body�̏K��
	b2Body* GetObjectAnchorPointBody()
	{
		return AnchorPoint_body;
	}

	void SetObjectAnchorPointBody(b2Body* body)
	{
		AnchorPoint_body = body;
	}

private:
	int id; // �e�C���X�^���X�ŗL�� ID

	b2Body* Wood_body;

	b2Vec2 m_Wood_size;


	b2Body* AnchorPoint_body;

	b2Vec2 m_AnchorPoint_size;

	bool	m_pulling = false;

	//�|��鎞�Ԃ������I�u�W�F�N�g�̏������邽�߂̃N���X
	class ObjectCollided_WhenFalling {
	public:
		b2Vec2	position;
		ObjectType type;
		int	count_down_to_play_sound;
	};

	//�|��鎞�Ԃ������I�u�W�F�N�g��list
	std::list<ObjectCollided_WhenFalling*>object_collided_when_falling;

	//����
	//----------------------------------------
	Sound_Manager m_sound_FalledDown = Object_Wood_Fall_Sound;	//�|�ꂽ���̉�
};
#endif // !WOOD_H
