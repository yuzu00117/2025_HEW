
//-----------------------------------------------------------------------------------------------------
// #name dead_production.h
// #description    éÄñSââèo
// #make 2025/02/08Å@âiñÏã`ñÁ
// #update 2025/02/08
// #comment í«â¡ÅEèCê≥ó\íË
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


	static bool Change_Flag;




};


#endif // !DEAD_PRODUCTION_H
