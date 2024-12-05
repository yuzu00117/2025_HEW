#pragma once

#ifndef ITEM_H
#define ITEM_H

#include"world_box2d.h"
#include"renderer.h"

//�A�C�e���̎��
enum ITEM
{
	ITEM_NONE,		//�����Ȃ�
	ITEM_SPEED_UP,	//�X�s�[�h�A�b�v
};

class ItemFactory
{
public:
	 ItemFactory(b2Vec2 position, b2Vec2 scale, float rotate, ITEM type, wchar_t* texture_name, bool shape_polygon = true, float Alpha = 1.0f);
	 ~ItemFactory();
	
	 //�`��p�ɃT�C�Y���������Ă���
	 b2Vec2 GetSize() const
	 {
		 return m_size;
	 }
	 void SetSize(b2Vec2 size)
	 {
		 m_size = size;
	 }

	 //���̃A�C�e���Ȃ̂����̎�ނ��擾
	 ITEM	GetType() { return m_type; }

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

	//�A�C�e���̃T�C�Y�i�`��p�j
	b2Vec2 m_size;
	//�A�C�e���̎��
	ITEM m_type = ITEM_NONE;
	//�A�C�e���̓����x
	float m_Alpha;

	//�e�N�X�`��
	ID3D11ShaderResourceView* m_Texture = NULL;

};


#endif // !ITEM_H

