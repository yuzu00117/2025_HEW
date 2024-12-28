//-----------------------------------------------------------------------------------------------------
// #name player_life.h
// #description プレイヤーの残機表示
// #make 2024/12/15　　今村友哉
// #update 2024/12/15
// #comment 
//          
//----------------------------------------------------------------------------------------------------
#ifndef PLAYER_LIFE_H_
#define PLAYER_LIFE_H_


class PlayerLife
{
public:
	static void Initialize();
	static void Update();
	static void Draw();
	static void Finalize();

private:

	static float m_life;
};

#endif
