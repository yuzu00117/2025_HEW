#ifndef GOKAI_H
#define GOKAI_H

// ---------------------------------------------------------------------------------------------------- -
// #name gokai.h
// #description     gokaiのUI表示用
// #make 2025/02/02　永野義也
// #update 2024/02/02
// #comment 追加・修正予定
//
//----------------------------------------------------------------------------------------------------



#include"main.h"



class Gokai_UI
{

public:
	Gokai_UI() = default;
	~Gokai_UI() {};

	static void	Initialize();
	static void	Finalize();
	static void	Draw();

	// 現在の豪快値を取得
	static int GetNowGokaiCount(void)
	{
		return now_get_gokai_count;
	}

	// 現在の豪快値を設定
	static void SetNowGokaiCount(int setCount)
	{
		now_get_gokai_count = setCount;
	}

	//追加：ごーかいゲージを
	static void AddGokaiCount(int setCount)
	{
		now_get_gokai_count =now_get_gokai_count +setCount ;
	}

	// リスポン時の豪快値を取得
	static int GetGokaiRecorded_WhenRespawn(void)
	{
		return gokai_when_respawn;
	}

	// リスポン時の豪快値を設定
	static void RecordGokai_WhenRespawn(int setCount)
	{
		gokai_when_respawn = setCount;
	}


private:

	static int max_gokai_count;
	static int now_get_gokai_count;
	static int gokai_when_respawn;



	
};



#endif // !GOKAI_H
