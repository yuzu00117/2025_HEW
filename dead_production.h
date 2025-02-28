
//-----------------------------------------------------------------------------------------------------
// #name dead_production.h
// #description    éÄñSââèo
// #make 2025/02/08Å@âiñÏã`ñÁ
// #update 2025/02/08
// #comment í«â¡ÅEèCê≥ó\íË
//
//-------------------------------------------------------------------------------------------------------
#ifndef DEAD_PRODUCTION_H
#define DEAD_PRODUCTION_H



#include"main.h"



class dead_production
{

public:
	dead_production() = default;
	~dead_production() {};

	static void	Initialize();
	static void	Update();

	static void	Finalize();
	static void	Draw();

	static void Reset();

	static bool GetDeadFlag(void)
	{
		return Dead_Flag;
	}

	static void SetDeadFlag(bool flag)
	{
		Dead_Flag = flag;
	}

private:

	static float Dead_Cnt;

	static float Black_fade_rate;


	static float Black_hole_rate;


	static bool Dead_Flag;


	static float Game_over_cnt;




	static float Game_over_Black_fade_rate;

	static float Game_over_tama_cnt;
	static float Game_over_text_cnt;





	

	

};


#endif // !DEAD_PRODUCTION_H
