//-----------------------------------------------------------------------------------------------------
// #name player_life.h
// #description �v���C���[�̎c�@�\��
// #make 2024/12/15�@�@�����F��
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
