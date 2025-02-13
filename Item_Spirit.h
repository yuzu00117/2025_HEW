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
#include<string>
#include<list>

enum SpiritType
{
	Spirit_L,
	Spirit_M,
	Spirit_S,
};

enum SpiritState
{
	Spirit_Idle,	//�n�ʂɒ����Ă���
	Spirit_Rising,	//�㏸���Ă���
	Spirit_Falling,	//�㏸��̗����i�I�u�W�F�N�g�Ɨ��ꂽ�u�Ԃ̍��W�܂ŗ����j
	Spirit_Collecting,	//�v���C���[�ɉ�����ꂢ�Ă���r��
	Spirit_Destory,		//���ꂩ��������\��
};

class ItemSpirit
{
public:
	//�Œ�K�v�Ȉ����Fposition�i�ʒu���j�Abody_size�i�T�C�Y�j�Aangle�i��]�p�x�̃��W�A���j
	// �R���C�_�[�̌`�̓f�t�H���g�Ŏl�p�`�A�~�ɂ������ꍇ�� false ��n���A�ύX���Ȃ���Γ��ɒl��n���Ȃ��Ă�������
	// Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������
	ItemSpirit(b2Vec2 position, b2Vec2 body_size, float angle, SpiritType type, float Alpha = 1.0f);
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


	//�������Ă���I�u�W�F�N�g��ǉ�
	void	AddCollidedObject(b2Body* object) { m_CollidedObject.push_back(object); }
	//���߂܂œ������Ă���I�u�W�F�N�g���N�����擾
	const b2Body* FindLeastCollidedObject() {
		return m_CollidedObject.back();
	}
	//�������܂œ������Ă����I�u�W�F�N�g������
	void	DeleteCollidedObject(b2Body* object) {
		if (!(m_CollidedObject.begin() == m_CollidedObject.end()) && m_state != Spirit_Collecting)
		{
			m_CollidedObject.remove_if([object](b2Body* p) { return p == object; });

			if (m_CollidedObject.size() == 0)
			{
				SetState(Spirit_Falling);
				//�����ꂽ�u�Ԃ̃\�E���̍��W�𗎉��̏I�_�ɂ���
				m_Falling_to_position = m_body->GetPosition();
			}
		}
	}


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

	//�\�E���̎��
	SpiritType m_type;

	//�A�j���[�V�����p�^�[��ID
	int m_anim_id = 1;

	//���̏��
	SpiritState m_state = Spirit_Idle;

	//�\�E�������������Ă���I�u�W�F�N�g�i�����͒n�ʁj
	std::list<b2Body*>m_CollidedObject;

	b2Vec2 m_Falling_to_position;	//�ǂ̍��W�܂ŗ����邩�i������Ԃ̎��Ɏg���j
};

#endif // !ITEM_SPIRIT_H
