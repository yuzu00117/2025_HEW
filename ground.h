//-----------------------------------------------------------------------------------------------------
// #name ground.h
// #description 地面
// #make 2024/11/22　永野義也
// #update 2024/11/22 
// #comment 追加・修正予定
//          ・壁にめり込むとジャンプできるのでそこの修正
//----------------------------------------------------------------------------------------------------

#ifndef GROUND_H
#define GROUND_H

#include"field.h"


//Filedを継承している　Drawとかの処理自体もFieldでやってるぞ
class Ground:public Field
{
public:
	Ground(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture);
	~Ground();


	//Updata

private:

};



#endif // !Ground_H
