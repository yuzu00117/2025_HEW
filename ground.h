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
