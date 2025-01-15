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
#include"UI_StaminaSpirit_Gauge.h"
#include"1-1_boss.h"


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

	void Teleport_player(b2Vec2 position);

private:
	
	Player player;
	StaminaSpiritGauge stamina_spirit_gauge;

	Boss_1_1 boss;
	
};





#endif // !GAME_H

