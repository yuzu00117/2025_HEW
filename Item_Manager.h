#pragma once

#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include <vector>
#include <memory>
#include"ItemInterface.h"
#include"Item_SpeedUp.h"


class ItemManager
{

public:
	static ItemManager& GetInstance();


	//�Œ�K�v�Ȉ����Fposition�i�ʒu���j�Abody_size�i�T�C�Y�j�Aangle�i��]�p�x�̃��W�A���j
	// �R���C�_�[�̌`�̓f�t�H���g�Ŏl�p�`�A�~�ɂ������ꍇ�� false ��n���A�ύX���Ȃ���Γ��ɒl��n���Ȃ��Ă�������
	// Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������
	void	AddSpeedUp(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f);
	



	// ID ���g���ăA�C�e��������
	Item* FindItem_SpeedUp_ByID(int ID);
	
	// �S�ẴA�C�e����������
	void InitializeAll();

	// �S�ẴA�C�e�����X�V
	void UpdateAll();

	// �S�ẴA�C�e����`��
	void DrawAll();

	// �S�ẴA�C�e����j��
	void FinalizeAll();

	
	
	
	////body�̃A�h���X�����m��Ȃ��ꍇ
	//void	Destory(b2Body* body);
	////�A�C�e���̔z��ԍ���m���Ă���ꍇ
	//void	Destory(int item_ID);
	


	//void	Update(b2Body* body);
	//void	Draw();

private:

	std::vector<std::unique_ptr<ItemSpeedUp>> m_SpeedUp_List; // �̃��X�g
	//�����ɃA�C�e�����ƂɃ��X�g��ǉ����Ă����������˂��[

	ItemManager() = default;
	//~ItemManager(){}
	//ItemManager(const ItemManager&) = delete;
	//ItemManager& operator=(const ItemManager&) = delete;

};

#endif // !ITEM_MANAGER_H



