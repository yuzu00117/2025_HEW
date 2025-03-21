
//-----------------------------------------------------------------------------------------------------
// #name change_scene_end_production.h
// #description    シーン移行の際の終わる側の演出
// #make 2025/02/15　永野義也
// #update 2025/02/15
// #comment 追加・修正予定
//
//-------------------------------------------------------------------------------------------------------
#ifndef CHANGE_SCENE_END_PRODUCTION_H
#define CHANGE_SCENE_END_PRODUCTION_H



#include"main.h"



class change_scene_end_production
{

public:
	change_scene_end_production() = default;
	~change_scene_end_production() {};

	static void	Initialize();
	static void	Update();

	static void	Finalize();
	static void	Draw();

	static void Reset();

	static bool GetChangeFlag(void)
	{
		return Change_Flag;
	}

	static void SetChangeFlag(bool flag)
	{
		Change_Flag = flag;
	}

private:

	static float Change_Scene_Cnt;

	static float Black_fade_rate;


	static float Black_hole_rate;

	static float Explanation_cnt;


	static bool Change_Flag;

	static bool fade_end_flag;




};


#endif // !DEAD_PRODUCTION_H
