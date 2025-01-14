//-----------------------------------------------------------------------------------------------------
// #name Item_Spirit.h
// #description		�\�E���i�G�����Ƃ��A�C�e���j
// #make 2024/12/28�@���j�S
// #update 2024/12/28
// #comment �ǉ��E�C���\��
//      
//
// 
//----------------------------------------------------------------------------------------------------

#ifndef ITEM_SPIRIT_H
#define	ITEM_SPIRIT_H

#include"include/box2d/box2d.h"

enum SpiritState
{
	Spirit_OnGround,
	Spirit_Falling,
	Spirit_Rising,
	Spirit_Collecting,
	Spirit_Destory,
};

class ItemSpirit
{
public:
	//�Œ�K�v�Ȉ����Fposition�i�ʒu���j�Abody_size�i�T�C�Y�j�Aangle�i��]�p�x�̃��W�A���j
	// �R���C�_�[�̌`�̓f�t�H���g�Ŏl�p�`�A�~�ɂ������ꍇ�� false ��n���A�ύX���Ȃ���Γ��ɒl��n���Ȃ��Ă�������
	// Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������
	ItemSpirit(b2Vec2 position, b2Vec2 body_size, float angle, float recovery, float Alpha = 1.0f);
	~ItemSpirit();

	//�{�f�B�[���擾
	b2Body* GetBody() { return m_body; }
	//�{�f�B�[���Z�b�g
	void SetBody(b2Body* body) { m_body = body; }


	// ID ���擾����
	int GetID() const { return m_ID; }
	// ID ���Z�b�g
	void SetID(int ID) { m_ID = ID; }

	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetSize() const { return m_size; }
	//�`��T�C�Y�Z�b�g
	void SetSize(b2Vec2 size) { m_size = size; }

	//���̏�Ԃ��擾
	SpiritState GetState() { return m_state; }
	//���̏�Ԃ��Z�b�g
	void	SetState(SpiritState state);

	//���߂܂œ������Ă���I�u�W�F�N�g���N�����擾
	const b2Body*	GetRecentCollidedObject() { return m_recent_collided_object; }
	//���߂܂œ������Ă���I�u�W�F�N�g���Z�b�g
	void	SetRecentCollidedObject(b2Body* object) { m_recent_collided_object = object; }

	//�A�C�e�����Q�b�g���ꂽ���̏���
	void	Function();


	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:
	// �e�C���X�^���X�ŗL�� ID
	int m_ID;

	//�A�C�e���̃{�f�B�[
	b2Body* m_body;

	//�A�C�e���̃T�C�Y�i�`��p�j
	b2Vec2 m_size;

	//�A�C�e���̓����x
	float m_Alpha;

	//�\�E���񕜒l
	float m_recovery;

	//���̏��
	SpiritState m_state = Spirit_Falling;

	//���߂܂ŏd�Ȃ��Ă���I�u�W�F�N�g�i���邢�͒n�ʁj
	const b2Body* m_recent_collided_object = nullptr;

};

#endif // !ITEM_SPIRIT_H