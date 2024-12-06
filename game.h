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
#include"UI_StaminaSpirit_gauge.h"



class Game
{
public:
	Game();
	~Game();


	static Game& GetInstance() {
		static Game instance;
		return instance;
	}







	void Initialize();
	void Update();
	void Draw();
	void Finalize();

private:

	DirectInputController controller;
	Player player;
	StaminaSpiritGauge stamina_spirit_gauge;
	
};





#endif // !GAME_H

