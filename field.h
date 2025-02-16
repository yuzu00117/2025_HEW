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

//csvでマップを読み込むのに必要
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

	enemy_dynamic_texture,
	enemy_static_texture,
  

	STAGE_BLOCK_GRASS,//草
	STAGE_BLOCK_GRASS_RIGHT,//草の右側
	STAGE_BLOCK_GRASS_LEFT,//草の左側

	STAGE_BLOCK_EARTH,//土
	STAGE_BLOCK_EARTH_UNDER_RIGHT,//土右側
	STAGE_BLOCK_EARTH_UNDER_LEFT,//土左側


	STAGE_BLOCK_INVISIBILITY,//不可視の壁

	

	STAGE_BLOCK_SLOOP_SIDE_RIGHT,
	STAGE_BLOCK_SLOOP_SIDE_LEFT,


	STAGE_BLOCK_TYPE_1,
	STAGE_BLOCK_TYPE_2,
	STAGE_BLOCK_TYPE_3,
	STAGE_BLOCK_TYPE_4,
	STAGE_BLOCK_TYPE_5,
	STAGE_BLOCK_TYPE_6,
	STAGE_BLOCK_TYPE_7,
	STAGE_BLOCK_TYPE_8,
	STAGE_BLOCK_TYPE_9,
	STAGE_BLOCK_TYPE_10,
	STAGE_BLOCK_TYPE_11,
	STAGE_BLOCK_TYPE_12,
	STAGE_BLOCK_TYPE_13,
	STAGE_BLOCK_TYPE_14,
};

enum class StageType {
    Stage1,
    Stage2,
    Stage3,
    // 他のステージを追加
};

class Field 
{
public:
	Field();
	virtual ~Field();

	static void Initialize(bool respawning = false);
	static void Update();
	static void Draw();
	static void Finalize(bool respawning = false);

	//csvファイルを読み込み、二次元配列として格納
	static bool LoadCSV(const std::string& filename);

	// マップの切り替え
    void LoadMap(StageType stage_type);

	// サイズの取得と設定
	b2Vec2 GetSize() const { return m_size; }
	void SetSize(const b2Vec2 size) { m_size = size; }

  //bodyの取得と設定
	b2Body* GetFieldBody(void){return m_body;}
	void SetFieldBody(b2Body* field_body) { m_body = field_body; }

	//フィールドのテクスチャのゲッター　セッター
	FieldTexture  GetFieldTexture(void){return texture;}
	void SetFieldTexture(const FieldTexture field_texture) { texture = field_texture; }

	static void DeleteFieldObject(b2Body* delete_object);
  
private:

	//Body
	b2Body *m_body;

	//表示に使用するサイズ
	b2Vec2 m_size;

	//フィールドのテクスチャを管理する関数
	FieldTexture texture;

  //2次元マップデータを保持する変数
  static std::vector<std::vector<int>> m_field_data;  // CSV読み込み後のマップデータ

	// 2次元配列へのポインタ
	static Field*** m_p_field_array;
	
	//フィールドの幅と高さ
	static int m_field_width;
	static int m_field_height;
};



#endif // !FIELD_H