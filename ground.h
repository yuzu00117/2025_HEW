#ifndef GROUND_H
#define GROUND_H

#include"field.h"


//Filed‚ğŒp³‚µ‚Ä‚¢‚é@Draw‚Æ‚©‚Ìˆ—©‘Ì‚àField‚Å‚â‚Á‚Ä‚é‚¼
class Ground:public Field
{
public:
	Ground(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture);
	~Ground();


	//Updata

private:

};



#endif // !Ground_H
