//-----------------------------------------------------------------------------------------------------
// #name rock.h
// #description ��̃I�u�W�F�N�g�̃w�b�_�[
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�Ȃ��Ƃ������[
//			�E����Ȃ�E�����ɓ|�ꂽ���Ƃ����������邩���ˁ[
//----------------------------------------------------------------------------------------------------


#ifndef ROCK_H
#define ROCK_H

#include"include/box2d/box2d.h"




class rock
{
public:

	rock(b2Vec2 Position, float radius, int need_anchor_level);
	~rock();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Pulling_rock(b2Vec2 pullingpower);


	// ID ���擾����
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetRockSize() const
	{
		return m_Rock_size;
	}

	void SetRockSize(b2Vec2 size) {
		m_Rock_size = size;
	}


	//body�̏K��
	b2Body* GetObjectRockBody()
	{
		return Rock_body;
	}

	void SetObjectRockBody(b2Body* body)
	{
		Rock_body = body;
	}




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

	b2Body* Rock_body;

	b2Vec2 m_Rock_size;


	b2Body* AnchorPoint_body;

	b2Vec2 m_AnchorPoint_size;
};
#endif // !WOOD_H