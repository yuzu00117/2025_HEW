#ifndef SAVE_H
#define SAVE_H

#include "field.h"


//���Ԓn�_�p�̃e�N�X�`������ground�ƕς��Ȃ��Ǝv��
class Save :public Field
{
public:
	Save(b2Vec2 position, b2Vec2 body_size, float angle, bool bFixed, bool is_sensor, FieldTexture texture);
	~Save();

private:

};







#endif //SAVE_H