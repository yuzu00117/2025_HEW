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
#include"1-1_boss.h"
#include"object_manager.h"
#include"Item_Manager.h"


enum GAME_STATE
{
	GAME_STATE_START,				//ゲームシーンに入ったばかリ（ステージ選択から来た初回のみ）
	GAME_STATE_RESPAWN_INITIAL,		//ステージの初期位置にリスポン
	GAME_STATE_RESPAWN_SAVE_POINT,	//ステージの中間地にリスポン
	GAME_STATE_NEXT_STAGE,			//次のステージに移行
	GAME_STATE_GAMEOVER,			//リザルト画面に移行
};

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

	//今ゲームシーンの状態を取得
	GAME_STATE GetGameState() { return m_state; }


	void Teleport_player(b2Vec2 position);

private:
	//シングルトンをこっちに移動
	Player &player=Player::GetInstance();
	Boss_1_1 &boss =Boss_1_1::GetInstance();



	ObjectManager& objectManager = ObjectManager::GetInstance();
	ItemManager& itemManager = ItemManager::GetInstance();

	//今のゲームシーンの状態リスポンしてたのか、開始したばかりなのかなどなど
	GAME_STATE	m_state = GAME_STATE_START;
};





#endif // !GAME_H

