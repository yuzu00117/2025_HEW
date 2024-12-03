#ifndef OBJECT_H
#define OBJECT_H

#include "field.h"
#include"include/box2d/box2d.h"


#define OBJECT_MAX_WIDTH  7//一つのオブジェクトの最大横幅
#define OBJECT_MAX_HEIGHT 7//一つのオブジェクトの最大縦幅

#define MAX_OBJECT_NUMBER 20//最大オブジェクト数値


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


	//描画用にサイズを持たせておく
	b2Vec2 GetSize() const { return m_size; }
	void SetSize(b2Vec2 size) {
		m_size = size;
	}

	//bodeyの取得
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
	b2Body* bodys[OBJECT_MAX_HEIGHT][OBJECT_MAX_WIDTH];//bodyの配列

	b2Vec2 m_size;//サイズの設定

	object_texture textures[OBJECT_MAX_HEIGHT][OBJECT_MAX_WIDTH];//テクスチャの配列

	static Object* m_object_array;

	//マップの配列の調整
	object_map_type map_type;

	//オブジェクトの入れ物を用意する
	int object_map[OBJECT_MAX_HEIGHT][OBJECT_MAX_WIDTH] = {};
	


};





#endif // !OBJECT_H
