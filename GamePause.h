//-----------------------------------------------------------------------------------------------------
// #name GamePause.h
// #description     �Q�[���̃|�[�Y���
// #make 2025/2/28�@���j�S
// #update 2025/2/28
// #comment �ǉ��E�C���\��
//                  
//                  
//----------------------------------------------------------------------------------------------------
#ifndef GAMEPAUSE_H
#define GAMEPAUSE_H

#include"include/box2d/box2d.h"
#include"KeyInput_Flag.h"


#define	BUTTON_NUM	(5)

enum ButtonSelection
{
	Button_UnPause = 0,
	Button_Respawn_SavePoint,
	Button_Respawn_InitalPoint,
	Button_SelectScene,
	Button_TitleScene,

	Button_NULL,
};



class GamePause
{
public:
	GamePause() {}
	~GamePause() {}

	void	Initialize();
	void	Finalize();
	void	Update();
	void	Draw();


private:
	ButtonSelection m_button_selected = Button_NULL;
	Button_PressFlag key_flag;
};


#endif // !GAMEPAUSE_H


