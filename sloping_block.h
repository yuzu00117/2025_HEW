//-----------------------------------------------------------------------------------------------------
// #name sloping_block.h
// #description �X�΂̂����u���b�N�̃w�b�_�[
// #make 2024/12/04�@�i��`��
// #update 2024/12/04
// #comment �ǉ��E�C���\��
//          �E�Ȃ��Ƃ������[
//
//----------------------------------------------------------------------------------------------------

#ifndef SLOPING_BLOCK_H
#define SLOPING_BLOCK_H


#include"include/box2d/box2d.h"


enum SlopingBlockAspect //�Ζʂ̌������Ǘ�����
{
	right_down,
	right_upper,
	left_down,
	left_upper,
};


class sloping_block
{
public:
	sloping_block(b2Vec2 position, b2Vec2 size, SlopingBlockAspect aspect);
	~sloping_block();



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
	b2Vec2 GetSlopingBlockSize() const
	{
		return m_SlopingBlock_size;
	}

	void SetSlopingBlockSize(b2Vec2 size) {
		m_SlopingBlock_size = size;
	}


	//body�̏K��
	b2Body* GetObjectSlopingBlockBody()
	{
		return SlopingBlock_body;
	}

	void SetObjectSlopingBlockBody(b2Body* body)
	{
		SlopingBlock_body = body;
	}





	void SetBlockAspect(SlopingBlockAspect aspect) 
	{
		m_aspect = aspect;
	}

	SlopingBlockAspect GetBlockAspect()
	{
		return m_aspect;
	}



private:
	int id; // �e�C���X�^���X�ŗL�� ID

	b2Body* SlopingBlock_body;

	b2Vec2 m_SlopingBlock_size;

	SlopingBlockAspect m_aspect;
	
};



#endif // !SLOPING_BLOCK_H
