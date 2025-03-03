//-----------------------------------------------------------------------------------------------------
// #name Item_Coin_UI.h
// #description     coin��UI�\���p
// #make 2024/12/28�@�i��`��
// #update 2024/02/01
// #comment �ǉ��E�C���\��
//
//----------------------------------------------------------------------------------------------------
#ifndef ITEM_COIN_UI_H
#define ITEM_COIN_UI_H



#include"main.h"



class Item_Coin_UI
{

public:
	Item_Coin_UI() = default;
	~Item_Coin_UI() {};

	static void	Initialize();
	static void	Finalize();
	static void	Draw();

	// ���݂̃R�C�������擾
	static int GetNowCoinCount(void)
	{
		return now_get_coin_count;
	}

	// ���݂̃R�C������ݒ�
	static void SetNowCoinCount(int setCount)
	{
		now_get_coin_count = setCount;
	}


	// ���Ԓn��o�^�������̃R�C���擾�����擾
	static int GetCoinRecorded_WhenRegisteringSavePoint()
	{
		return coin_got_when_registering_SavePoint;
	}

	// ���Ԓn��o�^���������݂̃R�C���擾����ݒ�
	static void RecordCoinGot_WhenRegisteringSavePoint(int value)
	{
		coin_got_when_registering_SavePoint = value;
	}

	static int GetDrawCount(void)
	{
		return DrawCount;
	}

	static void SetDrawCount(int setCount)
	{
		DrawCount = setCount;
	}

private:

	static int max_coin_count;
	static int now_get_coin_count;
	static int coin_got_when_registering_SavePoint;		//���Ԓn��o�^���鎞�̃R�C���擾��

	static float coin_effect_frame;

	static int DrawCount;
};

#endif // !ITEM_COIN_H
