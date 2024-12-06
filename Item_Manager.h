#pragma once

#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include"ItemInterface.h"

#define	ITEM_MAX (10)

class ItemManager
{
public:
	ItemManager() {}
	~ItemManager() {}

	static ItemManager& GetInstance() {
		static ItemManager instance;
		return instance;
	}


	//�Œ�K�v�Ȉ����Fposition�i�ʒu���j�Abody_size�i�T�C�Y�j�Aangle�i��]�p�x�̃��W�A���j�Atype�i�A�C�e���̎�ށj
	// �R���C�_�[�̌`�̓f�t�H���g�Ŏl�p�`�A�~�ɂ������ꍇ�� false ��n���A�ύX���Ȃ���Γ��ɒl��n���Ȃ��Ă�������
	// Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������
	void	Create(b2Vec2 position, b2Vec2 body_size, float angle, ItemType type, bool shape_polygon = true, float Alpha = 1.0f);
	//body�̃A�h���X�����m��Ȃ��ꍇ
	void	Destory(b2Body* body);
	//�A�C�e���̔z��ԍ���m���Ă���ꍇ
	void	Destory(int item_ID);
	
	void	Update(int ID);
	void	Draw();

private:
	Item* m_p_item[ITEM_MAX];
};
#endif // !ITEM_MANAGER_H



