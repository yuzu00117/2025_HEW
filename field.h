//-----------------------------------------------------------------------------------------------------
// #name field.h
// #description field.h
// #make 2024/11/04　永野義也
// #update 2024/12/01
// #comment 追加・修正予定
//          ・Gameないの管理している　基本的にすべての関数がここにたどり着くことに
//           
//----------------------------------------------------------------------------------------------------
#ifndef FIELD_H
#define FIELD_H



#include"include/box2d/box2d.h"
#include"texture.h"
#include<vector>

// csvでマップを読み込むのに必要
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>



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

	static void Initialize();
	static void Update();
	static void Draw();
	static void Finalize();

	// csvファイルを読み込み、二次元配列として格納
	static bool LoadCSV(const std::string& filename);

	// サイズの取得と設定
	b2Vec2 GetSize() const { return m_size; }
	void SetSize(const b2Vec2 size) { m_size = size; }

	//bodyの取得と設定
	b2Body* GetFieldBody(void){return m_body;}
	void SetFieldBody(b2Body* field_body) { m_body = field_body; }

	//フィールドのテクスチャのゲッターとセッター
	FieldTexture  GetFieldTexture(void){return texture;}
	void SetFieldTexture(const FieldTexture field_texture) { texture = field_texture; }

private:

	//Body
	b2Body *m_body;

	//表示に使用するサイズ
	b2Vec2 m_size;

	//フィールドのテクスチャを管理する関数
	FieldTexture texture;

	// 2次元マップデータを保持する変数
    static std::vector<std::vector<int>> m_field_data;  // CSV読み込み後のマップデータ

	// 2次元配列へのポインタ
	static Field*** m_p_field_array;
	
	//フィールドの幅と高さ
	static int m_field_width;
	static int m_field_height;
};



#endif // !FIELD_H

