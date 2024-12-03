//-----------------------------------------------------------------------------------------------------
// #name game.h
// #description ゲーム
// #make 2024/11/04　永野義也
// #update 2024/11/05
// #comment 追加・修正予定
//          ・Gameないの管理している　基本的にすべての関数がここにたどり着くことに
//           
//----------------------------------------------------------------------------------------------------
#ifndef GAME_H
#define GAME_H


#include"player.h"
#include"field.h"
#include"directx_controller.h"


class Game
{
public:
	Game();
	~Game();


	static Game& GetInstance() {
		static Game instance;
		return instance;
	}






	HRESULT Initialize(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Update();
	void Draw();
	void Finalize();

private:

	DirectInputController controller;
	Player player;

	
};





#endif // !GAME_H

