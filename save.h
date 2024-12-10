#ifndef SAVE_H
#define SAVE_H

#include "field.h"


//中間地点用のテクスチャ多分groundと変わらないと思う
class Save :public Field
{
public:
	Save(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture);
	~Save();

private:

};







#endif //SAVE_H