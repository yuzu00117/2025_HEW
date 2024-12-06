#pragma once

#ifndef ITEM_SPEEDUP_H
#define	ITEM_SPEEDUP_H

#include"ItemInterface.h"
#include"texture.h"

class ItemSpeedUp : public Item
{
public:
	//�Œ�K�v�Ȉ����Fposition�i�ʒu���j�Abody_size�i�T�C�Y�j�Aangle�i��]�p�x�̃��W�A���j
	// �R���C�_�[�̌`�̓f�t�H���g�Ŏl�p�`�A�~�ɂ������ꍇ�� false ��n���A�ύX���Ȃ���Γ��ɒl��n���Ȃ��Ă�������
	// Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������
	ItemSpeedUp(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f)
		:Item(position, body_size, angle, ITEM_SPEED_UP, InitTexture(L"asset\\texture\\sample_texture\\speed_up.png"), shape_polygon, Alpha) {}
	

	void	Update()override;



private:

};


#endif // !ITEM_SPEEDUP_H
