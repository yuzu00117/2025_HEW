//-----------------------------------------------------------------------------------------------------
// #name field.h
// #description field.h
// #make 2024/11/04
// #update 2024/11/03
// #comment �ǉ��E�C���\��
//          �EGame�Ȃ��̊Ǘ����Ă���@��{�I�ɂ��ׂĂ̊֐��������ɂ��ǂ蒅�����Ƃ�
//           
//----------------------------------------------------------------------------------------------------
#ifndef FEILD_H
#define FEILD_H



#include"include/box2d/box2d.h"
#include"texture.h"
#include<vector>



//�t�B�[���h�̃e�N�X�`�����Ǘ����邽�߂�enum�^�@
//�e�N�X�`���̌^���͕̂ʂ�field.cpp�̃O���[�o���ϐ��ɂȂ��Ăā@���ꎩ�͎̂��ʗp
enum FieldTexture
{
	ground_texture,
	anchor_point_texture,
};

class Field 
{
public:
	Field();
	~Field();


	static void Initialize(int field_width, int field_height);
	static void Update();
	static void Draw();
	static void Finalize();


	// �T�C�Y�̎擾�Ɛݒ�
	b2Vec2 GetSize() const { return m_size; }
	void SetSize(b2Vec2 size) {
		m_size = size;
	}

	//body�̎擾
	b2Body* GetFieldBody(void){return m_body;}
	void SetFieldBody(b2Body* field_body)
	{
		m_body = field_body;
	}


	//�t�B�[���h�̃e�N�X�`���̃Q�b�^�[�@�Z�b�^�[
	FieldTexture  GetFieldTexture(void){return texture;}
	void SetFieldTexture(FieldTexture field_texture)
	{
		texture = field_texture;
	}

private:

	//Body
	b2Body *m_body;

	//�\���Ɏg�p����T�C�Y
	b2Vec2 m_size;


	//�t�B�[���h�̃e�N�X�`�����Ǘ�����֐�
	FieldTexture texture;


	static Field*** m_p_field_array; // 2�����z��ւ̃|�C���^
	static int m_field_width;
	static int m_field_height;


	
};



#endif // !FEILD_H

