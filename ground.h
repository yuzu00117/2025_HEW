//-----------------------------------------------------------------------------------------------------
// #name ground.h
// #description �n��
// #make 2024/11/22�@�i��`��
// #update 2024/11/22 
// #comment �ǉ��E�C���\��
//          �E�ǂɂ߂荞�ނƃW�����v�ł���̂ł����̏C��
//----------------------------------------------------------------------------------------------------

#ifndef GROUND_H
#define GROUND_H

#include"field.h"


//Filed���p�����Ă���@Draw�Ƃ��̏������̂�Field�ł���Ă邼
class Ground:public Field
{
public:
	Ground(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture);
	~Ground();


	//Updata

private:

};



#endif // !Ground_H
