//-----------------------------------------------------------------------------------------------------
// #name field.h
// #description field.h
// #make 2024/11/04�@�i��`��
// #update 2024/12/01
// #comment �ǉ��E�C���\��
//          �EGame�Ȃ��̊Ǘ����Ă���@��{�I�ɂ��ׂĂ̊֐��������ɂ��ǂ蒅�����Ƃ�
//           
//----------------------------------------------------------------------------------------------------
#ifndef FIELD_H
#define FIELD_H



#include"include/box2d/box2d.h"
#include"texture.h"
#include<vector>

// csv�Ń}�b�v��ǂݍ��ނ̂ɕK�v
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>



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

	static void Initialize();
	static void Update();
	static void Draw();
	static void Finalize();

	// csv�t�@�C����ǂݍ��݁A�񎟌��z��Ƃ��Ċi�[
	static bool LoadCSV(const std::string& filename);

	// �T�C�Y�̎擾�Ɛݒ�
	b2Vec2 GetSize() const { return m_size; }
	void SetSize(const b2Vec2 size) { m_size = size; }

	//body�̎擾�Ɛݒ�
	b2Body* GetFieldBody(void){return m_body;}
	void SetFieldBody(b2Body* field_body) { m_body = field_body; }

	//�t�B�[���h�̃e�N�X�`���̃Q�b�^�[�ƃZ�b�^�[
	FieldTexture  GetFieldTexture(void){return texture;}
	void SetFieldTexture(const FieldTexture field_texture) { texture = field_texture; }

private:

	//Body
	b2Body *m_body;

	//�\���Ɏg�p����T�C�Y
	b2Vec2 m_size;

	//�t�B�[���h�̃e�N�X�`�����Ǘ�����֐�
	FieldTexture texture;

	// 2�����}�b�v�f�[�^��ێ�����ϐ�
    static std::vector<std::vector<int>> m_field_data;  // CSV�ǂݍ��݌�̃}�b�v�f�[�^

	// 2�����z��ւ̃|�C���^
	static Field*** m_p_field_array;
	
	//�t�B�[���h�̕��ƍ���
	static int m_field_width;
	static int m_field_height;
};



#endif // !FIELD_H

