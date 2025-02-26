//-----------------------------------------------------------------------------------------------------
// #name Item_Barrier.h
// #description �o���A
// #make 2025/2/26�@���j�S
// #update 2025/2/26
// #comment �ǉ��E�C���\��
//      
//
// 
//----------------------------------------------------------------------------------------------------
#ifndef ITEM_BARRIER_H
#define ITEM_BARRIER_H

#include"include/box2d/box2d.h"


enum BarrierState
{
	Barrier_Start,		// �o���A�𒣂�
	Barrier_Loop,		// �o���A������
	Barrier_Break,	// �o���A���j�󂳂ꂽ
};

class ItemBarrier
{
public:
	//�Œ�K�v�Ȉ����Fposition�i�ʒu���j�Abody_size�i�T�C�Y�j�Aangle�i��]�p�x�̃��W�A���j
	// �R���C�_�[�̌`�̓f�t�H���g�Ŏl�p�`�A�~�ɂ������ꍇ�� false ��n���A�ύX���Ȃ���Γ��ɒl��n���Ȃ��Ă�������
	// Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������
	ItemBarrier(b2Vec2 position, b2Vec2 body_size, float angle, const b2Body* owner_body, float Alpha = 1.0f);
	~ItemBarrier();

	//�{�f�B�[���擾
	b2Body* GetBody() { return m_body; }
	b2Body* GetBody() const { return m_body; }
	//�{�f�B�[���Z�b�g
	void SetBody(b2Body* body) { m_body = body; }


	// ID ���擾����
	int GetID() const { return m_ID; }
	// ID ���Z�b�g
	void SetID(int ID) { m_ID = ID; }

	//�@�o���A�𒣂����{�l���擾
	const b2Body* GetOwnerBody() { return m_owner; }

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetSize() const { return m_size; }
	//�`��T�C�Y�Z�b�g
	void SetSize(b2Vec2 size) { m_size = size; }


	//�o���A�̏�Ԃ��擾
	BarrierState GetState() { return m_state; }
	//�o���A�̏�Ԃ��Z�b�g
	void SetState(BarrierState state){
		m_state = state;
		m_effect_cnt = 0;
		switch (m_state)
		{
		case Barrier_Start:
			m_texture_total_cols = 8;
			m_texture_total_rows = 6;
			break;
		case Barrier_Loop:
			m_texture_total_cols = 8;
			m_texture_total_rows = 6;
			break;
		case Barrier_Break:
			m_texture_total_cols = 5;
			m_texture_total_rows = 4;
			break;
		default:
			break;
		}
	}

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	// �e�C���X�^���X�ŗL�� ID
	int m_ID;

	//�Z�[�u�|�C���g�̃{�f�B�[
	b2Body* m_body;

	//�G�t�F�N�g��\������
	float m_effect_cnt;

	//�Z�[�u�|�C���g�̃T�C�Y�i�`��p�j
	b2Vec2 m_size;

	//�Z�[�u�|�C���g�̓����x
	float m_Alpha;

	BarrierState m_state;

	//�o���A�𒣂����{�l��body���
	const b2Body* m_owner;

	//�摜�̏c��
	int m_texture_total_cols;
	//�摜�̉���
	int m_texture_total_rows;
};


#endif // !ITEM_BARRIER_H

