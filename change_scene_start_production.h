
//-----------------------------------------------------------------------------------------------------
// #name change_scene_start_production.h
// #description    シーン移行の際の始まる側の演出
// #make 2025/02/15　永野義也
// #update 2025/02/15
// #comment 追加・修正予定
//
//-------------------------------------------------------------------------------------------------------
#ifndef CHANGE_SCENE_START_PRODUCTION_H
#define CHANGE_SCENE_START_PRODUCTION_H



#include"main.h"



class change_scene_start_production
{

public:
	change_scene_start_production() = default;
	~change_scene_start_production() {};

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


	static bool Change_Flag;




};


#endif // !DEAD_PRODUCTION_H
#pragma once
