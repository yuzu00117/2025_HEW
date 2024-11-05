//-----------------------------------------------------------------------------------------------------
// #name game.h
// #description ゲーム
// #make 2024/11/04
// #update 2024/11/03
// #comment 追加・修正予定
//          ・Gameないの管理している　基本的にすべての関数がここにたどり着くことに
//           
//----------------------------------------------------------------------------------------------------
#ifndef GAME_H
#define GAME_H

class Game
{
public:
	Game();
	~Game();


	static Game& GetInstance() {
		static Game instance;
		return instance;
	}

	//初期化
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Update();
	void Draw();
	void Uninit();

private:
	
};





#endif // !GAME_H

