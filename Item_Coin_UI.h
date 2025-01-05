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

	static int GetNowCoinCount(void)
	{
		return now_get_coin__count;
	}

	static void SetNowCoinCount(int setCount)
	{
		now_get_coin__count = setCount;
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
	static int now_get_coin__count;



	static int DrawCount;
};

#endif // !ITEM_COIN_H
