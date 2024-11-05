//-----------------------------------------------------------------------------------------------------
// #name game.h
// #description �Q�[��
// #make 2024/11/04
// #update 2024/11/03
// #comment �ǉ��E�C���\��
//          �EGame�Ȃ��̊Ǘ����Ă���@��{�I�ɂ��ׂĂ̊֐��������ɂ��ǂ蒅�����Ƃ�
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

	

	static Field*** m_p_field_array; // 2�����z��ւ̃|�C���^
	static int m_field_width;
	static int m_field_height;
};



#endif // !FEILD_H

