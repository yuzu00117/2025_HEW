//-----------------------------------------------------------------------------------------------------
// #name Item_Coin_UI.h
// #description     coinのUI表示用
// #make 2024/12/28　永野義也
// #update 2024/02/01
// #comment 追加・修正予定
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

	// 現在のコイン数を取得
	static int GetNowCoinCount(void)
	{
		return now_get_coin_count;
	}

	// 現在のコイン数を設定
	static void SetNowCoinCount(int setCount)
	{
		now_get_coin_count = setCount;
	}


	// 中間地を登録した時のコイン取得数を取得
	static int GetCoinRecorded_WhenRegisteringSavePoint()
	{
		return coin_got_when_registering_SavePoint;
	}

	// 中間地を登録した時現在のコイン取得数を設定
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
	static int coin_got_when_registering_SavePoint;		//中間地を登録する時のコイン取得数

	static float coin_effect_frame;

	static int DrawCount;
};

#endif // !ITEM_COIN_H
