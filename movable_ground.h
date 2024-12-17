#pragma once

#ifndef MOVABLE_GROUND_H
#define MOVABLE_GROUND_H

#include"include/box2d/box2d.h"


class movable_ground
{
public:
	movable_ground(b2Vec2 position, b2Vec2 Ground_size, b2Vec2 AnchorPoint_size, int need_level);
	~movable_ground();



	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	void Pulling_ground(b2Vec2 pullingpower);


	// ID ���擾����
	int GetID() const {
		return id;
	}

	void SetID(int ID)
	{
		id = ID;
	}

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetGroundSize() const
	{
		return m_Ground_size;
	}

	void SetGroundSize(b2Vec2 size) {
		m_Ground_size = size;
	}


	//body�̏K��
	b2Body* GetObjectGroundBody()
	{
		return Ground_body;
	}

	void SetObjectGroundBody(b2Body* body)
	{
		Ground_body = body;
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

	b2Body* Ground_body;

	b2Vec2 m_Ground_size;


	b2Body* AnchorPoint_body;

	b2Vec2 m_AnchorPoint_size;

};

#endif // !MOVABLE_GROUND_H

