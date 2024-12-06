#pragma once

#ifndef ITEM_SPEEDUP_H
#define	ITEM_SPEEDUP_H

#include"include/box2d/box2d.h"

class ItemSpeedUp 
{
public:
	//�Œ�K�v�Ȉ����Fposition�i�ʒu���j�Abody_size�i�T�C�Y�j�Aangle�i��]�p�x�̃��W�A���j
	// �R���C�_�[�̌`�̓f�t�H���g�Ŏl�p�`�A�~�ɂ������ꍇ�� false ��n���A�ύX���Ȃ���Γ��ɒl��n���Ȃ��Ă�������
	// Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������
	ItemSpeedUp(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f);
	




private:
	//�A�C�e���̃{�f�B�[
	b2Body* m_body;

	//�A�C�e���̃T�C�Y�i�`��p�j
	b2Vec2 m_size;

	//�A�C�e���̓����x
	float m_Alpha;

	//�����\��Ȃ̂��ǂ���
	bool	m_destory = false;

};


#endif // !ITEM_SPEEDUP_H
