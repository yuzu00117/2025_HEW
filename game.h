//-----------------------------------------------------------------------------------------------------
// #name game.h
// #description �Q�[��
// #make 2024/11/04�@�i��`��
// #update 2024/11/05
// #comment �ǉ��E�C���\��
//          �EGame�Ȃ��̊Ǘ����Ă���@��{�I�ɂ��ׂĂ̊֐��������ɂ��ǂ蒅�����Ƃ�
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

