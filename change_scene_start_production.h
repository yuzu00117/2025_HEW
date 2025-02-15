
//-----------------------------------------------------------------------------------------------------
// #name change_scene_start_production.h
// #description    �V�[���ڍs�̍ۂ̎n�܂鑤�̉��o
// #make 2025/02/15�@�i��`��
// #update 2025/02/15
// #comment �ǉ��E�C���\��
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
