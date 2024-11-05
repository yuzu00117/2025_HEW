//-----------------------------------------------------------------------------------------------------
// #name game.h
// #description ゲーム
// #make 2024/11/04
// #update 2024/11/03
// #comment 追加・修正予定
//          ・Gameないの管理している　基本的にすべての関数がここにたどり着くことに
//           
//----------------------------------------------------------------------------------------------------
#ifndef FEILD_H
#define FEILD_H


#include"square_collider.h"
#include"include/box2d/box2d.h"
#include"texture.h"

class Field:public SquareCollider
{
public:
	Field(XMFLOAT2 position, XMFLOAT2 size, float angle, bool bFixed,bool is_sensor, bool in_piece);
	~Field();


	static void Init(int field_width,int field_height);
	static void Update();
	static void Draw();
	static void Uninit();

private:

	

	static Field*** m_p_field_array; // 2次元配列へのポインタ
	static int m_field_width;
	static int m_field_height;
};



#endif // !FEILD_H

