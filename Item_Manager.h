//-----------------------------------------------------------------------------------------------------
// #name Item_Manager.h
// #description �A�C�e���̊Ǘ�(�t�@�N�g���[)
// #make 2024/12/28�@���j�S
// #update 2024/12/28
// #comment �ǉ��E�C���\��
//      
//
// 
//----------------------------------------------------------------------------------------------------

#pragma once

#ifndef ITEM_MANAGER_H
#define ITEM_MANAGER_H

#include <vector>
#include <memory>
#include"Item_coin.h"
#include"Item_Spirit.h"
#include"Item_Jewel.h"
#include"Item_SavePoint.h"

//�A�C�e���̎��
enum ItemType
{
	ITEM_NONE,		//�����Ȃ�
	ITEM_SPIRIT,	//�\�E���i�G�����Ƃ��A�C�e���j
	ITEM_COIN,//�R�C��
	ITEM_JEWEL,	//���
	ITEM_SAVEPOINT,	//�Z�[�u�|�C���g
};



class ItemManager
{

public:
	static ItemManager& GetInstance();


	//�Œ�K�v�Ȉ����Fposition�i�ʒu���j�Abody_size�i�T�C�Y�j�Aangle�i��]�p�x�̃��W�A���j
	// �R���C�_�[�̌`�̓f�t�H���g�Ŏl�p�`�A�~�ɂ������ꍇ�� false ��n���A�ύX���Ȃ���Γ��ɒl��n���Ȃ��Ă�������
	// Alpha�l�̓f�t�H���g��1.0�A�ύX���Ȃ���Βl��n���Ȃ��Ă�������
	//recovery�����̓v���C���[���A�C�e�������Ƃ��̃\�E���̉񕜒l
	void	AddSpirit(b2Vec2 position, b2Vec2 body_size, float angle, SpiritType type, float Alpha = 1.0f);
	void	AddCoin(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f);
	//Jewel_Type�^��type������ BLUE, RED, YELLOW �̂ǂꂩ��n��
	void	AddJewel(b2Vec2 position, b2Vec2 body_size, float angle, Jewel_Type type, bool shape_polygon = true, float Alpha = 1.0f);
	void	AddSavePoint(b2Vec2 position, b2Vec2 body_size, float angle, bool shape_polygon = true, float Alpha = 1.0f);

	// ID ���g���ăA�C�e��������
	ItemSpirit* FindItem_Spirit_ByID(int ID);
	ItemCoin* FindItem_Coin_ByID(int ID);
	ItemJewel* FindItem_Jewel_ByID(int ID);
	ItemSavePoint* FindItem_SavePoint_ByID(int ID);

	// �S�ẴA�C�e����������
	void InitializeAll();

	// �S�ẴA�C�e�����X�V
	void UpdateAll();

	// �S�ẴA�C�e����`��
	void DrawAll();

	//�S�ʂ̕`�揈���@�G�t�F�N�g�Ȃ�
	void DrawFront();

	// �S�ẴA�C�e����j��
	void FinalizeAll();

	//�@�S�Ẵ\�E���A�C�e������������\�肾�Ɛݒ肷��
	void	SetCollectSpirit(bool flag);

	//�S�Ă̕�΂��g��
	void	UseAllJewel();


private:

	std::vector<std::unique_ptr<ItemSpirit>> m_Spirit_List; // �\�E���A�C�e���̃��X�g
	std::vector<std::unique_ptr<ItemCoin>> m_Coin_List; // �R�C���̃��X�g
	std::list<std::unique_ptr<ItemJewel>> m_Jewel_List; // ��΂̃��X�g
	std::vector<std::unique_ptr<ItemSavePoint>> m_SavePoint_List; // �Z�[�u�|�C���g�̃��X�g
	//�����ɃA�C�e�����ƂɃ��X�g��ǉ����Ă����������˂��[

	ItemManager() = default;
	ItemManager(const ItemManager&) = delete;
	ItemManager& operator=(const ItemManager&) = delete;

};

#endif // !ITEM_MANAGER_H



