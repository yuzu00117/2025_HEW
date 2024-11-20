//-----------------------------------------------------------------------------------------------------
// #name field.h
// #description field.h
// #make 2024/11/04
// #update 2024/11/03
// #comment 追加・修正予定
//          ・Gameないの管理している　基本的にすべての関数がここにたどり着くことに
//           
//----------------------------------------------------------------------------------------------------
#ifndef FEILD_H
#define FEILD_H



#include"include/box2d/box2d.h"
#include"texture.h"
#include<vector>



//フィールドのテクスチャを管理するためのenum型　
//テクスチャの型自体は別にfield.cppのグローバル変数になってて　これ自体は識別用
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


	// サイズの取得と設定
	b2Vec2 GetSize() const { return m_size; }
	void SetSize(b2Vec2 size) {
		m_size = size;
	}

	//bodyの取得
	b2Body* GetFieldBody(void){return m_body;}
	void SetFieldBody(b2Body* field_body)
	{
		m_body = field_body;
	}


	//フィールドのテクスチャのゲッター　セッター
	FieldTexture  GetFieldTexture(void){return texture;}
	void SetFieldTexture(FieldTexture field_texture)
	{
		texture = field_texture;
	}

private:

	//Body
	b2Body *m_body;

	//表示に使用するサイズ
	b2Vec2 m_size;


	//フィールドのテクスチャを管理する関数
	FieldTexture texture;


	static Field*** m_p_field_array; // 2次元配列へのポインタ
	static int m_field_width;
	static int m_field_height;


	
};



#endif // !FEILD_H

