#pragma once

#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include <vector>
#include <memory>
#include"Item_SpeedUp.h"

//�A�C�e���̎��
enum ItemType
{
	ITEM_NONE,		//�����Ȃ�
	ITEM_SPEED_UP,	//�X�s�[�h�A�b�v
};



class ItemManager
{

public:
	static ItemManager& GetInstance();


	//�Œ�K�v�Ȉ����Fposition�i�ʒu���j�Abody_size�i�T�C�Y�j�Aangle�i��]�p�x�̃��W�A���j
	// �R���C�_�[�̌`�̓f�t�H���g�Ŏl�p�`�A�~�ɂ������ꍇ�� false ��n���A�ύX���Ȃ���Γ��ɒl��n���Ȃ��Ă�������
	// Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������
	void	AddSpeedUp(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f);
	



	// ID ���g���ăA�C�e��������
	ItemSpeedUp* FindItem_SpeedUp_ByID(int ID);
	
	// �S�ẴA�C�e����������
	void InitializeAll();

	// �S�ẴA�C�e�����X�V
	void UpdateAll();

	// �S�ẴA�C�e����`��
	void DrawAll();

	// �S�ẴA�C�e����j��
	void FinalizeAll();

	//�w��̃A�C�e�����폜
	void DestroyItem(int id, ItemType type);



private:

	std::vector<std::unique_ptr<ItemSpeedUp>> m_SpeedUp_List; // �̃��X�g
	//�����ɃA�C�e�����ƂɃ��X�g��ǉ����Ă����������˂��[

	ItemManager() = default;
	ItemManager(const ItemManager&) = delete;
	ItemManager& operator=(const ItemManager&) = delete;

};

#endif // !ITEM_MANAGER_H



