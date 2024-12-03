#ifndef OBJECT_H
#define OBJECT_H

#include "field.h"
#include"include/box2d/box2d.h"


#define OBJECT_MAX_WIDTH  7//��̃I�u�W�F�N�g�̍ő剡��
#define OBJECT_MAX_HEIGHT 7//��̃I�u�W�F�N�g�̍ő�c��

#define MAX_OBJECT_NUMBER 20//�ő�I�u�W�F�N�g���l


enum object_texture
{
	null,
	anchorpoint_texture,
	sample_blue,
	sample_yellow,
};


enum object_map_type
{
	type_wood,
	type_Rock,
};




class Object
{
public:
	Object(b2Vec2 position, b2Vec2 body_size, bool is_sensor, object_map_type type);
	~Object ();


	static void Initialize();

	static void SetObject(int x, int y, b2Vec2 body_size, bool is_sensor, object_map_type type);

	static void Update();
	static void Draw();
	static void Finalize();


	//�`��p�ɃT�C�Y���������Ă���
	b2Vec2 GetSize() const { return m_size; }
	void SetSize(b2Vec2 size) {
		m_size = size;
	}

	//bodey�̎擾
	b2Body* GetObjectBody(int index_x, int index_y)
	{
		return bodys[index_y][index_x];
	}

	void SetObjectBody(b2Body*body,int index_x,int index_y)
	{
		bodys[index_y][index_x] = body;
	}


	object_texture GetObjectTexture(int index_x, int index_y)
	{
		return textures[index_y][index_x];
	}

	void SetObjectTexture(object_texture texture, int index_x, int index_y)
	{
		textures[index_y][index_x] = texture;
	}

	
	object_map_type GetObjectMap()
	{
		return map_type;
	}

	void SetObjectMap(object_map_type object_map_name)
	{
		map_type = object_map_name;
	}


private:
	b2Body* bodys[OBJECT_MAX_HEIGHT][OBJECT_MAX_WIDTH];//body�̔z��

	b2Vec2 m_size;//�T�C�Y�̐ݒ�

	object_texture textures[OBJECT_MAX_HEIGHT][OBJECT_MAX_WIDTH];//�e�N�X�`���̔z��

	static Object* m_object_array;

	//�}�b�v�̔z��̒���
	object_map_type map_type;

	//�I�u�W�F�N�g�̓��ꕨ��p�ӂ���
	int object_map[OBJECT_MAX_HEIGHT][OBJECT_MAX_WIDTH] = {};
	


};





#endif // !OBJECT_H
