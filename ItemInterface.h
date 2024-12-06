#pragma once

#ifndef ITEM_H
#define ITEM_H

#include"world_box2d.h"
#include"renderer.h"

//�A�C�e���̎��
enum ItemType
{
	ITEM_NONE,		//�����Ȃ�
	ITEM_SPEED_UP,	//�X�s�[�h�A�b�v
};

class Item
{
public:
	//angle�̓��W�A���ł���
	// �R���C�_�[�̌`�̓f�t�H���g�Ŏl�p�`�A�~�ɂ������ꍇ�� false ��n���A�ύX���Ȃ���Γ��ɒl��n���Ȃ��Ă�������
	// Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������
	 Item(b2Vec2 position, b2Vec2 body_size, float angle, ItemType type, ID3D11ShaderResourceView* Texture, bool shape_polygon = true, float Alpha = 1.0f);
	 ~Item();
	
	 //�{�f�B�[���擾
	 b2Body* GetBody(){ return m_body; }
	 //�{�f�B�[���Z�b�g
	 void SetBody(b2Body* body){ m_body = body; }


	 // ID ���擾����
	 int GetID() const {return m_ID; }
	 // ID ���Z�b�g
	 void SetID(int ID) { m_ID = ID; }


	 //������\��Ȃ̂����擾
	 bool	GetDestory() { return m_destory; }
	 //�C���X�^���X���폜���邩�ǂ����̃Z�b�g
	 void	SetDestory(bool destory) { m_destory = destory; }

	 //�`��p�ɃT�C�Y���������Ă���
	 b2Vec2 GetSize() const{ return m_size; }
	 //�`��T�C�Y�Z�b�g
	 void SetSize(b2Vec2 size){ m_size = size; }


	 //���̃A�C�e���Ȃ̂����̎�ނ��擾
	 ItemType	GetType() { return m_type; }


	 //���̃e�N�X�`�����擾
	 ID3D11ShaderResourceView* GetTexture() { return m_Texture; }
	 //�e�N�X�`�����Z�b�g
	 void	SetTexture(ID3D11ShaderResourceView* texture) { m_Texture = texture; }


	 //�p����ɃI�[�o�[���C�h�����\��
	 virtual	void	Update() = 0;
	 //�`��
	 void	Draw();
	
protected:
	//�A�C�e���̃{�f�B�[
	b2Body*	m_body;

	// �e�C���X�^���X�ŗL�� ID
	int m_ID; 

	//�A�C�e���̃T�C�Y�i�`��p�j
	b2Vec2 m_size;
	//�A�C�e���̎��
	ItemType m_type = ITEM_NONE;
	//�A�C�e���̓����x
	float m_Alpha;

	//�����\��Ȃ̂��ǂ���
	bool	m_destory = false;

	//�e�N�X�`��
	ID3D11ShaderResourceView* m_Texture = NULL;

};


#endif // !ITEM_H

